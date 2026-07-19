# ICPC Mastery Roadmap

Coverage of the codebook against the full CP topic map, plus prioritized next
steps. `[x]` = implemented in `src/`, `[~]` = partially covered, `[ ]` = not yet.
Pure-STL basics (stack, queue, vector, sort, ...) are assumed known and omitted.

---

## Recommended next additions (highest ROI first)

### Phase 1 — common, medium effort
- [ ] **2-SAT** (implication graph + SCC — you already have Tarjan)
- [ ] **Suffix array + LCP** (Kasai)
- [ ] **Min-cost max-flow** (SPFA/Dijkstra potentials)
- [ ] **Closest pair of points** (line sweep) — the one geometry gap
- [ ] **Combinatorics kit**: inclusion–exclusion, Catalan, Lucas' theorem
- [ ] **Gaussian elimination** (linear systems, XOR basis)
- [ ] **Miller–Rabin + Pollard's rho** (fast factorization of big n)
- [ ] **Matrix-chain DP** (classical interval DP you don't have yet)

### Phase 2 — advanced, higher effort
- [ ] **Heavy-light decomposition** (path queries)
- [ ] **Centroid decomposition**, **DSU on tree (small-to-large)**
- [ ] **Aho-Corasick**, **Manacher**, **suffix automaton**
- [ ] **DP optimizations**: convex hull trick, divide-and-conquer, Knuth
- [ ] **Geometry**: rotating calipers, Pick's theorem, polygon triangulation, half-plane
- [ ] **Discrete log (BSGS)**, **primitive roots**, **NTT/FFT**

---

## Coverage by category

### Data Structures
- [x] DSU (union by size + path compression)
- [x] Segment tree — iterative, recursive (mergeable node), lazy propagation
- [x] Difference array, 2D prefix sums
- [x] MO's algorithm (sqrt decomposition, offline)
- [x] Fenwick / BIT
- [x] Sparse table (idempotent RMQ)
- [ ] 2D BIT
- [ ] Monotonic stack / queue
- [ ] Persistent / merge-sort segment tree, segment tree beats, Li Chao
- [ ] Balanced BST internals (treap/splay) — use `std::set` in practice

### Strings
- [x] KMP (prefix function), Z-function, polynomial hashing, Trie
- [ ] Aho-Corasick, Manacher, suffix array + LCP
- [ ] Suffix automaton / suffix tree, Lyndon (Duval)

### Graph
- [x] BFS, DFS, 0-1 BFS, multi-source BFS
- [x] Dijkstra, Bellman-Ford (+ k-edges, + negative cycle), Floyd-Warshall
- [x] Prim, Kruskal (MST)
- [x] Kahn topological sort, bipartite check, directed-cycle detection
- [x] Tarjan SCC, bridges & articulation points, LCA (binary lifting), tree diameter
- [x] Min-bottleneck (min-effort) grid
- [x] Euler tour / tree flattening
- [ ] Euler circuit/path, 2-SAT
- [ ] Biconnected components / block-cut & bridge trees
- [ ] HLD, centroid decomposition, DSU on tree, dominator tree
- [ ] Shortest-path counting, min-mean cycle, stable marriage

### DP & Paradigms
- [x] Knapsack (0/1, by value, 2D), coin-change counting
- [x] LCS, LIS (n log n), edit distance, interval (slimes)
- [x] Bitmask matching, tree DP, digit DP
- [x] Game theory: win/lose states, Nim, Grundy, minimax interval
- [x] Binary search on answer (int + real), ternary search, meet in the middle
- [ ] Matrix-chain, DP with memory reduction, cyclic/self-referential DP
- [ ] DP optimizations (CHT, D&C, Knuth), backtracking with pruning

### Number Theory & Math
- [x] Modular exponentiation, modular inverse, gcd, lcm
- [x] Sieve, linear sieve (SPF), trial-division factorize, Euler phi
- [x] Extended gcd, CRT, Pascal nCr, modular nCr
- [x] Matrix exponentiation
- [ ] Inclusion–exclusion, Catalan, Lucas, Stirling, Möbius, Burnside
- [ ] Gaussian elimination / determinant, Miller-Rabin + Pollard rho
- [ ] Discrete log (BSGS), Lagrange interpolation, FFT/NTT

### Geometry
- [x] Point primitives (cross, dot, orientation)
- [x] Segment intersection, polygon area (shoelace), point in polygon
- [x] Convex hull (Andrew's monotone chain)
- [ ] Closest pair (line sweep), rotating calipers, Pick's theorem
- [ ] Line/circle intersection, half-plane intersection, polygon triangulation
- [ ] Voronoi / Delaunay, 3D geometry

### Flow & Matching
- [x] Dinic max-flow, Kuhn bipartite matching
- [ ] Min-cost max-flow, Hopcroft-Karp, Hungarian
- [ ] Flows with lower bounds / demands, Gomory-Hu, global min-cut (Stoer-Wagner)

### Misc
- [x] Kadane, sliding window / two pointers, coordinate compression, inversion count
- [ ] DP-table path reconstruction as a reusable pattern
- [ ] Amortized-analysis intuition, custom sorts (counting/radix)

---

## Skills to drill (not codebook entries)
- Greedy exchange-argument proofs; backtracking with bounding/pruning
- Amortized & complexity analysis under contest constraints
- Reading a problem → picking the paradigm fast (the real bottleneck)
