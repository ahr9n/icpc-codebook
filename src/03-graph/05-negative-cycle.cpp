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
vector<long long> dist;

/**
 * Detect ANY negative cycle (all sources) and return one, else empty.
 * Virtual source: dist all 0, relax n times; an nth-round change sits on a cycle.
 * For a POSITIVE cycle, negate every edge weight and reuse. O(V*E)
 */
vector<int> find_negative_cycle() {
    dist.assign(n, 0);
    vector<int> par(n, -1);
    int x = -1;

    for (int i = 0; i < n; i++) {
        x = -1;
        for (Edge& e: edges)
            if (dist[e.from] + e.cost < dist[e.to]) {
                dist[e.to] = dist[e.from] + e.cost;
                par[e.to] = e.from;
                x = e.to;
            }
    }
    if (x == -1) return {};

    for (int i = 0; i < n; i++) x = par[x];

    vector<int> cycle;
    for (int v = x;; v = par[v]) {
        cycle.push_back(v);
        if (v == x and cycle.size() > 1) break;
    }
    reverse(cycle.begin(), cycle.end());
    return cycle;
}

/**
 * Example: detect a negative cycle 0 -> 1 -> 2 -> 0 (total weight -1).
 */
int main() {
    n = 3;
    edges.clear();
    edges.push_back({0, 1, 1});
    edges.push_back({1, 2, -1});
    edges.push_back({2, 0, -1});
    vector<int> cycle = find_negative_cycle();
    for (int v: cycle) cout << v << " ";
    cout << "\n";  // -> 0 1 2 0
    return 0;
}
