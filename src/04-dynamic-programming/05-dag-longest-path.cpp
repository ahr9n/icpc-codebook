vector<vector<int>> g;
vector<long long> dp;

/**
 * Longest path in a DAG from u. Memoized DFS; dp[u] = longest path starting at u.
 * Caller: dp.assign(n, -1), answer = max over all u. O(V+E)
 */
long long dag_longest(int u) {
    if (dp[u] != -1) return dp[u];
    dp[u] = 0;
    for (int v: g[u]) dp[u] = max(dp[u], 1 + dag_longest(v));
    return dp[u];
}
