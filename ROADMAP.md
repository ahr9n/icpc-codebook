# ICPC Roadmap — remaining work

This file tracks only what is **not yet in the codebook**. Everything already
implemented is catalogued in `README.md` (the "what we have"); this file is the
"what's next." Items are grouped by ROI — contest frequency and teaching value
divided by implementation + verification effort — so the list doubles as an
implementation order. Pure-STL basics and non-contest material are omitted (see
the bottom).

Gaps were reconciled against Mostafa Saad's full syllabus; anything of his we
deliberately skip is listed under "Out of scope" rather than left implicit.

---

## Tier 1 — high ROI (short, common, easy to brute-force)

- [ ] Fast Fibonacci (fast doubling) — Math
- [ ] Lyndon decomposition (Duval) + minimal rotation — Strings
- [ ] Functional-graph cycle finding (Floyd / Brent tortoise–hare) — Graph
- [ ] Subset-sum-over-subsets (SOS) DP — Paradigm
- [ ] Pick's theorem — Geometry
- [ ] Kadane in 2D (max submatrix) — Misc
- [ ] Custom sorts (counting, radix) + when to beat `std::sort` — Misc
- [ ] Tonelli–Shanks (modular square root) — Math

## Tier 2 — medium-high ROI

- [ ] Palindromic tree (eertree) — Strings
- [ ] Stable marriage (Gale–Shapley) — Graph
- [ ] Rotating calipers (diameter, width, farthest pair) — Geometry
- [ ] Half-plane intersection — Geometry
- [ ] Point-in-convex-polygon in O(log n) — Geometry
- [ ] Lagrange interpolation — Math
- [ ] Discrete log (baby-step giant-step) + primitive roots — Math
- [ ] Burnside / Pólya counting — Math
- [ ] XOR / AND / OR convolution (Walsh–Hadamard, SOS transform) — Math
- [ ] Matrix-tree theorem (Kirchhoff, spanning-tree count) — Math/Graph
- [ ] Parallel binary search — Paradigm
- [ ] Flows with lower bounds / demands (circulation) — Flow
- [ ] Newton / secant root finding — Misc

## Tier 3 — solid but heavier

- [ ] Global min-cut (Stoer–Wagner) — Flow
- [ ] All-pairs min-cut (Gomory–Hu tree) — Flow
- [ ] Directed MST / arborescence (Chu–Liu–Edmonds) — Graph
- [ ] Polygon triangulation — Geometry
- [ ] Area of union / intersection of circles — Geometry
- [ ] Stirling numbers (1st/2nd), Bell numbers, Prüfer sequence — Math
- [ ] Line sweep / radial sweep as reusable patterns — Geometry
- [ ] 3D geometry primitives (dot/cross, plane, hull) — Geometry
- [ ] DP-table path reconstruction as a reusable pattern — Paradigm

## Tier 4 — reference-only / advanced (listed, not scheduled)

- [ ] Link-cut tree, Euler-tour tree, dynamic connectivity — DS
- [ ] Dominator tree — Graph
- [ ] Suffix tree (build from suffix array + LCP), splay, kd-tree — DS/Strings
- [ ] General matching (Blossom), Simplex — Flow
- [ ] Voronoi (Fortune), Bentley–Ottmann, 3D convex hull — Geometry
- [ ] Discrete-log extras: Pohlig–Hellman, Pollard kangaroo — Math
- [ ] Number-theory extras: Pell, Cornacchia, Cipolla, wheel factorization — Math

---

## Out of scope (deliberately skipped)

Pure-STL containers (array, stack, queue, deque, vector, list, priority_queue),
`std::string`/`cstring`/`ctype`/`complex` usage, roman numerals, postfix
calculator, towers of Hanoi, insertion/selection sort, regular expressions, OOP,
Babbage difference engine, quaternions, Method of Four Russians, Farey sequence,
Bernoulli numbers. These are either assumed known, trivial, or effectively never
decisive in a contest.
