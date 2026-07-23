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

/**
 * Example: find a directed cycle 0 -> 1 -> 2 -> 0.
 */
int main() {
    n = 3;
    g.assign(n, {});
    g[0].push_back(1);
    g[1].push_back(2);
    g[2].push_back(0);
    vector<int> cycle = find_directed_cycle();
    for (int v: cycle) cout << v << " ";
    cout << "\n";  // -> 0 1 2
    return 0;
}
