static bool brute_feasible(int n, vector<DifferenceConstraints::Constraint> constraints) {
    const int bound = 9;
    vector<int> value(n);
    function<bool(int)> search = [&](int at) {
        if (at == n) {
            for (auto c: constraints)
                if (value[c.v] - value[c.u] > c.w) return false;
            return true;
        }
        for (value[at] = -bound; value[at] <= 0; value[at]++)
            if (search(at + 1)) return true;
        return false;
    };
    return search(0);
}

int stress() {
    mt19937 rng(806);
    long long checks = 0;
    for (int rep = 0; rep < 5000; rep++) {
        int n = 1 + rng() % 4;
        DifferenceConstraints dc(n);
        int m = rng() % 9;
        for (int i = 0; i < m; i++) {
            int u = rng() % n;
            int v = rng() % n;
            long long w = (int)(rng() % 7) - 3;
            dc.add_constraint(u, v, w);
        }

        auto [feasible, assignment] = dc.solve();
        bool expected = brute_feasible(n, dc.constraints);
        if (feasible != expected) {
            printf("FAIL feasibility got=%d expected=%d n=%d m=%d\n", feasible, expected, n, m);
            return 1;
        }
        if (feasible) {
            for (auto c: dc.constraints)
                if (assignment[c.v] - assignment[c.u] > c.w) {
                    printf("FAIL invalid assignment\n");
                    return 1;
                }
            for (auto x: assignment)
                if (x > 0) return 1;
        }
        checks++;
    }

    DifferenceConstraints empty(0);
    auto [feasible, assignment] = empty.solve();
    if (not feasible or not assignment.empty()) return 1;
    printf("difference-constraints PASS %lld", checks + 1);
    return 0;
}
