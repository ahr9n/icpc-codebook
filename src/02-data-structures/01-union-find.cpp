/**
 * Union-Find / DSU: track connected groups. Path compression + union by size.
 * find/unite ~O(1) amortized. Use: connectivity, Kruskal, grouping. cnt = #components.
 */
struct DSU {
    vector<int> par, sz;
    int cnt;

    DSU(int n) : par(n), sz(n, 1), cnt(n) {
        iota(par.begin(), par.end(), 0);
    }

    int find(int x) {
        return par[x] == x ? x : par[x] = find(par[x]);
    }

    bool unite(int u, int v) {
        u = find(u), v = find(v);
        if (u == v) return false;
        if (sz[u] < sz[v]) swap(u, v);
        par[v] = u;
        sz[u] += sz[v];
        cnt--;
        return true;
    }
};
