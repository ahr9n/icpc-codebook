# Competitive Programming Codebook

Clean, printable C++ implementations of the algorithms I use in contests.

Built from `src/`: each subdirectory is a section, each `.cpp` a subsection,
assembled into a landscape two-column PDF via LaTeX. Files are numbered to fix
section/subsection order. Every listing is self-contained — it declares only the
globals it uses (the template's includes and constants are assumed in scope), so
any snippet copies on its own, and carries a worked `main()` demo.

## Layout

```
src/
  01-template/            contest template (includes, consts, fast IO, main)
  02-data-structures/     DSU, segment trees (iter/recursive/lazy/beats/2D/persistent/
                          merge-sort/Li-Chao), Fenwick/BIT (1D+2D), sparse table,
                          sqrt tree, wavelet tree, treap + pb_ds order-statistics,
                          difference array, 2D prefix sums, MO's, monotonic stack/queue
  03-graph/               BFS/DFS/0-1-BFS, shortest paths (+ path counting), MST, topo,
                          SCC, bridges, biconnected/block-cut/bridge tree, LCA, tree
                          diameter, cycles (+ min-mean), Euler tour, 2-SAT, Hierholzer,
                          HLD, centroid decomp, DSU-on-tree (small-to-large)
  04-dynamic-programming/ knapsack, LCS/LIS, interval, matrix-chain, bitmask, tree,
                          digit, games, backtracking, Hirschberg (linear-memory LCS),
                          DP optimizations (convex-hull trick, divide & conquer, Knuth)
  05-strings/             KMP, Z, hashing, trie, Manacher, suffix array + LCP, LCE,
                          Aho-Corasick, suffix automaton
  06-number-theory/       modpow, inverse, sieves, factorize, phi, Möbius/multiplicative,
                          ext-gcd, CRT, nCr, Catalan, Lucas, inclusion-exclusion,
                          Miller-Rabin, Pollard's rho
  07-geometry/            point primitives, orientation, polygons, convex hull,
                          line/circle & circle/circle intersection, circle construction
                          (circumcircle + min-enclosing), closest pair (line sweep)
  08-flow/                Dinic max-flow, Kuhn + Hopcroft-Karp matching,
                          min-cost max-flow (SPFA), Hungarian assignment,
                          difference constraints
  09-math/                matrix exponentiation, big integer arithmetic,
                          Gaussian elimination, XOR (linear) basis, FFT, NTT
  10-bitmask/             bit ops, lowest-bit, gray code, subset/submask enumeration
  11-probability/         distribution DP, expected-value DP (self-loop algebra)
  12-misc/                Kadane, sliding window, binary/ternary search, MITM,
                          coordinate compression, inversion count, mt19937_64
```

## Build

Needs `python3` and a LaTeX toolchain with `pdflatex`.

```
# macOS: brew install --cask mactex   (or basictex for a smaller install)
./build.sh          # Linux/macOS -> codebook.pdf
build.bat           # Windows
```

`build.sh` regenerates `contents.tex` from `src/`, then runs `pdflatex` twice so
the table of contents resolves.
