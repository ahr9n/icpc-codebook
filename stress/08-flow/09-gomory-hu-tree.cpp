// Compare every tree-path bottleneck with exhaustive enumeration of all cuts
// separating that pair on random small undirected weighted graphs.
int stress() {
    mt19937 rng(809);
    long long checks = 0;

    const long long wide_cut = 6000000000000000000LL;
    vector<vector<long long>> wide_capacity = {{0, wide_cut}, {wide_cut, 0}};
    vector<GomoryHuEdge> wide_tree = gomory_hu_tree(wide_capacity);
    if (wide_tree.size() != 1 or wide_tree[0].cut != wide_cut) {
        printf("gomory-hu FAIL wide\n");
        return 1;
    }
    checks++;

    for (int rep = 0; rep < 3500; rep++) {
        int n = 2 + (int)(rng() % 7);
        vector<vector<long long>> capacity(n, vector<long long>(n, 0));
        for (int u = 0; u < n; u++) {
            for (int v = u + 1; v < n; v++) {
                capacity[u][v] = capacity[v][u] = rng() % 8;
            }
        }

        vector<vector<pair<int, long long>>> tree(n);
        for (GomoryHuEdge edge: gomory_hu_tree(capacity)) {
            tree[edge.u].push_back({edge.v, edge.cut});
            tree[edge.v].push_back({edge.u, edge.cut});
        }

        for (int source = 0; source < n; source++) {
            vector<long long> path_cut(n, -1);
            path_cut[source] = LINF;
            queue<int> q;
            q.push(source);
            while (not q.empty()) {
                int u = q.front();
                q.pop();
                for (auto [v, weight]: tree[u]) {
                    if (path_cut[v] != -1) {
                        continue;
                    }
                    path_cut[v] = min(path_cut[u], weight);
                    q.push(v);
                }
            }

            for (int sink = source + 1; sink < n; sink++) {
                long long expected = LINF;
                for (int mask = 0; mask < (1 << n); mask++) {
                    if (((mask >> source) & 1) == ((mask >> sink) & 1)) {
                        continue;
                    }
                    long long cut = 0;
                    for (int u = 0; u < n; u++) {
                        for (int v = u + 1; v < n; v++) {
                            if (((mask >> u) & 1) != ((mask >> v) & 1)) {
                                cut += capacity[u][v];
                            }
                        }
                    }
                    expected = min(expected, cut);
                }

                if (path_cut[sink] != expected) {
                    printf("gomory-hu FAIL rep=%d source=%d sink=%d expected=%lld actual=%lld\n",
                           rep, source, sink, expected, path_cut[sink]);
                    return 1;
                }
                checks++;
            }
        }
    }

    printf("gomory-hu-tree PASS %lld", checks);
    return 0;
}
