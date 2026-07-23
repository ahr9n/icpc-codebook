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

/**
 * Example: DFS from 0 counts reachable nodes in one component.
 */
int main() {
    int n = 5;
    g.assign(n, {});
    g[0].push_back(1);
    g[1].push_back(2);
    g[2].push_back(3);
    g[3].push_back(4);
    vis.assign(n, false);
    dfs(0);
    int reached = count(vis.begin(), vis.end(), true);
    cout << reached << "\n";  // -> 5
    return 0;
}
