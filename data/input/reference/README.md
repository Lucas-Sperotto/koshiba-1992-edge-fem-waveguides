# Reference curves

Place numeric reference CSVs for validation here. Images from the paper are not
versioned in this repository.

Expected CSV header:

```text
case_id,curve_id,x,y,method,units
```

`scripts/plot/compare_validation.py` interpolates each reference curve on the
solver x-grid and writes `out/validation/validation_summary.csv`.
