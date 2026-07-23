int n;
vector<vector<int>> g;
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
        for (int v: g[u])
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
    }
}

/**
 * Example: unweighted distances from 0; report distance to node 4.
 */
int main() {
    n = 5;
    g.assign(n, {});
    g[0].push_back(1);
    g[0].push_back(2);
    g[1].push_back(3);
    g[2].push_back(3);
    g[3].push_back(4);
    bfs(0);
    cout << dist[4] << "\n";  // -> 3
    return 0;
}
