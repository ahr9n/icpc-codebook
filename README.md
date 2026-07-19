# Competitive Programming Codebook

My personal competitive-programming codebook — clean, printable C++ implementations
of the algorithms and data structures I reach for in contests.

The document is built from `src/`: each subdirectory is a section and each `.cpp`
file is a subsection, assembled into a landscape two-column PDF via LaTeX.

## Layout

```
src/
  01-template/            contest template + is_valid grid helper
  02-data-structures/     DSU, segment trees (iterative/recursive/lazy),
                          difference array, 2D prefix sums, MO's algorithm
  03-graph/               shortest paths, MST, SCC, bridges, LCA, cycles, ...
  04-dynamic-programming/ knapsack, LCS/LIS, interval, bitmask, tree, digit, games
  05-strings/             KMP, Z, hashing, trie
  06-number-theory/       modpow, inverse, sieves, factorize, phi, ext-gcd, CRT, nCr
  07-geometry/            point primitives, orientation, polygons, convex hull
  08-flow/                Dinic max-flow, Kuhn bipartite matching
  09-math/                matrix exponentiation, big integer arithmetic
  10-misc/                Kadane, sliding window, binary/ternary search, MITM
```

Files are numbered to control section and subsection order. Each listing is
self-contained: it declares at the top only the globals it uses (assuming the
contest template's includes and constants are in scope), so any single snippet
can be copied on its own. Worked `main()` examples are embedded in the algorithm
file they demonstrate.

## Build

Requires `python3` and a LaTeX toolchain with `pdflatex`.

```
# macOS: brew install --cask mactex   (or basictex for a smaller install)
./build.sh          # Linux/macOS -> codebook.pdf
build.bat           # Windows
```

`build.sh` regenerates `contents.tex` from `src/` (via `build-contents.py`), then
runs `pdflatex` twice so the table of contents resolves.
