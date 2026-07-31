# ICPC Roadmap

Codebook coverage. `[x]` done, `[~]` partial, `[ ]` planned. Within a category
items run easy → hard, matching the `NN-` order in `src/`, so the list is also a
study order. Pure-STL basics are assumed and omitted.

---

## Next additions

- [ ] **Combinatorics kit**: inclusion–exclusion, Catalan, Lucas
- [ ] **Gaussian elimination** (linear systems + XOR basis)
- [ ] **Miller–Rabin + Pollard's rho** (factor big `n`)
- [ ] **Closest pair of points** (line sweep) — the main geometry gap
- [ ] **Monotonic stack / queue** (sliding-window minimum, histogram)
- [ ] **Matrix-chain DP** (the classic interval DP still missing)
- [ ] **FFT / NTT** (polynomial / big-number multiply, convolutions)
- [ ] **HLD** and **centroid decomposition** (tree path / distance queries)
- [ ] **Suffix automaton** (substring counting, longest common substring)
- [ ] **Hungarian algorithm** (assignment / min-cost perfect matching)

---

## Coverage by category

### 1. Data Structures
- [x] DSU (union by size + path compression)
- [x] Sparse table (idempotent RMQ)
- [x] Fenwick / BIT
- [x] Difference array, 2D prefix sums
- [x] Segment tree — iterative, recursive (mergeable node), lazy propagation
- [x] Sqrt decomposition + MO's algorithm (offline range queries)
- [ ] Monotonic stack / queue
- [ ] 2D BIT / 2D segment tree
- [ ] Order-statistics tree (policy tree) / treap (split-merge, order statistics)
- [ ] Merge-sort segment tree (offline range rank / k-th)
- [ ] Persistent segment tree (k-th on a path, historic queries)
- [ ] Li Chao tree, segment tree beats
- [ ] Sqrt tree, wavelet tree

### 2. Strings
- [x] KMP (prefix function)
- [x] Z-function
- [x] Polynomial hashing (double mod) + longest common extension (LCE)
- [x] Trie
- [x] Manacher (all palindromic radii)
- [x] Aho-Corasick (multi-pattern matching)
- [x] Suffix array + LCP — O(n log²n) and O(n log n) radix builds
- [ ] Suffix automaton
- [ ] Lyndon decomposition (Duval), minimal rotation
- [ ] Palindromic tree (eertree)

### 3. Graph
- [x] BFS, DFS, 0-1 BFS, multi-source BFS
- [x] Kahn topological sort, bipartite check, directed-cycle detection
- [x] Dijkstra, Bellman-Ford (+ k-edges, + negative-cycle), Floyd-Warshall
- [x] Min-bottleneck (min-effort) grid
- [x] Prim, Kruskal (MST)
- [x] Tree diameter, Euler tour / tree flattening
- [x] LCA (binary lifting)
- [x] Tarjan SCC, bridges & articulation points
- [x] Euler path / circuit (Hierholzer)
- [x] 2-SAT (implication graph + SCC)
- [ ] Functional-graph cycle finding (tortoise–hare / rho)
- [ ] Biconnected components, block-cut tree, bridge tree
- [ ] Shortest-path counting, min-mean cycle
- [ ] Small-to-large on trees (DSU on tree / sack)
- [ ] Heavy-light decomposition (path queries)
- [ ] Centroid decomposition (distance queries)
- [ ] Stable marriage (Gale–Shapley)

### 4. Problem Solving Paradigms
- [x] Binary search on answer (integer + real), ternary search
- [x] Meet in the middle
- [x] 0/1, by-value, 2D knapsack; coin-change counting
- [x] LCS, edit distance, LIS (O(n log n))
- [x] Interval DP (slimes), minimax interval
- [x] Bitmask DP (assignment matching)
- [x] Tree DP, digit DP
- [x] Game theory: win/lose states, Nim, Grundy
- [x] Probability (distribution) DP, expected-value DP (self-loop algebra)
- [ ] Backtracking with pruning
- [ ] Matrix-chain multiplication (classical interval DP)
- [ ] Subset-sum-over-subsets (SOS) DP
- [ ] Memory-reduction (rolling) + Hirschberg (LCS in O(n) memory)
- [ ] DP optimizations: convex hull trick, divide-and-conquer, Knuth
- [ ] Parallel binary search

