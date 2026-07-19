int n, W;
vector<int> wt, val;
vector<long long> dp;

/**
 * 0/1 Knapsack by weight. dp[c] = best value at capacity c; iterate c downward. O(n*W)
 */
long long knapsack01() {
    dp.assign(W + 1, 0);
    for (int i = 0; i < n; i++)
        for (int c = W; c >= wt[i]; c--) dp[c] = max(dp[c], dp[c - wt[i]] + val[i]);
    return dp[W];
}
