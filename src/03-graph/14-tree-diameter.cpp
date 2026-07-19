int n;
vector<vector<pair<int, long long>>> adj;
vector<long long> dist;

/**
 * Tree diameter: two-sweep BFS. Farthest node from any src, then farthest from it.
 * Returns the diameter (edge count on unweighted adj). O(V)
 */
int tree_diameter() {
    dist.assign(n, -1);
    dist[0] = 0;
    queue<int> q;
    q.push(0);
    int far = 0;

    while (not q.empty()) {
        int u = q.front();
        q.pop();
        if (dist[u] > dist[far]) far = u;
        for (auto [v, w]: adj[u])
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
    }

    dist.assign(n, -1);
    dist[far] = 0;
    q.push(far);
    int diam = 0;

    while (not q.empty()) {
        int u = q.front();
        q.pop();
        diam = max(diam, dist[u]);
        for (auto [v, w]: adj[u])
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
    }
    return diam;
}
