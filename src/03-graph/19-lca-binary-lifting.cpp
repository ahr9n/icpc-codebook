int n;
vector<vector<int>> g;

/**
 * LCA by binary lifting. Preprocess up[k][v] = 2^k-th ancestor + depth via DFS,
 * then lca in O(log n). LOG must satisfy 2^LOG >= n. O(V log V) build.
 */
const int LOG = 20;
vector<array<int, LOG>> up;
vector<int> depth;

void lca_dfs(int u, int parent) {
    up[u][0] = parent < 0 ? u : parent;
    for (int k = 1; k < LOG; k++) up[u][k] = up[up[u][k - 1]][k - 1];
    for (int v: g[u])
        if (v != parent) {
            depth[v] = depth[u] + 1;
            lca_dfs(v, u);
        }
}

void lca_build(int root) {
    up.assign(n, {});
    depth.assign(n, 0);
    lca_dfs(root, -1);
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

/**
 * Example: build an undirected tree, then answer LCA and SCC on a directed graph.
 */
int main() {
    n = 5;
    g.assign(n, {});
    int tree_edges[][2] = {{0, 1}, {0, 2}, {1, 3}, {1, 4}};
    for (auto& e: tree_edges) {
        g[e[0]].push_back(e[1]);
        g[e[1]].push_back(e[0]);
    }
    lca_build(0);
    cout << lca(3, 4) << '\n';
    cout << lca(3, 2) << '\n';

    g.assign(n, {});
    int dir_edges[][2] = {{0, 1}, {1, 2}, {2, 0}, {2, 3}, {3, 4}};
    for (auto& e: dir_edges) g[e[0]].push_back(e[1]);
    cout << tarjan_scc() << '\n';

    return 0;
}
