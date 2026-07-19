vector<vector<pair<int, long long>>> adj;
vector<int> color;

/**
 * Bipartite check / 2-coloring. BFS paints alternating colors; clash => not bipartite.
 * Caller: color.assign(n, -1) then run for each uncolored node. O(V+E)
 */
bool is_bipartite(int src) {
    queue<int> q;
    color[src] = 0;
    q.push(src);

    while (not q.empty()) {
        int u = q.front();
        q.pop();
        for (auto [v, w]: adj[u]) {
            if (color[v] == -1) {
                color[v] = color[u] ^ 1;
                q.push(v);
            } else if (color[v] == color[u])
                return false;
        }
    }
    return true;
}
