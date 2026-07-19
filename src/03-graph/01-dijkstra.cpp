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
vector<vector<Edge>> adj;
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
        if (cur.cost > dist[cur.to]) continue;

        for (Edge e: adj[cur.to]) {
            long long new_cost = cur.cost + e.cost;
            if (new_cost >= dist[e.to]) continue;

            dist[e.to] = new_cost;
            e.cost = new_cost;
            pq.push(e);
        }
    }
}
