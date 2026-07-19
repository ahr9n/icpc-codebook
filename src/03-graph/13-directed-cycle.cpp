int n;
vector<vector<int>> g;
vector<int> color;

/**
 * Directed cycle detection + reconstruction. White/gray/black DFS; a gray neighbor
 * is a back edge. Returns one cycle (empty if the graph is a DAG). O(V+E)
 */
vector<int> cyc_par;
int cyc_start, cyc_end;

bool cycle_dfs(int u) {
    color[u] = 1;
    for (int v: g[u]) {
        if (color[v] == 0) {
            cyc_par[v] = u;
            if (cycle_dfs(v)) return true;
        } else if (color[v] == 1) {
            cyc_start = v, cyc_end = u;
            return true;
        }
    }
    color[u] = 2;
    return false;
}

vector<int> find_directed_cycle() {
    color.assign(n, 0);
    cyc_par.assign(n, -1);
    cyc_start = -1;

    for (int u = 0; u < n; u++)
        if (color[u] == 0 and cycle_dfs(u)) break;
    if (cyc_start == -1) return {};

    vector<int> cycle;
    for (int v = cyc_end; v != cyc_start; v = cyc_par[v]) cycle.push_back(v);
    cycle.push_back(cyc_start);
    reverse(cycle.begin(), cycle.end());
    return cycle;
}
