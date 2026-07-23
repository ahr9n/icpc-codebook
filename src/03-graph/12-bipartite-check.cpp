vector<vector<int>> g;
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
        for (int v: g[u]) {
            if (color[v] == -1) {
                color[v] = color[u] ^ 1;
                q.push(v);
            } else if (color[v] == color[u])
                return false;
        }
    }
    return true;
}

/**
 * Example: a 4-cycle graph is bipartite (2-colorable).
 */
int main() {
    int n = 4;
    g.assign(n, {});
    g[0].push_back(1);
    g[1].push_back(0);
    g[1].push_back(2);
    g[2].push_back(1);
    g[2].push_back(3);
    g[3].push_back(2);
    g[3].push_back(0);
    g[0].push_back(3);
    color.assign(n, -1);
    cout << is_bipartite(0) << "\n";  // -> 1
    return 0;
}
