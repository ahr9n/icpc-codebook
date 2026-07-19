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
