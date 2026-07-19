int n, W, D;
vector<int> wt, val, a;
vector<vector<long long>> dp;
vector<vector<vector<long long>>> mem3;

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

// top-down: mem3[i][x][y] = best value from items i.. within capacities x, y.
// Caller: mem3.assign(n, vector<vector<long long>>(W + 1, vector<long long>(D + 1, -1))), rec(0, W,
// D).
long long knapsack_2d_rec(int i, int x, int y) {
    if (i == n) return 0;

    long long& ret = mem3[i][x][y];
    if (~ret) return ret;

    ret = knapsack_2d_rec(i + 1, x, y);
    if (wt[i] <= x and val[i] <= y)
        ret = max(ret, knapsack_2d_rec(i + 1, x - wt[i], y - val[i]) + a[i]);
    return ret;
}
