int n;
const long long LINF = 0x3f3f3f3f3f3f3f3fLL;
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
 * SSSP with negative edges. Relax all edges n-1x; extra pass => negative cycle. O(V*E)
 */
bool bellman_ford(int src) {
    dist.assign(n, LINF);
    dist[src] = 0;

    for (int i = 0; i < n - 1; i++) {
        bool changed = false;
        for (Edge& e: edges)
            if (dist[e.from] < LINF and dist[e.from] + e.cost < dist[e.to]) {
                dist[e.to] = dist[e.from] + e.cost;
                changed = true;
            }
        if (not changed) break;
    }

    for (Edge& e: edges)
        if (dist[e.from] < LINF and dist[e.from] + e.cost < dist[e.to]) return true;
    return false;
}
