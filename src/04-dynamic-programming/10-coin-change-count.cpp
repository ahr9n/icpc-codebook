int W;
vector<int> coins;
vector<long long> dp;
vector<vector<long long>> mem;

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

// top-down: mem[i][rem] = ways to form rem using coins i.. with reuse.
// Caller: mem.assign(coins.size(), vector<long long>(W + 1, -1)), coin_change_count_rec(0, W).
long long coin_change_count_rec(int i, int rem) {
    if (rem == 0) return 1;
    if (i == (int)coins.size() or rem < 0) return 0;

    long long& ret = mem[i][rem];
    if (~ret) return ret;

    return ret = coin_change_count_rec(i + 1, rem) + coin_change_count_rec(i, rem - coins[i]);
}
