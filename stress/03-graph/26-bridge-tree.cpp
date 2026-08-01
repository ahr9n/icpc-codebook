// Stress: bridge tree vs brute bridges (an edge is a bridge iff deleting that one
// instance raises the connected-component count), the 2-edge-connected component
// partition, and the tree edge count (= number of bridges). Random small
// undirected graphs with parallel edges.
int stress() {
    mt19937 rng(26);
    long long checks = 0;

    for (int rep = 0; rep < 4000; rep++) {
        int n = 1 + rng() % 8;
        int m = rng() % (2 * n + 2);

        BridgeTree bt(n);
        vector<pair<int, int>> es;
        for (int i = 0; i < m; i++) {
            int u = rng() % n, v = rng() % n;
            if (u == v) continue;
            bt.add_edge(u, v);
            es.push_back({u, v});
        }
        bt.build();

        auto comp_count = [&](int skip_id) {
            vector<int> par(n);
            iota(par.begin(), par.end(), 0);
            auto find = [&](int x) {
                while (par[x] != x) x = par[x] = par[par[x]];
                return x;
            };
            for (int id = 0; id < (int)es.size(); id++) {
                if (id == skip_id) continue;
                par[find(es[id].first)] = find(es[id].second);
            }
            int c = 0;
            for (int v = 0; v < n; v++)
                if (find(v) == v) c++;
            return c;
        };

        int base = comp_count(-1);
        int brute_bridges = 0;
        for (int id = 0; id < (int)es.size(); id++) {
            bool brute_bridge = comp_count(id) > base;
            if (brute_bridge) brute_bridges++;
            if (brute_bridge != (bool)bt.is_bridge[id]) {
                printf("bridge FAIL(bridge) rep=%d id=%d\n", rep, id);
                return 1;
            }
        }

        // 2ecc from the non-bridge subgraph, compared to comp[] as a partition.
        vector<int> par(n);
        iota(par.begin(), par.end(), 0);
        auto find = [&](int x) {
            while (par[x] != x) x = par[x] = par[par[x]];
            return x;
        };
        for (int id = 0; id < (int)es.size(); id++)
            if (not bt.is_bridge[id]) par[find(es[id].first)] = find(es[id].second);
        for (int a = 0; a < n; a++)
            for (int b = 0; b < n; b++) {
                bool same_brute = find(a) == find(b);
                bool same_algo = bt.comp[a] == bt.comp[b];
                if (same_brute != same_algo) {
                    printf("bridge FAIL(2ecc) rep=%d a=%d b=%d\n", rep, a, b);
                    return 1;
                }
            }

        int tree_edges = 0;
        for (auto& adj: bt.tree) tree_edges += adj.size();
        tree_edges /= 2;
        if (tree_edges != brute_bridges) {
            printf("bridge FAIL(tree-edges) rep=%d\n", rep);
            return 1;
        }

        checks++;
    }

    printf("bridge-tree PASS %lld", checks);
    return 0;
}
