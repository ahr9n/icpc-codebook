int n;
vector<vector<int>> g;
vector<long long> dist;

/**
 * Tree diameter: two-sweep BFS. Farthest node from any src, then farthest from it.
 * Returns the diameter (edge count on the unweighted tree). O(V)
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
        for (int v: g[u])
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
    }

    dist.assign(n, -1);
    dist[far] = 0;
    q.push(far);
    long long diam = 0;

    while (not q.empty()) {
        int u = q.front();
        q.pop();
        diam = max(diam, dist[u]);
        for (int v: g[u])
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
    }
    return diam;
}

/**
 * Example: diameter of a 5-node path tree.
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
    cout << tree_diameter() << "\n";  // -> 4
    return 0;
}
