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
            if (not(mask & (1 << j)) and compat[i][j])
                dp[mask | (1 << j)] = (dp[mask | (1 << j)] + dp[mask]) % MOD;
    }
    return dp[(1 << n) - 1];
}

// top-down: dp[mask] = ways to complete from mask; i = popcount(mask) is next item.
// Caller: dp.assign(1 << n, -1), matching_rec(0).
long long matching_rec(int mask) {
    int i = __builtin_popcount(mask);
    if (i == n) return 1;

    long long& ret = dp[mask];
    if (~ret) return ret;

    ret = 0;
    for (int j = 0; j < n; j++)
        if (compat[i][j] and not(mask & (1 << j)))
            ret = (ret + matching_rec(mask | (1 << j))) % MOD;
    return ret;
}

/**
 * Example: 3 items, 3 slots, all compatible -> 3! = 6 perfect matchings.
 */
int main() {
    n = 3;
    compat.assign(n, vector<int>(n, 1));
    cout << matching() << "\n";  // -> 6

    dp.assign(1 << n, -1);
    cout << matching_rec(0) << "\n";  // -> 6
    return 0;
}
