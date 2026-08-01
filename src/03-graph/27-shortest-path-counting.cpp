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
vector<long long> dist;
vector<long long> ways;

/**
 * Number of distinct shortest paths from src to every node, non-negative
 * weights. Dijkstra carries a count: the first time a node is settled at a
 * shorter distance its count is reset to the parent's; every later relaxation
 * reaching it at the same distance adds the parent's count. O(E log V).
 *
 * Contract: edge weights must be non-negative (Dijkstra). ways[] is the path
 * count taken modulo MOD; ways[v] == 0 means v is unreachable. Parallel edges
 * and multiple equal-length routes are counted as distinct paths.
 */
void count_shortest_paths(int src) {
    dist.assign(n, LINF);
    ways.assign(n, 0);
    priority_queue<Edge> pq;

    dist[src] = 0;
    ways[src] = 1;
    pq.push({src, src, 0});

    while (not pq.empty()) {
        Edge cur = pq.top();
        pq.pop();
        if (cur.cost > dist[cur.to]) continue;

        for (Edge e: adj[cur.to]) {
            long long new_cost = cur.cost + e.cost;

            if (new_cost < dist[e.to]) {
                dist[e.to] = new_cost;
                ways[e.to] = ways[cur.to];
                e.cost = new_cost;
                pq.push(e);
            } else if (new_cost == dist[e.to]) {
                ways[e.to] = (ways[e.to] + ways[cur.to]) % MOD;
            }
        }
    }
}

/**
 * Example: two equal-length shortest routes 0 -> 3 (via 1 and via 2).
 */
int main() {
    n = 4;
    adj.assign(n, {});
    adj[0].push_back({0, 1, 1});
    adj[0].push_back({0, 2, 1});
    adj[1].push_back({1, 3, 1});
    adj[2].push_back({2, 3, 1});
    count_shortest_paths(0);
    cout << dist[3] << " " << ways[3] << "\n";  // -> 2 2
    cout << dist[1] << " " << ways[1] << "\n";  // -> 1 1
    return 0;
}
