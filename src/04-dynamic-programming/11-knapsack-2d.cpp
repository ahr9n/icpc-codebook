int n, W, D;
vector<int> wt, val, a;
vector<vector<long long>> dp;

/**
 * Two-constraint (2D) knapsack: maximize value under two capacities W and D.
 * dp[x][y] over both capacities; iterate both downward for 0/1 items. O(n*W*D)
 */
long long knapsack_2d() {
    dp.assign(W + 1, vector<long long>(D + 1, 0));
    for (int i = 0; i < n; i++)
        for (int x = W; x >= wt[i]; x--)
            for (int y = D; y >= val[i]; y--)
                dp[x][y] = max(dp[x][y], dp[x - wt[i]][y - val[i]] + a[i]);
    return dp[W][D];
}
