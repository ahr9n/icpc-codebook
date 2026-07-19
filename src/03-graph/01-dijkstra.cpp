int n;
const long long LINF = 0x3f3f3f3f3f3f3f3fLL;
vector<vector<pair<int, long long>>> adj;
vector<long long> dist;

/**
 * SSSP, non-negative weights. Min-heap, relax neighbors. O(E log V)
 */
void dijkstra(int src) {
    dist.assign(n, LINF);
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;

    dist[src] = 0;
    pq.push({0, src});

    while (not pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d > dist[u]) continue;

        for (auto [v, w]: adj[u])
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
    }
}
