/**
 * LCA by binary lifting. build() fills up[v][k] = 2^k-th ancestor + depth via
 * DFS, then lca() answers in O(log n). LOG must satisfy 2^LOG >= n. O(V log V)
 * build.
 */
struct Lca {
    static const int LOG = 20;
    int n;
    vector<vector<int>> g;
    vector<array<int, LOG>> up;
    vector<int> depth;

    Lca(int n) : n(n), g(n), up(n), depth(n, 0) {}

    void add_edge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void build(int u, int parent = -1) {
        up[u][0] = parent < 0 ? u : parent;
        for (int k = 1; k < LOG; k++) up[u][k] = up[up[u][k - 1]][k - 1];
        for (int v: g[u])
            if (v != parent) {
                depth[v] = depth[u] + 1;
                build(v, u);
            }
    }

    int lca(int a, int b) {
        if (depth[a] < depth[b]) swap(a, b);
        int diff = depth[a] - depth[b];
        for (int k = 0; k < LOG; k++)
            if (diff & (1 << k)) a = up[a][k];
        if (a == b) return a;
        for (int k = LOG - 1; k >= 0; k--)
            if (up[a][k] != up[b][k]) a = up[a][k], b = up[b][k];
        return up[a][0];
    }
};

/**
 * Example: build a small tree, then answer two LCA queries.
 */
int main() {
    Lca tree(5);
    int edges[][2] = {{0, 1}, {0, 2}, {1, 3}, {1, 4}};
    for (auto& e: edges) tree.add_edge(e[0], e[1]);
    tree.build(0);
    cout << tree.lca(3, 4) << '\n';  // -> 1
    cout << tree.lca(3, 2) << '\n';  // -> 0
    return 0;
}
