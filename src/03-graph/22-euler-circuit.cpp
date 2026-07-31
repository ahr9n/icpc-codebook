/**
 * Euler path / circuit on an undirected multigraph via Hierholzer. An Euler
 * trail visits every edge exactly once; it exists iff all edges sit in one
 * connected component and the count of odd-degree vertices is 0 (a closed
 * circuit, may start anywhere) or 2 (an open path, must start at an odd
 * vertex). add_edge(u, v) for each edge, then euler_path() returns the vertex
 * sequence of length edges+1, or an empty vector when no trail exists.
 *
 * The visited[] marks an edge id shared by both endpoint entries so each edge
 * is walked once; the it[] cursor makes the whole walk amortized O(V + E).
 */
struct EulerUndirected {
    int n, edge_cnt = 0;
    vector<vector<pair<int, int>>> g;  // g[u] = list of (neighbor, edge_id)
    vector<int> degree;

    EulerUndirected(int n) : n(n), g(n), degree(n, 0) {}

    void add_edge(int u, int v) {
        g[u].push_back({v, edge_cnt});
        g[v].push_back({u, edge_cnt});
        degree[u]++;
        degree[v]++;
        edge_cnt++;
    }

    vector<int> euler_path() {
        if (edge_cnt == 0) return {};

        int start = -1, odd = 0;
        for (int v = 0; v < n; v++) {
            if (degree[v] % 2 == 1) {
                odd++;
                start = v;
            }
        }
        if (odd != 0 and odd != 2) return {};
        if (start == -1)
            for (int v = 0; v < n; v++)
                if (degree[v] > 0) {
                    start = v;
                    break;
                }

        vector<bool> visited(edge_cnt, false);
        vector<int> it(n, 0), stk = {start}, path;
        while (not stk.empty()) {
            int u = stk.back();
            while (it[u] < (int)g[u].size() and visited[g[u][it[u]].second]) it[u]++;
            if (it[u] == (int)g[u].size()) {
                path.push_back(u);
                stk.pop_back();
            } else {
                int v = g[u][it[u]].first, id = g[u][it[u]].second;
                it[u]++;
                visited[id] = true;
                stk.push_back(v);
            }
        }

        if ((int)path.size() != edge_cnt + 1) return {};  // edges span >1 component
        reverse(path.begin(), path.end());
        return path;
    }
};

/**
 * Example: square 0-1-2-3-0, every vertex even degree, so an Euler circuit.
 */
int main() {
    EulerUndirected eu(4);
    eu.add_edge(0, 1);
    eu.add_edge(1, 2);
    eu.add_edge(2, 3);
    eu.add_edge(3, 0);
    vector<int> path = eu.euler_path();
    for (int i = 0; i < (int)path.size(); i++) cout << path[i] << " \n"[i + 1 == (int)path.size()];
    return 0;
}
// -> 0 1 2 3 0
