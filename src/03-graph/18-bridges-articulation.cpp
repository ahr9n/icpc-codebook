/**
 * Bridges & articulation points (undirected). DFS low-links: edge (u,v) is a
 * bridge when low[v] > num[u]; u is a cut vertex when a child has low[v] >= num[u].
 * run() fills bridges and is_cut. O(V+E)
 */
struct Bridges {
    int n, timer = 0;
    vector<vector<int>> g;
    vector<int> low, num;
    vector<bool> is_cut;
    vector<pair<int, int>> bridges;

    Bridges(int n) : n(n), g(n), low(n), num(n, -1), is_cut(n) {}

    void add_edge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void dfs(int u, int parent) {
        low[u] = num[u] = timer++;
        int children = 0;

        for (int v: g[u]) {
            if (v == parent) continue;
            if (num[v] != -1) {
                low[u] = min(low[u], num[v]);
                continue;
            }
            children++;
            dfs(v, u);
            low[u] = min(low[u], low[v]);
            if (low[v] > num[u]) bridges.push_back({u, v});
            if (parent != -1 and low[v] >= num[u]) is_cut[u] = true;
        }
        if (parent == -1 and children > 1) is_cut[u] = true;
    }

    void run() {
        for (int u = 0; u < n; u++)
            if (num[u] == -1) dfs(u, -1);
    }
};

/**
 * Example: a 4-node path graph where every edge is a bridge.
 */
int main() {
    Bridges b(4);
    b.add_edge(0, 1);
    b.add_edge(1, 2);
    b.add_edge(2, 3);
    b.run();
    cout << b.bridges.size() << "\n";  // -> 3
    return 0;
}
