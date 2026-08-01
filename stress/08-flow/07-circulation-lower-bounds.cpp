// Stress: lower-bound circulation feasibility vs exhaustive flow assignment on
// small graphs; every returned solution is also checked for bounds and balance.
int stress() {
    mt19937 rng(7);
    long long checks = 0;

    LowerBoundCirculation impossible(2);
    impossible.add_edge(0, 1, 1, 1);
    vector<long long> impossible_flow = {42};
    if (impossible.solve(impossible_flow) or not impossible_flow.empty()) {
        printf("circulation-lower-bounds FAIL(deterministic-infeasible)\n");
        return 1;
    }
    checks++;

    LowerBoundCirculation self_loop(1);
    self_loop.add_edge(0, 0, 2, 4);
    vector<long long> self_loop_flow;
    if (not self_loop.solve(self_loop_flow) or self_loop_flow != vector<long long>{2}) {
        printf("circulation-lower-bounds FAIL(self-loop)\n");
        return 1;
    }
    checks++;

    for (int rep = 0; rep < 5000; rep++) {
        int n = 1 + rng() % 5;
        int edge_count = rng() % 8;
        vector<LowerBoundCirculation::InputEdge> edges;
        LowerBoundCirculation circulation(n);
        for (int i = 0; i < edge_count; i++) {
            int from = rng() % n;
            int to = rng() % n;
            long long lower = rng() % 3;
            long long upper = lower + rng() % 3;
            edges.push_back({from, to, lower, upper});
            circulation.add_edge(from, to, lower, upper);
        }

        vector<long long> candidate(edge_count);
        function<bool(int)> brute = [&](int index) {
            if (index == edge_count) {
                vector<long long> balance(n);
                for (int i = 0; i < edge_count; i++) {
                    balance[edges[i].from] -= candidate[i];
                    balance[edges[i].to] += candidate[i];
                }
                return all_of(balance.begin(), balance.end(),
                              [](long long value) { return value == 0; });
            }
            for (long long value = edges[index].lower; value <= edges[index].upper; value++) {
                candidate[index] = value;
                if (brute(index + 1)) {
                    return true;
                }
            }
            return false;
        };

        bool want = brute(0);
        vector<long long> flow;
        bool got = circulation.solve(flow);
        if (got != want) {
            printf("circulation-lower-bounds FAIL(exists) rep=%d got=%d want=%d\n", rep, got, want);
            return 1;
        }
        checks++;

        if (got) {
            vector<long long> balance(n);
            for (int i = 0; i < edge_count; i++) {
                if (flow[i] < edges[i].lower or flow[i] > edges[i].upper) {
                    printf("circulation-lower-bounds FAIL(bounds) rep=%d edge=%d\n", rep, i);
                    return 1;
                }
                balance[edges[i].from] -= flow[i];
                balance[edges[i].to] += flow[i];
                checks++;
            }
            for (int v = 0; v < n; v++) {
                if (balance[v] != 0) {
                    printf("circulation-lower-bounds FAIL(balance) rep=%d v=%d\n", rep, v);
                    return 1;
                }
                checks++;
            }
        } else if (not flow.empty()) {
            printf("circulation-lower-bounds FAIL(nonempty) rep=%d\n", rep);
            return 1;
        }
    }

    printf("circulation-lower-bounds PASS %lld", checks);
    return 0;
}
