vector<vector<int>> g;
vector<bool> vis;

/**
 * Depth-first traversal from u. Components, cycle detection, topo. O(V+E)
 */
void dfs(int u) {
    vis[u] = true;
    for (int v: g[u])
        if (not vis[v]) dfs(v);
}
