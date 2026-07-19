const long long LINF = 0x3f3f3f3f3f3f3f3fLL;
int n;
vector<int> a;
vector<vector<long long>> dp;
vector<vector<long long>> mem;

/**
 * Minimax interval DP (take from either end): two players alternately remove a[l] or
 * a[r], each maximizing their own score. dp[l][r] = best margin for the mover. O(n^2)
 */
long long game_ends() {
    dp.assign(n, vector<long long>(n, 0));
    for (int i = 0; i < n; i++) dp[i][i] = a[i];
    for (int len = 2; len <= n; len++)
        for (int l = 0; l + len - 1 < n; l++) {
            int r = l + len - 1;
            dp[l][r] = max(a[l] - dp[l + 1][r], a[r] - dp[l][r - 1]);
        }
    return dp[0][n - 1];
}

// top-down: mem[l][r] = best margin for the mover on a[l..r]. Margin can be negative,
// so LINF marks "uncomputed". Caller: mem.assign(n, vector<long long>(n, LINF)), rec(0, n-1).
long long game_ends_rec(int l, int r) {
    if (l > r) return 0;

    long long& ret = mem[l][r];
    if (ret != LINF) return ret;

    return ret = max(a[l] - game_ends_rec(l + 1, r), a[r] - game_ends_rec(l, r - 1));
}
