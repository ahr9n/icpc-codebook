vector<vector<pair<int, long long>>> adj;
vector<bool> vis;

/**
 * Depth-first traversal from u. Components, cycle detection, topo. O(V+E)
 */
void dfs(int u) {
    vis[u] = true;
    for (auto [v, w]: adj[u])
        if (not vis[v]) dfs(v);
}
