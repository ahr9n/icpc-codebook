int n;
vector<vector<pair<int, long long>>> adj;
vector<bool> in_mst;

/**
 * MST weight from adjacency list. Grow tree, add cheapest crossing edge. O(E log V)
 */
long long prim(int src = 0) {
    in_mst.assign(n, false);
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;

    pq.push({0, src});
    long long total = 0;
    int cnt = 0;

    while (not pq.empty() and cnt < n) {
        auto [w, u] = pq.top();
        pq.pop();
        if (in_mst[u]) continue;

        in_mst[u] = true;
        total += w;
        cnt++;

        for (auto [v, wt]: adj[u])
            if (not in_mst[v]) pq.push({wt, v});
    }
    return cnt == n ? total : -1;
}
