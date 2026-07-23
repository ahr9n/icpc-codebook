int n;
struct Edge {
    int from, to;
    long long cost;

    // reversed so the default priority_queue (max-heap) pops the smallest cost
    bool operator<(const Edge& o) const {
        return cost > o.cost;
    }
};
vector<Edge> edges;

/**
 * MST weight from edge list. Sort edges, union if they join two components. O(E log E)
 */
long long kruskal() {
    sort(edges.begin(), edges.end(), [](Edge& a, Edge& b) { return a.cost < b.cost; });
    DSU dsu(n);
    long long total = 0;
    int cnt = 0;

    for (Edge& e: edges)
        if (dsu.unite(e.from, e.to)) {
            total += e.cost;
            if (++cnt == n - 1) break;
        }
    return cnt == n - 1 ? total : -1;
}

/**
 * Example: MST weight of a 4-node weighted graph (edge list).
 */
int main() {
    n = 4;
    edges.clear();
    edges.push_back({0, 1, 1});
    edges.push_back({1, 2, 2});
    edges.push_back({2, 3, 3});
    edges.push_back({0, 3, 4});
    cout << kruskal() << "\n";  // -> 6
    return 0;
}
