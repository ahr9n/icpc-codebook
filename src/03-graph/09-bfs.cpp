int n;
vector<vector<pair<int, long long>>> adj;
vector<long long> dist;

/**
 * Unweighted shortest path (edge count) from src. Queue frontier. O(V+E)
 */
void bfs(int src) {
    dist.assign(n, -1);
    queue<int> q;

    dist[src] = 0;
    q.push(src);

    while (not q.empty()) {
        int u = q.front();
        q.pop();
        for (auto [v, w]: adj[u])
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
    }
}
