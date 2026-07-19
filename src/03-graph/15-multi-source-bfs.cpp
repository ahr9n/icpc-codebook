int n;
vector<vector<pair<int, long long>>> adj;
vector<long long> dist;

/**
 * Multi-source BFS: distance to the nearest of many sources. Seed all at dist 0. O(V+E)
 */
void multi_source_bfs(vector<int>& sources) {
    dist.assign(n, -1);
    queue<int> q;
    for (int s: sources) {
        dist[s] = 0;
        q.push(s);
    }

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
