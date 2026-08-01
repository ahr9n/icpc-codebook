// Stress: generic parallel binary search vs replaying every update prefix for
// point-add updates and per-position threshold queries.
int stress() {
    mt19937 rng(24);
    long long checks = 0;

    int resets = 0;
    vector<int> empty =
        parallel_binary_search(0, 0, [&]() { resets++; }, [&](int) {}, [&](int) { return false; });
    if (not empty.empty() or resets != 1) {
        printf("parallel-binary-search FAIL(empty)\n");
        return 1;
    }
    checks++;

    for (int rep = 0; rep < 5000; rep++) {
        int positions = 1 + rng() % 8;
        int update_count = rng() % 31;
        int query_count = 1 + rng() % 30;
        vector<pair<int, int>> updates(update_count);
        for (auto& [position, delta]: updates) {
            position = rng() % positions;
            delta = rng() % 6;
        }

        vector<pair<int, int>> queries(query_count);
        for (auto& [position, target]: queries) {
            position = rng() % positions;
            target = rng() % 61;
        }

        vector<int> value(positions);
        auto reset = [&]() { fill(value.begin(), value.end(), 0); };
        auto apply_update = [&](int i) { value[updates[i].first] += updates[i].second; };
        auto is_satisfied = [&](int q) { return value[queries[q].first] >= queries[q].second; };
        vector<int> got =
            parallel_binary_search(update_count, query_count, reset, apply_update, is_satisfied);

        if (any_of(value.begin(), value.end(), [](int current) { return current != 0; })) {
            printf("parallel-binary-search FAIL(reset) rep=%d\n", rep);
            return 1;
        }
        checks++;

        for (int q = 0; q < query_count; q++) {
            vector<int> brute_value(positions);
            int want = update_count + 1;
            for (int prefix = 0; prefix <= update_count; prefix++) {
                if (brute_value[queries[q].first] >= queries[q].second) {
                    want = prefix;
                    break;
                }
                if (prefix < update_count) {
                    brute_value[updates[prefix].first] += updates[prefix].second;
                }
            }
            if (got[q] != want) {
                printf("parallel-binary-search FAIL rep=%d q=%d got=%d want=%d\n", rep, q, got[q],
                       want);
                return 1;
            }
            checks++;
        }
    }

    printf("parallel-binary-search PASS %lld", checks);
    return 0;
}
