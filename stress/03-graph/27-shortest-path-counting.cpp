// Stress: shortest-path distances vs Bellman-Ford, and path counts vs a direct
// DFS enumeration of every shortest path. Positive weights keep shortest paths
// simple, so the DFS terminates and each distinct edge sequence is one path.
int stress() {
    mt19937 rng(27);
    long long checks = 0;

    for (int rep = 0; rep < 3000; rep++) {
        n = 2 + rng() % 5;
        adj.assign(n, {});
        vector<array<long long, 3>> es;
        int m = rng() % (2 * n + 1);
        for (int i = 0; i < m; i++) {
            int u = rng() % n, v = rng() % n;
            long long w = 1 + rng() % 3;
            adj[u].push_back({u, v, w});
            es.push_back({u, v, w});
        }

        count_shortest_paths(0);

        vector<long long> bd(n, LINF);
        bd[0] = 0;
        for (int it = 0; it < n; it++)
            for (auto& e: es)
                if (bd[e[0]] < LINF and bd[e[0]] + e[2] < bd[e[1]]) bd[e[1]] = bd[e[0]] + e[2];

        for (int v = 0; v < n; v++)
            if (bd[v] != dist[v]) {
                printf("spc FAIL(dist) rep=%d v=%d\n", rep, v);
                return 1;
            }

        vector<long long> bw(n, 0);
        function<void(int, long long)> go = [&](int u, long long cost) {
            if (cost > bd[u]) return;
            bw[u] = (bw[u] + 1) % MOD;
            for (auto& e: adj[u]) go(e.to, cost + e.cost);
        };
        go(0, 0);

        for (int v = 0; v < n; v++)
            if (bw[v] != ways[v]) {
                printf("spc FAIL(ways) rep=%d v=%d\n", rep, v);
                return 1;
            }

        checks++;
    }

    printf("shortest-path-counting PASS %lld", checks);
    return 0;
}
