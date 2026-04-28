#!/usr/bin/env python3
import csv
from pathlib import Path


INPUT = Path("data/output/mini_case_modes.csv")
OUTPUT = Path("out/mini_case_modes.svg")


def main() -> None:
    rows = []
    with INPUT.open(newline="") as handle:
        for row in csv.DictReader(handle):
            if row["status"] == "propagating":
                rows.append((int(row["mode"]), float(row["beta"])))

    OUTPUT.parent.mkdir(parents=True, exist_ok=True)
    width, height = 640, 360
    margin = 48
    max_beta = max((beta for _, beta in rows), default=1.0)
    x_step = 0 if len(rows) <= 1 else (width - 2 * margin) / (len(rows) - 1)

    points = []
    for index, (_, beta) in enumerate(rows):
        x = margin + index * x_step
        y = height - margin - (beta / max_beta) * (height - 2 * margin)
        points.append((x, y))

    polyline = " ".join(f"{x:.2f},{y:.2f}" for x, y in points)
    circles = "\n".join(
        f'<circle cx="{x:.2f}" cy="{y:.2f}" r="4" fill="#1f6feb" />'
        for x, y in points
    )

    OUTPUT.write_text(
        f'''<svg xmlns="http://www.w3.org/2000/svg" width="{width}" height="{height}" viewBox="0 0 {width} {height}">
  <rect width="100%" height="100%" fill="white"/>
  <line x1="{margin}" y1="{height-margin}" x2="{width-margin}" y2="{height-margin}" stroke="#222"/>
  <line x1="{margin}" y1="{margin}" x2="{margin}" y2="{height-margin}" stroke="#222"/>
  <text x="{width/2}" y="28" text-anchor="middle" font-family="sans-serif" font-size="16">Mini case beta modes</text>
  <text x="{width/2}" y="{height-10}" text-anchor="middle" font-family="sans-serif" font-size="12">mode</text>
  <text x="16" y="{height/2}" text-anchor="middle" transform="rotate(-90 16 {height/2})" font-family="sans-serif" font-size="12">beta</text>
  <polyline points="{polyline}" fill="none" stroke="#1f6feb" stroke-width="2"/>
  {circles}
</svg>
''',
        encoding="utf-8",
    )


if __name__ == "__main__":
    main()
