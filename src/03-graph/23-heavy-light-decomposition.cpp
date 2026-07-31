/**
 * Heavy-light decomposition for path queries on a node-valued tree. Each node
 * points at its heavy child (the one with the largest subtree); following heavy
 * edges carves the tree into chains laid out contiguously, so any path u..v
 * spans O(log n) chains. A Fenwick over that layout then answers path sums.
 * point_update sets one node's value; path_query returns the sum on the path.
 * Build O(n); each update / query O(log^2 n). Values live on nodes, not edges.
 */
struct Hld {
    int n, pos_counter = 0;
    vector<vector<int>> adj;
    vector<long long> node_value, fen;
    vector<int> par, dep, heavy, sub, chain_head, pos;

    Hld(int n)
        : n(n), adj(n), node_value(n, 0), fen(n + 1, 0), par(n, -1), dep(n, 0), heavy(n, -1),
          sub(n, 0), chain_head(n, 0), pos(n, 0) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void fen_add(int i, long long delta) {
        for (i++; i <= n; i += i & (-i)) fen[i] += delta;
    }

    long long fen_prefix(int i) {
        long long sum = 0;
        for (i++; i > 0; i -= i & (-i)) sum += fen[i];
        return sum;
    }

    int dfs_size(int u, int p) {
        par[u] = p;
        sub[u] = 1;
        heavy[u] = -1;
        int max_sub = 0;
        for (int w: adj[u]) {
            if (w == p) continue;
            dep[w] = dep[u] + 1;
            int child = dfs_size(w, u);
            if (child > max_sub) {
                max_sub = child;
                heavy[u] = w;
            }
            sub[u] += child;
        }
        return sub[u];
    }

    void dfs_chains(int u, int h) {
        chain_head[u] = h;
        pos[u] = pos_counter++;
        if (heavy[u] != -1) dfs_chains(heavy[u], h);
        for (int w: adj[u])
            if (w != par[u] and w != heavy[u]) dfs_chains(w, w);
    }

    void build(int root, vector<long long>& values) {
        node_value = values;
        dfs_size(root, -1);
        dfs_chains(root, root);
        for (int u = 0; u < n; u++) fen_add(pos[u], node_value[u]);
    }

    void point_update(int u, long long new_value) {
        fen_add(pos[u], new_value - node_value[u]);
        node_value[u] = new_value;
    }

    long long path_query(int u, int v) {
        long long res = 0;
        while (chain_head[u] != chain_head[v]) {
            if (dep[chain_head[u]] < dep[chain_head[v]]) swap(u, v);
            res += fen_prefix(pos[u]) - fen_prefix(pos[chain_head[u]] - 1);
            u = par[chain_head[u]];
        }
        if (dep[u] > dep[v]) swap(u, v);
        res += fen_prefix(pos[v]) - fen_prefix(pos[u] - 1);
        return res;
    }
};

/**
 * Example: tree 0-{1,2}, 1-{3,4}, 2-{5,6} with values [1..7]. Path 3..6 is
 * 3-1-0-2-6 summing 4+2+1+3+7 = 17; after node 0 becomes 10 it is 26.
 */
int main() {
    Hld hld(7);
    int edges[6][2] = {{0, 1}, {0, 2}, {1, 3}, {1, 4}, {2, 5}, {2, 6}};
    for (auto& e: edges) hld.add_edge(e[0], e[1]);
    vector<long long> values = {1, 2, 3, 4, 5, 6, 7};
    hld.build(0, values);
    cout << hld.path_query(3, 6) << "\n";
    hld.point_update(0, 10);
    cout << hld.path_query(3, 6) << "\n";
    // -> 17
    // -> 26
    return 0;
}
