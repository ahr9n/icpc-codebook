// Stress: Karp's min mean cycle vs brute enumeration of every simple cycle (min
// of sum/len). The optimum is always a simple cycle, so enumeration is a valid
// oracle. Random directed graphs with self-loops, parallel edges, and negative
// weights.
int stress() {
    mt19937 rng(28);
    long long checks = 0;

    for (int rep = 0; rep < 4000; rep++) {
        n = 1 + rng() % 6;
        edges.clear();
        vector<vector<pair<int, long long>>> g(n);
        int m = rng() % (2 * n + 2);
        for (int i = 0; i < m; i++) {
            int u = rng() % n, v = rng() % n;
            long long w = (long long)(rng() % 11) - 5;
            edges.push_back({u, v, w});
            g[u].push_back({v, w});
        }

        long double mean;
        bool has = min_mean_cycle(mean);

        bool brute_has = false;
        long double best = 0;
        vector<char> vis(n, 0);
        function<void(int, int, long long, int)> dfs =
            [&](int s, int u, long long sum, int len) {
                for (auto [to, w]: g[u]) {
                    if (to == s) {
                        long double mm = (long double)(sum + w) / (len + 1);
                        if (not brute_has or mm < best) {
                            best = mm;
                            brute_has = true;
                        }
                    } else if (to > s and not vis[to]) {
                        vis[to] = 1;
                        dfs(s, to, sum + w, len + 1);
                        vis[to] = 0;
                    }
                }
            };
        for (int s = 0; s < n; s++) {
            vis[s] = 1;
            dfs(s, s, 0, 0);
            vis[s] = 0;
        }

        if (has != brute_has) {
            printf("mmc FAIL(exists) rep=%d\n", rep);
            return 1;
        }
        if (brute_has and fabsl(mean - best) > 1e-9) {
            printf("mmc FAIL(mean) rep=%d got=%.9Lf want=%.9Lf\n", rep, mean, best);
            return 1;
        }

        checks++;
    }

    printf("min-mean-cycle PASS %lld", checks);
    return 0;
}
