// Stress: Floyd rho-shape vs a naive walk that records the step index at which
// each node is first seen. When the walk revisits a node, that node is the
// cycle start, its recorded step is mu, and lambda is the steps since then.
RhoShape naive_cycle(const vector<int>& f, int start) {
    vector<int> seen_at(f.size(), -1);
    int cur = start;
    int step = 0;
    while (seen_at[cur] == -1) {
        seen_at[cur] = step;
        cur = f[cur];
        step++;
    }
    RhoShape r;
    r.cycle_start = cur;
    r.mu = seen_at[cur];
    r.lambda = step - seen_at[cur];
    return r;
}

bool same(const RhoShape& a, const RhoShape& b) {
    return a.cycle_start == b.cycle_start and a.mu == b.mu and a.lambda == b.lambda;
}

int check(const vector<int>& f, int start, long long& checks) {
    RhoShape got = find_cycle(f, start);
    RhoShape ref = naive_cycle(f, start);
    if (not same(got, ref)) {
        printf("FAIL n=%d start=%d got(%d,%lld,%lld) ref(%d,%lld,%lld)\n", (int)f.size(), start,
               got.cycle_start, got.mu, got.lambda, ref.cycle_start, ref.mu, ref.lambda);
        return 1;
    }
    checks++;
    return 0;
}

int stress() {
    mt19937 rng(12345);
    long long checks = 0;

    // Hand-built edge cases.
    if (check({0}, 0, checks)) return 1;     // self-loop
    if (check({1, 0}, 0, checks)) return 1;  // 2-cycle
    if (check({1, 0}, 1, checks)) return 1;
    if (check({1, 2, 3, 4, 2}, 0, checks)) return 1;  // pure tail into cycle
    if (check({1, 2, 3, 0}, 2, checks)) return 1;     // whole graph one cycle
    if (check({1, 2, 2}, 0, checks)) return 1;        // tail then self-loop

    // Random functional graphs, every node as a start.
    for (int n = 1; n <= 60; n++)
        for (int rep = 0; rep < 60; rep++) {
            vector<int> f(n);
            for (auto& x: f) x = rng() % n;
            for (int start = 0; start < n; start++)
                if (check(f, start, checks)) return 1;
        }

    printf("functional-graph-cycle PASS %lld", checks);
    return 0;
}
