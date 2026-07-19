const long long LINF = 0x3f3f3f3f3f3f3f3fLL;
int n;
vector<int> a;
vector<vector<long long>> dp;
vector<vector<long long>> mem;
vector<long long> pre;

/**
 * Interval DP (Slimes): min cost to merge a[0..n-1], a merge costs its element sum.
 * dp[i][j] over interval length; pre = prefix sums for the merge cost. O(n^3)
 */
long long slimes() {
    vector<long long> ps(n + 1, 0);
    for (int i = 0; i < n; i++) ps[i + 1] = ps[i] + a[i];

    dp.assign(n, vector<long long>(n, 0));
    for (int len = 2; len <= n; len++)
        for (int i = 0; i + len - 1 < n; i++) {
            int j = i + len - 1;
            dp[i][j] = LINF;
            for (int m = i; m < j; m++)
                dp[i][j] = min(dp[i][j], dp[i][m] + dp[m + 1][j] + ps[j + 1] - ps[i]);
        }
    return dp[0][n - 1];
}

// top-down: mem[l][r] = min cost to merge a[l..r]; pre = prefix sums.
// Caller: build pre (pre[i+1]=pre[i]+a[i]), mem.assign(n, vector<long long>(n, -1)), rec(0, n-1).
long long slimes_rec(int l, int r) {
    if (l == r) return 0;

    long long& ret = mem[l][r];
    if (~ret) return ret;

    ret = LINF;
    for (int k = l; k < r; k++)
        ret = min(ret, slimes_rec(l, k) + slimes_rec(k + 1, r) + pre[r + 1] - pre[l]);
    return ret;
}
