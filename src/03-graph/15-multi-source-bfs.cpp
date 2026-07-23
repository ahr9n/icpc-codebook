int n;
vector<vector<int>> g;
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
        for (int v: g[u])
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
    }
}

/**
 * Example: distance from nearest of sources {0, 4} on a 5-node path.
 */
int main() {
    n = 5;
    g.assign(n, {});
    g[0].push_back(1);
    g[1].push_back(0);
    g[1].push_back(2);
    g[2].push_back(1);
    g[2].push_back(3);
    g[3].push_back(2);
    g[3].push_back(4);
    g[4].push_back(3);
    vector<int> sources = {0, 4};
    multi_source_bfs(sources);
    cout << dist[2] << "\n";  // -> 2
    return 0;
}
