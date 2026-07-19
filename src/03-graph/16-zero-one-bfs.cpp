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
 * 0-1 BFS: shortest path when every edge weight is 0 or 1. Deque: 0-edge push front,
 * 1-edge push back. Replaces Dijkstra at O(V+E) on 0/1 weights.
 */
void bfs_01(int src) {
    dist.assign(n, LINF);
    deque<int> dq;
    dist[src] = 0;
    dq.push_front(src);

    while (not dq.empty()) {
        int u = dq.front();
        dq.pop_front();
        for (Edge e: adj[u])
            if (dist[u] + e.cost < dist[e.to]) {
                dist[e.to] = dist[u] + e.cost;
                if (e.cost == 0)
                    dq.push_front(e.to);
                else
                    dq.push_back(e.to);
            }
    }
}
