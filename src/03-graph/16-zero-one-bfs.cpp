int n;
const long long LINF = 0x3f3f3f3f3f3f3f3fLL;
vector<vector<pair<int, long long>>> adj;
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
        for (auto [v, w]: adj[u])
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                if (w == 0)
                    dq.push_front(v);
                else
                    dq.push_back(v);
            }
    }
}
