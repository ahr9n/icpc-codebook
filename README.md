# Competitive Programming Codebook

My personal competitive-programming codebook — clean, printable C++ implementations
of the algorithms and data structures I reach for in contests.

The document is built from `src/`: each subdirectory is a section and each `.cpp`
file is a subsection, assembled into a landscape two-column PDF via LaTeX.

## Layout

```
src/
  01-template/            contest template (includes, consts, fast IO, main)
  02-data-structures/     DSU, segment trees (iter/recursive/lazy), Fenwick/BIT,
                          sparse table, difference array, 2D prefix sums, MO's
  03-graph/               BFS/DFS/0-1-BFS, shortest paths, MST, topo, SCC, bridges,
                          LCA, tree diameter, cycles, Euler tour, 2-SAT,
                          Euler circuit/path (Hierholzer)
  04-dynamic-programming/ knapsack, LCS/LIS, interval, bitmask, tree, digit, games
                          (each with both iterative and recursive versions)
  05-strings/             KMP, Z, hashing, trie, Manacher, suffix array + LCP
                          (O(n log²n) + O(n log n) radix), LCE, Aho-Corasick
  06-number-theory/       modpow, inverse, sieves, factorize, phi, ext-gcd, CRT, nCr
  07-geometry/            point primitives, orientation, polygons, convex hull
  08-flow/                Dinic max-flow, Kuhn + Hopcroft-Karp bipartite matching,
                          min-cost max-flow (SPFA)
  09-math/                matrix exponentiation, big integer arithmetic
  10-bitmask/             bit ops, lowest-bit, gray code, subset/submask enumeration
  11-probability/         probability (distribution) DP, expected-value DP
                          (law of total expectation, self-loop algebra)
  12-misc/                Kadane, sliding window, binary/ternary search, MITM,
                          coordinate compression, inversion count, randomness (mt19937_64)
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
