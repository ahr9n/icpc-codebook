const long long MOD = 1e9 + 7;
vector<vector<int>> g;
vector<vector<long long>> dp;

/**
 * Tree DP (EDPC Painting): dp[u][0] = ways with u white, dp[u][1] = u black.
 * A child may be black only if its parent is white. Caller: dp.assign(n, {0,0}),
 * tree_dp(root, -1), answer = (dp[root][0] + dp[root][1]) % MOD. O(V)
 */
void tree_dp(int u, int par) {
    dp[u][0] = dp[u][1] = 1;
    for (int v: g[u]) {
        if (v == par) continue;
        tree_dp(v, u);
        dp[u][0] = dp[u][0] * (dp[v][0] + dp[v][1]) % MOD;
        dp[u][1] = dp[u][1] * dp[v][0] % MOD;
    }
}
