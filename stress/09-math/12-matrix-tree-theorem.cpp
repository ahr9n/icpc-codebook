// Compare Kirchhoff's determinant with explicit enumeration of every (n-1)-edge
// subset on random small multigraphs, including loops and parallel edges.
int stress() {
    mt19937 rng(912);
    long long checks = 0;

    for (int rep = 0; rep < 5000; rep++) {
        int n = 1 + (int)(rng() % 7);
        int m = rng() % 11;
        vector<pair<int, int>> edges(m);
        for (auto& [u, v]: edges) {
            u = rng() % n;
            v = rng() % n;
        }

        long long expected = 0;
        for (int mask = 0; mask < (1 << m); mask++) {
            if (__builtin_popcount((unsigned)mask) != n - 1) {
                continue;
            }
            vector<int> parent(n);
            iota(parent.begin(), parent.end(), 0);
            auto find = [&](int vertex) {
                int root = vertex;
                while (parent[root] != root) {
                    root = parent[root];
                }
                while (parent[vertex] != vertex) {
                    int next = parent[vertex];
                    parent[vertex] = root;
                    vertex = next;
                }
                return root;
            };

            bool acyclic = true;
            for (int id = 0; id < m; id++) {
                if (not(mask & (1 << id))) {
                    continue;
                }
                int u = find(edges[id].first);
                int v = find(edges[id].second);
                if (u == v) {
                    acyclic = false;
                    break;
                }
                parent[u] = v;
            }
            if (acyclic) {
                expected++;
            }
        }

        long long actual = count_spanning_trees(n, edges);
        if (actual != expected % MOD) {
            printf("matrix-tree FAIL rep=%d n=%d m=%d expected=%lld actual=%lld\n", rep, n, m,
                   expected, actual);
            return 1;
        }
        checks++;
    }

    printf("matrix-tree-theorem PASS %lld", checks);
    return 0;
}
