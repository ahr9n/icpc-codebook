int n;
const long long LINF = 0x3f3f3f3f3f3f3f3fLL;
struct Edge {
    int from, to;
    long long cost;

    // reversed so the default priority_queue (max-heap) pops the smallest cost
    bool operator<(const Edge& o) const {
        return cost > o.cost;
    }
};
vector<vector<Edge>> adj;
vector<long long> dist;

/**
 * SSSP, non-negative weights. Min-heap, relax neighbors. O(E log V)
 */
void dijkstra(int src) {
    dist.assign(n, LINF);
    priority_queue<Edge> pq;

    dist[src] = 0;
    pq.push({src, src, 0});

    while (not pq.empty()) {
        Edge cur = pq.top();
        pq.pop();
        if (cur.cost > dist[cur.to]) continue;

        for (Edge e: adj[cur.to]) {
            long long new_cost = cur.cost + e.cost;
            if (new_cost >= dist[e.to]) continue;

            dist[e.to] = new_cost;
            e.cost = new_cost;
            pq.push(e);
        }
    }
}

/**
 * Example: shortest path 0 -> 4 in a small weighted digraph.
 */
int main() {
    n = 5;
    adj.assign(n, {});
    adj[0].push_back({0, 1, 2});
    adj[0].push_back({0, 2, 5});
    adj[1].push_back({1, 2, 1});
    adj[1].push_back({1, 3, 2});
    adj[2].push_back({2, 4, 3});
    adj[3].push_back({3, 4, 1});
    dijkstra(0);
    cout << dist[4] << "\n";  // -> 5
    return 0;
}