### 5. Math
- [x] gcd, lcm, binary (fast) exponentiation
- [x] Modular exponentiation, modular inverse
- [x] Sieve, linear sieve (smallest prime factor), trial-division factorize
- [x] Euler phi
- [x] Extended gcd, CRT
- [x] Pascal nCr, modular nCr (factorials + inverse factorials)
- [x] Matrix multiply / exponentiation (linear-recurrence & path counting)
- [ ] Fast Fibonacci (fast doubling)
- [ ] Inclusion–exclusion, Catalan numbers
- [ ] Lucas' theorem, factorial mod p
- [ ] Möbius / multiplicative functions
- [ ] Burnside / Pólya counting
- [ ] Gaussian elimination, determinant, XOR (linear) basis
- [ ] Miller–Rabin + Pollard's rho (deterministic 64-bit)
- [ ] Discrete log (baby-step giant-step), primitive roots
- [ ] Lagrange interpolation
- [ ] FFT / NTT (convolutions, polynomial & big-number multiply)

### 6. Geometry
- [x] Point primitives (cross, dot, orientation)
- [x] Segment intersection
- [x] Polygon area (shoelace), point in polygon
- [x] Convex hull (Andrew's monotone chain)
- [ ] Line / circle intersection, circle construction
- [ ] Closest pair of points (line sweep)
- [ ] Rotating calipers (diameter, width, closest/farthest pair on hull)
- [ ] Pick's theorem
- [ ] Half-plane intersection
- [ ] Line sweep / radial sweep as reusable patterns
- [ ] Polygon triangulation
- [ ] Area of union / intersection of circles

### 7. Combinatorial Optimization (flow & matching)
- [x] Dinic max-flow
- [x] Kuhn bipartite matching
- [x] Min-cost max-flow (SPFA successive shortest paths)
- [x] Hopcroft–Karp (fast bipartite matching)
- [ ] Hungarian algorithm (assignment / min-cost perfect matching)
- [ ] Difference constraints (shortest paths ↔ inequalities)
- [ ] Flows with lower bounds / demands
- [ ] Global min-cut (Stoer–Wagner), all-pairs min-cut (Gomory–Hu)
- [ ] General matching (Blossom) — reference only

### 8. Misc
- [x] Kadane (max subarray)
- [x] Sliding window / two pointers
- [x] Coordinate compression
- [x] Inversion count
- [x] Bit tricks (lowest bit, gray code, subset / submask enumeration)
- [x] Randomness kit (mt19937_64: seeding, unbiased range, shuffle)
- [ ] Custom sorts (counting, radix) + when to beat `std::sort`
- [ ] Kadane in 2D (max submatrix)
- [ ] Newton / secant root finding
- [ ] DP-table path reconstruction as a reusable pattern
- [ ] Amortized-analysis intuition under contest constraints

---

## Out of scope

Dropped as too theoretical or rarely contest-decisive: link-cut / fibonacci-heap /
skip-list / kd trees, dynamic connectivity, wavelet trees, suffix/PATRICIA tree;
dominator tree, directed MST, LexBFS; Voronoi/Delaunay, sweep-line geometry,
3D hull; Kirchhoff/Prüfer, Bell/Stirling tables, Lagrange multipliers;
Tonelli–Shanks, Pohlig–Hellman, Pell, wheel factorization; Blossom-W, flow
circulation, Markov chains.

---

## Skills to drill (not codebook entries)
- Greedy exchange-argument proofs; backtracking with pruning
- Amortized / complexity analysis under contest constraints
- Reading a problem → picking the paradigm fast (the real bottleneck)
