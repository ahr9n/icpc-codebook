int n;
struct Edge {
    int u, v;
    long long w;

    // reversed so the default priority_queue (max-heap) pops the smallest weight
    bool operator<(const Edge& o) const {
        return w > o.w;
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
            if (dist[e.u] + e.w < dist[e.v]) {
                dist[e.v] = dist[e.u] + e.w;
                par[e.v] = e.u;
                x = e.v;
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
