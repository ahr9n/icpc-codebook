const long long LINF = 0x3f3f3f3f3f3f3f3fLL;
int n;
vector<int> a;
vector<vector<long long>> dp;

/**
 * Interval DP (Slimes): min cost to merge a[0..n-1], a merge costs its element sum.
 * dp[i][j] over interval length; pre = prefix sums for the merge cost. O(n^3)
 */
long long slimes() {
    vector<long long> pre(n + 1, 0);
    for (int i = 0; i < n; i++) pre[i + 1] = pre[i] + a[i];

    dp.assign(n, vector<long long>(n, 0));
    for (int len = 2; len <= n; len++)
        for (int i = 0; i + len - 1 < n; i++) {
            int j = i + len - 1;
            dp[i][j] = LINF;
            for (int m = i; m < j; m++)
                dp[i][j] = min(dp[i][j], dp[i][m] + dp[m + 1][j] + pre[j + 1] - pre[i]);
        }
    return dp[0][n - 1];
}
