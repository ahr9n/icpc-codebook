int n;
const long long LINF = 0x3f3f3f3f3f3f3f3fLL;
struct Edge {
    int u, v;
    long long w;

    // reversed so the default priority_queue (max-heap) pops the smallest weight
    bool operator < (const Edge& o) const {
        return w > o.w;
    }
};
vector<vector<pair<int, long long>>> adj;
vector<long long> dist;

/**
 * SSSP, non-negative weights. Min-heap, relax neighbors. O(E log V)
 */
void dijkstra(int src) {
    dist.assign(n, LINF);
    priority_queue<Edge> pq;

    dist[src] = 0;
    pq.push({src, src, 0});

    while (not pq.empty()) {
        Edge cur = pq.top();
        pq.pop();
        if (cur.w > dist[cur.v]) continue;

        for (auto [v, w]: adj[cur.v])
            if (cur.w + w < dist[v]) {
                dist[v] = cur.w + w;
                pq.push({cur.v, v, dist[v]});
            }
    }
}
