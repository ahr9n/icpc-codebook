// Stress: DSU-on-tree subtree distinct-color counts vs a naive O(n^2) recompute
// that gathers each subtree's colors into a set. Random rooted trees, small
// palettes so collisions are common.
int stress() {
    mt19937 rng(29);
    long long checks = 0;

    for (int rep = 0; rep < 4000; rep++) {
        int n = 1 + rng() % 12;
        adj.assign(n, {});
        color.assign(n, 0);
        for (int i = 0; i < n; i++) color[i] = rng() % 4;
        for (int i = 1; i < n; i++) {
            int p = rng() % i;
            adj[i].push_back(p);
            adj[p].push_back(i);
        }

        solve(0, n);

        vector<int> want(n, 0);
        function<void(int, int)> go = [&](int u, int p) {
            set<int> s;
            function<void(int, int)> col = [&](int x, int px) {
                s.insert(color[x]);
                for (int y: adj[x])
                    if (y != px) col(y, x);
            };
            col(u, p);
            want[u] = s.size();
            for (int v: adj[u])
                if (v != p) go(v, u);
        };
        go(0, -1);

        for (int u = 0; u < n; u++)
            if (want[u] != answer[u]) {
                printf("dsu-on-tree FAIL rep=%d u=%d want=%d got=%d\n", rep, u, want[u], answer[u]);
                return 1;
            }

        checks++;
    }

    printf("dsu-on-tree PASS %lld", checks);
    return 0;
}
