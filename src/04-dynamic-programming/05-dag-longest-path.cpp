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

/**
 * Example: DAG 0->1->2->3 plus edge 0->2; longest path has 3 edges (0->1->2->3).
 */
int main() {
    int n = 4;
    g.assign(n, {});
    g[0] = {1, 2};
    g[1] = {2};
    g[2] = {3};

    dp.assign(n, -1);
    long long ans = 0;
    for (int u = 0; u < n; u++) ans = max(ans, dag_longest(u));
    cout << ans << "\n";  // -> 3
    return 0;
}
