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
vector<Edge> edges;
vector<long long> dist;

/**
 * Cheapest src->dst using at most k+1 edges (k stops). Rolling layer per hop. O(k*E)
 */
long long bellman_ford_k_edges(int src, int dst, int k) {
    dist.assign(n, LINF);
    dist[src] = 0;

    for (int i = 0; i <= k; i++) {
        vector<long long> cur = dist;
        for (Edge& e: edges)
            if (dist[e.from] < LINF and dist[e.from] + e.cost < cur[e.to])
                cur[e.to] = dist[e.from] + e.cost;
        if (cur == dist) break;
        dist = cur;
    }
    return dist[dst] == LINF ? -1 : dist[dst];
}

/**
 * Example: cheapest 0 -> 3 using at most k+1 = 2 edges (1 stop).
 */
int main() {
    n = 4;
    edges.clear();
    edges.push_back({0, 1, 100});
    edges.push_back({1, 2, 100});
    edges.push_back({2, 0, 100});
    edges.push_back({1, 3, 600});
    edges.push_back({2, 3, 200});
    cout << bellman_ford_k_edges(0, 3, 1) << "\n";  // -> 700
    return 0;
}
