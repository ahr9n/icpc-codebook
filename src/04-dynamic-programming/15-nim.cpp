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
