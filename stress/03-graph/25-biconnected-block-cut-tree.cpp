// Stress: block-cut tree vs brute articulation points, plus two structural
// invariants — the blocks partition the edges, and the block-cut tree is a
// forest. Random small undirected graphs with parallel edges (no self-loops,
// per contract).
int stress() {
    mt19937 rng(25);
    long long checks = 0;

    for (int rep = 0; rep < 4000; rep++) {
        int n = 1 + rng() % 8;
        int m = rng() % (2 * n + 2);

        BlockCutTree bct(n);
        vector<pair<int, int>> es;
        for (int i = 0; i < m; i++) {
            int u = rng() % n, v = rng() % n;
            if (u == v) continue;
            bct.add_edge(u, v);
            es.push_back({u, v});
        }
        bct.run();

        auto comp_count = [&](int skip) {
            vector<int> par(n);
            iota(par.begin(), par.end(), 0);
            auto find = [&](int x) {
                while (par[x] != x) x = par[x] = par[par[x]];
                return x;
            };
            for (auto& e: es) {
                if (e.first == skip or e.second == skip) continue;
                par[find(e.first)] = find(e.second);
            }
            int c = 0;
            for (int v = 0; v < n; v++)
                if (v != skip and find(v) == v) c++;
            return c;
        };

        int base = comp_count(-1);
        for (int u = 0; u < n; u++) {
            bool brute_cut = comp_count(u) > base;
            if (brute_cut != (bool)bct.is_cut[u]) {
                printf("block-cut FAIL(cut) rep=%d u=%d\n", rep, u);
                return 1;
            }
        }

        vector<int> seen(es.size(), 0);
        for (auto& block: bct.bccs)
            for (int id: block) seen[id]++;
        for (int id = 0; id < (int)es.size(); id++)
            if (seen[id] != 1) {
                printf("block-cut FAIL(partition) rep=%d id=%d\n", rep, id);
                return 1;
            }

        int tn = bct.tree.size();
        vector<int> tp(tn);
        iota(tp.begin(), tp.end(), 0);
        auto tfind = [&](int x) {
            while (tp[x] != x) x = tp[x] = tp[tp[x]];
            return x;
        };
        for (int a = 0; a < tn; a++)
            for (int b: bct.tree[a])
                if (a < b) {
                    if (tfind(a) == tfind(b)) {
                        printf("block-cut FAIL(cycle) rep=%d\n", rep);
                        return 1;
                    }
                    tp[tfind(a)] = tfind(b);
                }

        checks++;
    }

    printf("block-cut-tree PASS %lld", checks);
    return 0;
}
