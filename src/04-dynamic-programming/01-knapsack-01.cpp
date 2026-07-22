int n, W;
vector<int> wt, val;
vector<long long> dp;
vector<vector<long long>> mem;

/**
 * 0/1 Knapsack by weight. dp[c] = best value at capacity c; iterate c downward. O(n*W)
 */
long long knapsack01() {
    dp.assign(W + 1, 0);
    for (int i = 0; i < n; i++)
        for (int c = W; c >= wt[i]; c--) dp[c] = max(dp[c], dp[c - wt[i]] + val[i]);
    return dp[W];
}

// top-down: mem[i][cap] = best value from items i.. with cap left.
// Caller: mem.assign(n, vector<long long>(W + 1, -1)), knapsack01_rec(0, W).
long long knapsack01_rec(int i, int cap) {
    if (i == n) return 0;

    long long& ret = mem[i][cap];
    if (~ret) return ret;

    ret = knapsack01_rec(i + 1, cap);
    if (wt[i] <= cap) 
        ret = max(ret, knapsack01_rec(i + 1, cap - wt[i]) + val[i]);

    return ret;
}
