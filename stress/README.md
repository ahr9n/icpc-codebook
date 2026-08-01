# Stress tests

Regression harnesses validating each `src/` listing against a brute-force
reference on thousands of random inputs. These are **not** part of the printed
codebook — `build-contents.py` only scans `src/`, so this directory never
reaches the PDF.

## Layout

Mirrors `src/`: `stress/<section>/<file>.cpp` tests `src/<section>/<file>.cpp`.
Each harness contains only test logic — an `int stress()` returning nonzero on
failure — and assumes the algorithm's symbols and the contest template are in
scope. `run.sh` assembles harness + source (demo `main()` stripped) + template
shim and runs it, so a harness always exercises the current source.

## Run

```sh
stress/run.sh            # all
stress/run.sh sqrt-tree  # only harnesses whose path contains "sqrt-tree"
```

Exits nonzero if any harness fails to compile or reports a mismatch.

The GNU-only policy-based order-statistics tree is compiled with Homebrew
`g++-15` when available and reported as skipped on toolchains without GNU
`pb_ds`; every other listing uses `clang++` through the repository shim.
