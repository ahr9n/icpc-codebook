int n;
const long long LINF = 0x3f3f3f3f3f3f3f3fLL;
struct Edge {
    int u, v;
    long long w;
};
vector<Edge> edges;
vector<long long> dist;

/**
 * Cheapest src->dst using at most k+1 edges (k stops). Rolling layer per hop. O(k*E)
 */
long long bellman_ford_k_edges(int src, int dst, int k) {
    dist.assign(n, LINF);
    dist[src] = 0;

    for (int i = 0; i <= k; i++) {
        vector<long long> cur = dist;
        for (Edge& e: edges)
            if (dist[e.u] < LINF and dist[e.u] + e.w < cur[e.v]) cur[e.v] = dist[e.u] + e.w;
        if (cur == dist) break;
        dist = cur;
    }
    return dist[dst] == LINF ? -1 : dist[dst];
}
