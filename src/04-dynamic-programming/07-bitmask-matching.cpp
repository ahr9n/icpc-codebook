const long long MOD = 1e9 + 7;
int n;
vector<vector<int>> compat;
vector<long long> dp;

/**
 * Bitmask DP (assignment / EDPC Matching): count ways to match n items to n slots.
 * dp[mask] = ways to place the items already in mask; i = popcount(mask) is next item.
 * compat[i][j] = item i may take slot j. O(2^n * n)
 */
long long matching() {
    dp.assign(1 << n, 0);
    dp[0] = 1;
    for (int mask = 0; mask < (1 << n); mask++) {
        int i = __builtin_popcount(mask);
        for (int j = 0; j < n; j++)
            if (compat[i][j] and not(mask & (1 << j)))
                dp[mask | (1 << j)] = (dp[mask | (1 << j)] + dp[mask]) % MOD;
    }
    return dp[(1 << n) - 1];
}
