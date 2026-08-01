// Compare the contraction algorithm with enumeration of every non-trivial
// bipartition on random small weighted multigraphs.
int stress() {
    mt19937 rng(808);
    long long checks = 0;

    const long long wide_cut = 6000000000000000000LL;
    vector<vector<long long>> wide_weights = {{0, wide_cut}, {wide_cut, 0}};
    if (global_min_cut(wide_weights) != wide_cut) {
        printf("stoer-wagner FAIL wide\n");
        return 1;
    }
    checks++;

    for (int rep = 0; rep < 6000; rep++) {
        int n = 2 + (int)(rng() % 8);
        vector<vector<long long>> weights(n, vector<long long>(n, 0));
        for (int u = 0; u < n; u++) {
            for (int v = u + 1; v < n; v++) {
                weights[u][v] = weights[v][u] = rng() % 8;
            }
        }

        long long expected = LINF;
        for (int mask = 1; mask < (1 << (n - 1)); mask++) {
            long long cut = 0;
            for (int u = 0; u < n; u++) {
                for (int v = u + 1; v < n; v++) {
                    if (((mask >> u) & 1) != ((mask >> v) & 1)) {
                        cut += weights[u][v];
                    }
                }
            }
            expected = min(expected, cut);
        }

        long long actual = global_min_cut(weights);
        if (actual != expected) {
            printf("stoer-wagner FAIL rep=%d n=%d expected=%lld actual=%lld\n", rep, n, expected,
                   actual);
            return 1;
        }
        checks++;
    }

    printf("stoer-wagner-min-cut PASS %lld", checks);
    return 0;
}
