// Stress: divide-and-conquer min_partition_cost vs a plain O(groups * n^2) DP over
// the same squared-segment-sum cost (which obeys the quadrangle inequality).
int stress() {
    mt19937 rng(21);
    long long checks = 0;

    for (int trial = 0; trial < 4000; trial++) {
        int size = 1 + rng() % 16;
        int g = 1 + rng() % size;

        vector<long long> a(size);
        for (auto& v: a) v = rng() % 10;  // non-negative keeps cost quadrangle-convex

        n = size;
        groups = g;
        prefix.assign(size + 1, 0);
        for (int i = 0; i < size; i++) prefix[i + 1] = prefix[i] + a[i];

        long long got = min_partition_cost();

        vector<vector<long long>> ref(g + 1, vector<long long>(size + 1, LINF));
        for (int i = 1; i <= size; i++) ref[1][i] = cost(0, i);
        for (int level = 2; level <= g; level++)
            for (int i = 1; i <= size; i++)
                for (int j = 0; j < i; j++) {
                    if (ref[level - 1][j] >= LINF) continue;
                    ref[level][i] = min(ref[level][i], ref[level - 1][j] + cost(j, i));
                }

        if (got != ref[g][size]) {
            printf("FAIL trial=%d n=%d g=%d got=%lld ref=%lld\n", trial, size, g, got, ref[g][size]);
            return 1;
        }
        checks++;
    }

    printf("divide-conquer-dp PASS %lld", checks);
    return 0;
}
