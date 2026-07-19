int n;
const long long LINF = 0x3f3f3f3f3f3f3f3fLL;
struct Edge {
    int u, v;
    long long w;
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
            if (dist[e.u] < LINF and dist[e.u] + e.w < dist[e.v]) {
                dist[e.v] = dist[e.u] + e.w;
                changed = true;
            }
        if (not changed) break;
    }

    for (Edge& e: edges)
        if (dist[e.u] < LINF and dist[e.u] + e.w < dist[e.v]) return true;
    return false;
}
