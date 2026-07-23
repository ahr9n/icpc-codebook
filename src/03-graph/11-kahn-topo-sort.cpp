int n;
vector<vector<int>> g;

/**
 * Kahn: topological order of a DAG. Peel zero-indegree nodes.
 * order.size() < n => graph has a cycle. O(V+E)
 */
vector<int> kahn() {
    vector<int> indeg(n, 0), order;
    for (int u = 0; u < n; u++)
        for (int v: g[u]) indeg[v]++;

    queue<int> q;
    for (int u = 0; u < n; u++)
        if (indeg[u] == 0) q.push(u);

    while (not q.empty()) {
        int u = q.front();
        q.pop();
        order.push_back(u);
        for (int v: g[u])
            if (--indeg[v] == 0) q.push(v);
    }
    return order;
}

/**
 * Example: topological order of a small DAG.
 */
int main() {
    n = 4;
    g.assign(n, {});
    g[0].push_back(1);
    g[0].push_back(2);
    g[1].push_back(3);
    g[2].push_back(3);
    vector<int> order = kahn();
    for (int v: order) cout << v << " ";
    cout << "\n";  // -> 0 1 2 3
    return 0;
}
