/**
 * Directed cycle detection + reconstruction. White/gray/black DFS; a gray
 * neighbor is a back edge. find() returns one cycle (empty if the graph is a
 * DAG). O(V+E)
 */
struct DirectedCycle {
    int n, cyc_start = -1, cyc_end = -1;
    vector<vector<int>> g;
    vector<int> color, par;

    DirectedCycle(int n) : n(n), g(n), color(n, 0), par(n, -1) {}

    void add_edge(int u, int v) {
        g[u].push_back(v);
    }

    bool dfs(int u) {
        color[u] = 1;
        for (int v: g[u]) {
            if (color[v] == 0) {
                par[v] = u;
                if (dfs(v)) return true;
            } else if (color[v] == 1) {
                cyc_start = v, cyc_end = u;
                return true;
            }
        }
        color[u] = 2;
        return false;
    }

    vector<int> find() {
        for (int u = 0; u < n; u++)
            if (color[u] == 0 and dfs(u)) break;
        if (cyc_start == -1) return {};

        vector<int> cycle;
        for (int v = cyc_end; v != cyc_start; v = par[v]) cycle.push_back(v);
        cycle.push_back(cyc_start);
        reverse(cycle.begin(), cycle.end());
        return cycle;
    }
};

/**
 * Example: find a directed cycle 0 -> 1 -> 2 -> 0.
 */
int main() {
    DirectedCycle dc(3);
    dc.add_edge(0, 1);
    dc.add_edge(1, 2);
    dc.add_edge(2, 0);
    for (int v: dc.find()) cout << v << " ";
    cout << "\n";  // -> 0 1 2
    return 0;
}
