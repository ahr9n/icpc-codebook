int W;
vector<int> coins;
vector<long long> dp;

/**
 * Coin change counting (unbounded): number of ways to form W using coins with reuse.
 * Iterate coins outer, capacity inner => counts combinations, not permutations. O(n*W)
 */
long long coin_change_count() {
    dp.assign(W + 1, 0);
    dp[0] = 1;
    for (int c: coins)
        for (int x = c; x <= W; x++) dp[x] += dp[x - c];
    return dp[W];
}
