int n;
struct Edge {
    int u, v;
    long long w;
};
vector<Edge> edges;

/**
 * MST weight from edge list. Sort edges, union if they join two components. O(E log E)
 */
long long kruskal() {
    sort(edges.begin(), edges.end(), [](Edge& a, Edge& b) { return a.w < b.w; });
    DSU dsu(n);
    long long total = 0;
    int cnt = 0;

    for (Edge& e: edges)
        if (dsu.unite(e.u, e.v)) {
            total += e.w;
            if (++cnt == n - 1) break;
        }
    return cnt == n - 1 ? total : -1;
}
