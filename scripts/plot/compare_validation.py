#!/usr/bin/env python3
"""Compare validation CSVs against optional numeric reference curves."""

from __future__ import annotations

from pathlib import Path

import numpy as np
import pandas as pd


ROOT = Path(__file__).resolve().parents[2]
SOLVER_DIR = ROOT / "data" / "output" / "validation"
REFERENCE_DIR = ROOT / "data" / "input" / "reference"
OUTPUT = ROOT / "out" / "validation" / "validation_summary.csv"
VALID_STATUSES = {"propagating"}


SUMMARY_COLUMNS = [
    "case_id",
    "curve_id",
    "status",
    "n_points",
    "max_abs",
    "mean_abs",
    "max_rel",
    "mean_rel",
    "target",
    "max_target",
    "mean_target",
]


def read_many(directory: Path) -> pd.DataFrame:
    frames = [pd.read_csv(path) for path in sorted(directory.glob("*.csv"))]
    if not frames:
        return pd.DataFrame()
    return pd.concat(frames, ignore_index=True)


def target_for(case_id: str) -> tuple[str, float, float]:
    if case_id.startswith("fig3"):
        return "relative", 0.05, 0.02
    if case_id.startswith(("fig5", "fig7")):
        return "absolute", 0.05, 0.02
    return "unspecified", np.nan, np.nan


def no_reference_rows(solver: pd.DataFrame) -> list[dict[str, object]]:
    rows: list[dict[str, object]] = []
    for (case_id, curve_id), _ in solver.groupby(["case_id", "curve_id"], sort=False):
        target, max_target, mean_target = target_for(str(case_id))
        rows.append(
            {
                "case_id": case_id,
                "curve_id": curve_id,
                "status": "no_reference",
                "n_points": 0,
                "max_abs": np.nan,
                "mean_abs": np.nan,
                "max_rel": np.nan,
                "mean_rel": np.nan,
                "target": target,
                "max_target": max_target,
                "mean_target": mean_target,
            }
        )
    return rows


def compare_curve(solver_curve: pd.DataFrame, reference_curve: pd.DataFrame) -> dict[str, float]:
    solver_curve = solver_curve.copy()
    reference_curve = reference_curve.copy()
    if "status" in solver_curve.columns:
        solver_curve = solver_curve[solver_curve["status"].isin(VALID_STATUSES)]
    solver_curve["x"] = pd.to_numeric(solver_curve["x"], errors="coerce")
    solver_curve["y"] = pd.to_numeric(solver_curve["y"], errors="coerce")
    reference_curve["x"] = pd.to_numeric(reference_curve["x"], errors="coerce")
    reference_curve["y"] = pd.to_numeric(reference_curve["y"], errors="coerce")
    solver_curve = solver_curve.dropna(subset=["x", "y"]).sort_values("x")
    reference_curve = reference_curve.dropna(subset=["x", "y"]).sort_values("x")

    if solver_curve.empty or len(reference_curve) < 2:
        return {"n_points": 0, "max_abs": np.nan, "mean_abs": np.nan,
                "max_rel": np.nan, "mean_rel": np.nan}

    x_ref = reference_curve["x"].to_numpy(dtype=float)
    y_ref = reference_curve["y"].to_numpy(dtype=float)
    x_solver = solver_curve["x"].to_numpy(dtype=float)
    y_solver = solver_curve["y"].to_numpy(dtype=float)
    inside = (x_solver >= x_ref.min()) & (x_solver <= x_ref.max())
    if not inside.any():
        return {"n_points": 0, "max_abs": np.nan, "mean_abs": np.nan,
                "max_rel": np.nan, "mean_rel": np.nan}

    y_interp = np.interp(x_solver[inside], x_ref, y_ref)
    error = y_solver[inside] - y_interp
    abs_error = np.abs(error)
    rel_mask = np.abs(y_interp) > 1.0e-12
    rel_error = np.abs(error[rel_mask] / y_interp[rel_mask])

    return {
        "n_points": int(abs_error.size),
        "max_abs": float(abs_error.max()),
        "mean_abs": float(abs_error.mean()),
        "max_rel": float(rel_error.max()) if rel_error.size else np.nan,
        "mean_rel": float(rel_error.mean()) if rel_error.size else np.nan,
    }


def main() -> int:
    solver = read_many(SOLVER_DIR)
    if solver.empty:
        raise SystemExit("compare_validation: no solver CSVs found")

    reference = read_many(REFERENCE_DIR) if REFERENCE_DIR.exists() else pd.DataFrame()
    if reference.empty:
        rows = no_reference_rows(solver)
    else:
        rows = []
        for (case_id, curve_id), solver_curve in solver.groupby(["case_id", "curve_id"], sort=False):
            reference_curve = reference[
                (reference["case_id"] == case_id) & (reference["curve_id"] == curve_id)
            ]
            target, max_target, mean_target = target_for(str(case_id))
            if reference_curve.empty:
                metrics = {"n_points": 0, "max_abs": np.nan, "mean_abs": np.nan,
                           "max_rel": np.nan, "mean_rel": np.nan}
                status = "no_reference"
            else:
                metrics = compare_curve(solver_curve, reference_curve)
                status = "compared" if metrics["n_points"] else "no_overlap"
            rows.append(
                {
                    "case_id": case_id,
                    "curve_id": curve_id,
                    "status": status,
                    **metrics,
                    "target": target,
                    "max_target": max_target,
                    "mean_target": mean_target,
                }
            )

    OUTPUT.parent.mkdir(parents=True, exist_ok=True)
    pd.DataFrame(rows, columns=SUMMARY_COLUMNS).to_csv(OUTPUT, index=False)
    print(f"Validation comparison summary written to {OUTPUT}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
