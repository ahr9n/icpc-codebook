const long long LINF = 0x3f3f3f3f3f3f3f3fLL;

/**
 * Min-cost max-flow via successive shortest augmenting paths. Each round SPFA
 * finds a cheapest source->sink path in the residual graph, then pushes the
 * bottleneck along it; summing until the sink is unreachable yields the maximum
 * flow, and among all maximum flows the one of minimum total cost. SPFA (not
 * Dijkstra) so negative edge costs are fine; the graph must have no negative
 * cost cycle. add_edge(u, v, cap, cost), then min_cost_flow(s, t). O(V * E * flow)
 * worst case, far faster in practice.
 */
struct MinCostMaxFlow {
    struct Edge {
        int to;
        long long cap, cost;
    };
    vector<Edge> edges;
    vector<vector<int>> g;
    int n;

    MinCostMaxFlow(int n) : g(n), n(n) {}

    void add_edge(int u, int v, long long cap, long long cost) {
        g[u].push_back(edges.size());
        edges.push_back({v, cap, cost});
        g[v].push_back(edges.size());
        edges.push_back({u, 0, -cost});
    }

    pair<long long, long long> min_cost_flow(int src, int sink) {
        long long total_flow = 0, total_cost = 0;
        while (true) {
            vector<long long> dist(n, LINF);
            vector<int> parent_edge(n, -1);
            vector<bool> in_queue(n, false);
            deque<int> q;
            dist[src] = 0;
            q.push_back(src);
            in_queue[src] = true;

            while (not q.empty()) {
                int u = q.front();
                q.pop_front();
                in_queue[u] = false;
                for (int id: g[u]) {
                    int v = edges[id].to;
                    if (edges[id].cap > 0 and dist[u] + edges[id].cost < dist[v]) {
                        dist[v] = dist[u] + edges[id].cost;
                        parent_edge[v] = id;
                        if (not in_queue[v]) {
                            in_queue[v] = true;
                            q.push_back(v);
                        }
                    }
                }
            }
            if (dist[sink] == LINF) break;

            long long pushed = LINF;
            for (int v = sink; v != src; v = edges[parent_edge[v] ^ 1].to)
                pushed = min(pushed, edges[parent_edge[v]].cap);
            for (int v = sink; v != src; v = edges[parent_edge[v] ^ 1].to) {
                edges[parent_edge[v]].cap -= pushed;
                edges[parent_edge[v] ^ 1].cap += pushed;
            }
            total_flow += pushed;
            total_cost += pushed * dist[sink];
        }
        return {total_flow, total_cost};
    }
};

/**
 * Example: min-cost max-flow from 0 to 3; prints "flow cost".
 */
int main() {
    MinCostMaxFlow mc(4);
    mc.add_edge(0, 1, 2, 1);
    mc.add_edge(0, 2, 1, 3);
    mc.add_edge(1, 3, 1, 1);
    mc.add_edge(2, 3, 2, 1);
    mc.add_edge(1, 2, 1, 1);
    pair<long long, long long> result = mc.min_cost_flow(0, 3);
    cout << result.first << ' ' << result.second << '\n';
    return 0;
}
// -> 3 9
