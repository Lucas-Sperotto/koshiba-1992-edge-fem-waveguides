#!/usr/bin/env python3
"""Generate project-owned validation plots from CSV outputs."""

from __future__ import annotations

from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd


ROOT = Path(__file__).resolve().parents[2]
INPUT_DIR = ROOT / "data" / "output" / "validation"
OUTPUT_DIR = ROOT / "out" / "validation"


AXIS_LABELS = {
    "beta_rad_per_m": r"$\beta$ (rad/m)",
    "beta_rad_per_mm": r"$\beta$ (rad/mm)",
    "frequency_ghz": "frequency (GHz)",
    "v_number": r"$v$",
    "b_number": r"$b$",
}

VALID_STATUSES = {"propagating"}


def label_for(quantity: str) -> str:
    return AXIS_LABELS.get(quantity, quantity.replace("_", " "))


def finite_numeric(series: pd.Series) -> pd.Series:
    return pd.to_numeric(series, errors="coerce")


def plot_case(csv_path: Path, output_dir: Path) -> None:
    data = pd.read_csv(csv_path)
    if data.empty:
        return

    case_id = str(data["case_id"].iloc[0])
    x_quantity = str(data["x_quantity"].iloc[0])
    y_quantity = str(data["y_quantity"].iloc[0])
    data = data.copy()
    data["x"] = finite_numeric(data["x"])
    data["y"] = finite_numeric(data["y"])
    data = data[data["status"].isin(VALID_STATUSES)]

    fig, ax = plt.subplots(figsize=(7.0, 4.6), constrained_layout=True)
    plotted = False
    for curve_id, curve in data.groupby("curve_id", sort=False):
        curve = curve.sort_values("x")
        finite = curve[np.isfinite(curve["x"]) & np.isfinite(curve["y"])]
        if finite.empty:
            continue
        ax.plot(
            finite["x"],
            finite["y"],
            marker="o",
            linewidth=1.5,
            markersize=3.5,
            label=str(curve_id),
        )
        plotted = True

    ax.set_title(case_id)
    ax.set_xlabel(label_for(x_quantity))
    ax.set_ylabel(label_for(y_quantity))
    ax.grid(True, alpha=0.28)
    if plotted:
        ax.legend()
    else:
        ax.text(
            0.5,
            0.5,
            "no finite points",
            ha="center",
            va="center",
            transform=ax.transAxes,
        )

    output_dir.mkdir(parents=True, exist_ok=True)
    fig.savefig(output_dir / f"{case_id}.svg")
    plt.close(fig)


def main() -> int:
    csv_files = sorted(INPUT_DIR.glob("*.csv"))
    if not csv_files:
        raise SystemExit("plot_validation: no validation CSVs found")

    for csv_path in csv_files:
        plot_case(csv_path, OUTPUT_DIR)

    print(f"Validation plots written to {OUTPUT_DIR}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
