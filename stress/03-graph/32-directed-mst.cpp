// Compare Chu–Liu–Edmonds with enumeration of one incoming edge per non-root
// vertex on random small directed graphs with negative and parallel edges.
int stress() {
    mt19937 rng(332);
    long long checks = 0;

    const long long wide_cost = 6000000000000000000LL;
    auto [wide_exists, wide_actual] = directed_mst(2, 0, {{0, 1, wide_cost}});
    if (not wide_exists or wide_actual != wide_cost) {
        printf("directed-mst FAIL wide expected=%lld actual=%d/%lld\n", wide_cost, wide_exists,
               wide_actual);
        return 1;
    }
    checks++;

    auto [max_exists, max_actual] = directed_mst(2, 0, {{0, 1, LLONG_MAX}});
    if (not max_exists or max_actual != LLONG_MAX) {
        printf("directed-mst FAIL max-edge\n");
        return 1;
    }
    checks++;

    for (int rep = 0; rep < 5000; rep++) {
        int n = 1 + (int)(rng() % 6);
        int root = rng() % n;
        vector<DirectedEdge> edges;
        for (int from = 0; from < n; from++) {
            for (int to = 0; to < n; to++) {
                if (from != to and rng() % 100 < 45) {
                    edges.push_back({from, to, (long long)(int)(rng() % 16) - 5});
                    if (rng() % 5 == 0) {
                        edges.push_back({from, to, (long long)(int)(rng() % 16) - 5});
                    }
                }
            }
        }

        vector<vector<int>> incoming(n);
        for (int id = 0; id < (int)edges.size(); id++) {
            incoming[edges[id].to].push_back(id);
        }

        bool brute_exists = false;
        long long brute_cost = LINF;
        vector<int> parent(n, -1);
        function<void(int, long long)> enumerate = [&](int vertex, long long cost) {
            if (vertex == n) {
                bool valid = true;
                for (int start = 0; start < n; start++) {
                    int u = start;
                    vector<bool> seen(n, false);
                    while (u != root and u != -1 and not seen[u]) {
                        seen[u] = true;
                        u = parent[u];
                    }
                    if (u != root) {
                        valid = false;
                        break;
                    }
                }
                if (valid) {
                    brute_exists = true;
                    brute_cost = min(brute_cost, cost);
                }
                return;
            }
            if (vertex == root) {
                enumerate(vertex + 1, cost);
                return;
            }
            for (int id: incoming[vertex]) {
                parent[vertex] = edges[id].from;
                enumerate(vertex + 1, cost + edges[id].cost);
            }
            parent[vertex] = -1;
        };
        enumerate(0, 0);

        auto [actual_exists, actual_cost] = directed_mst(n, root, edges);
        if (actual_exists != brute_exists or (actual_exists and actual_cost != brute_cost)) {
            printf("directed-mst FAIL rep=%d n=%d root=%d expected=%d/%lld actual=%d/%lld\n", rep,
                   n, root, brute_exists, brute_cost, actual_exists, actual_cost);
            return 1;
        }
        checks++;
    }

    printf("directed-mst PASS %lld", checks);
    return 0;
}
