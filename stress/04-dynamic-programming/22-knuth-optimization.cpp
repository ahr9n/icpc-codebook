// Stress: knuth_merge_cost vs a plain O(n^3) interval DP with the same prefix-sum
// merge cost (which obeys the quadrangle inequality for non-negative weights).
int stress() {
    mt19937 rng(22);
    long long checks = 0;

    for (int trial = 0; trial < 4000; trial++) {
        int n = 1 + rng() % 14;
        vector<long long> weight(n);
        for (auto& w: weight) w = rng() % 20;  // non-negative keeps QI valid

        long long got = knuth_merge_cost(weight);

        vector<long long> pre(n + 1, 0);
        for (int i = 0; i < n; i++) pre[i + 1] = pre[i] + weight[i];
        vector<vector<long long>> dp(n, vector<long long>(n, 0));
        for (int len = 2; len <= n; len++)
            for (int i = 0; i + len - 1 < n; i++) {
                int j = i + len - 1;
                dp[i][j] = LINF;
                for (int k = i; k < j; k++)
                    dp[i][j] = min(dp[i][j], dp[i][k] + dp[k + 1][j] + pre[j + 1] - pre[i]);
            }

        if (got != dp[0][n - 1]) {
            printf("FAIL trial=%d n=%d got=%lld ref=%lld\n", trial, n, got, dp[0][n - 1]);
            return 1;
        }
        checks++;
    }

    printf("knuth-optimization PASS %lld", checks);
    return 0;
}
