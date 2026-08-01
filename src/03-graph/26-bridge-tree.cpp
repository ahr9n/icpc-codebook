/**
 * Bridge tree (2-edge-connected component tree). Low-link DFS marks every
 * bridge, a DSU over the non-bridge edges contracts each 2-edge-connected
 * component to one node, and the bridges become the edges joining those nodes.
 * comp[v] is v's component id; on a connected graph the result is a tree with
 * (num_comp - 1) edges, exactly one per bridge. build() fills comp and tree.
 * O(V + E). Edge ids (not parent vertex) skip the parent, so parallel edges
 * are correctly treated as non-bridges. Contract: undirected graph.
 */
struct BridgeTree {
    int n, timer = 0, edge_count = 0, num_comp = 0;
    vector<vector<pair<int, int>>> g;
    vector<pair<int, int>> edges;
    vector<int> low, num, par, sz, comp;
    vector<bool> is_bridge;
    vector<vector<int>> tree;

    BridgeTree(int n) : n(n), g(n), low(n), num(n, -1), par(n), sz(n, 1) {
        iota(par.begin(), par.end(), 0);
    }

    void add_edge(int u, int v) {
        g[u].push_back({v, edge_count});
        g[v].push_back({u, edge_count});
        edges.push_back({u, v});
        edge_count++;
    }

    int find(int x) {
        return par[x] == x ? x : par[x] = find(par[x]);
    }

    void unite(int u, int v) {
        u = find(u), v = find(v);
        if (u == v) return;
        if (sz[u] < sz[v]) swap(u, v);
        par[v] = u;
        sz[u] += sz[v];
    }

    void dfs(int u, int parent_edge) {
        low[u] = num[u] = timer++;

        for (auto [v, id]: g[u]) {
            if (id == parent_edge) continue;
            if (num[v] != -1) {
                low[u] = min(low[u], num[v]);
                continue;
            }
            dfs(v, id);
            low[u] = min(low[u], low[v]);
            if (low[v] > num[u]) is_bridge[id] = true;
        }
    }

    void build() {
        is_bridge.assign(edge_count, false);
        for (int u = 0; u < n; u++)
            if (num[u] == -1) dfs(u, -1);

        for (int id = 0; id < edge_count; id++)
            if (not is_bridge[id]) unite(edges[id].first, edges[id].second);

        comp.assign(n, -1);
        vector<int> label(n, -1);
        for (int v = 0; v < n; v++) {
            int root = find(v);
            if (label[root] == -1) label[root] = num_comp++;
            comp[v] = label[root];
        }

        tree.assign(num_comp, {});
        for (int id = 0; id < edge_count; id++)
            if (is_bridge[id]) {
                int a = comp[edges[id].first], b = comp[edges[id].second];
                tree[a].push_back(b);
                tree[b].push_back(a);
            }
    }
};

/**
 * Example: two triangles joined by a single bridge edge collapse to two
 * component nodes linked by one tree edge.
 */
int main() {
    BridgeTree bt(6);
    bt.add_edge(0, 1);
    bt.add_edge(1, 2);
    bt.add_edge(2, 0);
    bt.add_edge(3, 4);
    bt.add_edge(4, 5);
    bt.add_edge(5, 3);
    bt.add_edge(2, 3);
    bt.build();

    int tree_edges = 0;
    for (auto& adj: bt.tree) tree_edges += adj.size();
    tree_edges /= 2;

    cout << bt.num_comp << "\n";                 // -> 2
    cout << tree_edges << "\n";                  // -> 1
    cout << (bt.comp[0] == bt.comp[2]) << "\n";  // -> 1
    cout << (bt.comp[0] == bt.comp[3]) << "\n";  // -> 0
    return 0;
}
