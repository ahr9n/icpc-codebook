int n;
struct Edge {
    int from, to;
    long long cost;

    // reversed so the default priority_queue (max-heap) pops the smallest cost
    bool operator<(const Edge& o) const {
        return cost > o.cost;
    }
};
vector<vector<Edge>> adj;
vector<bool> in_mst;

/**
 * MST weight from adjacency list. Grow tree, add cheapest crossing edge. O(E log V)
 */
long long prim(int src = 0) {
    in_mst.assign(n, false);
    priority_queue<Edge> pq;

    pq.push({src, src, 0});
    long long total = 0;
    int cnt = 0;

    while (not pq.empty() and cnt < n) {
        Edge cur = pq.top();
        pq.pop();
        if (in_mst[cur.to]) continue;

        in_mst[cur.to] = true;
        total += cur.cost;
        cnt++;

        for (Edge e: adj[cur.to])
            if (not in_mst[e.to]) pq.push(e);
    }
    return cnt == n ? total : -1;
}
