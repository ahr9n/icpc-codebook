struct GomoryHuEdge {
    int u, v;
    long long cut;
};

struct GomoryHuFlow {
    struct Edge {
        int to, reverse;
        long long capacity;
    };

    int n;
    vector<vector<Edge>> graph;
    vector<int> level, next_edge;

    GomoryHuFlow(int n) : n(n), graph(n), level(n), next_edge(n) {}

    void add_directed_edge(int u, int v, long long capacity) {
        int reverse_u = graph[v].size();
        int reverse_v = graph[u].size();
        graph[u].push_back({v, reverse_u, capacity});
        graph[v].push_back({u, reverse_v, 0});
    }

    bool build_levels(int source, int sink) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        level[source] = 0;
        q.push(source);
        while (not q.empty()) {
            int u = q.front();
            q.pop();
            for (const Edge& edge: graph[u]) {
                if (edge.capacity > 0 and level[edge.to] == -1) {
                    level[edge.to] = level[u] + 1;
                    q.push(edge.to);
                }
            }
        }
        return level[sink] != -1;
    }

    long long send_flow(int u, int sink, long long pushed) {
        if (u == sink) {
            return pushed;
        }
        for (int& id = next_edge[u]; id < (int)graph[u].size(); id++) {
            Edge& edge = graph[u][id];
            if (edge.capacity == 0 or level[edge.to] != level[u] + 1) {
                continue;
            }
            long long sent = send_flow(edge.to, sink, min(pushed, edge.capacity));
            if (sent == 0) {
                continue;
            }
            edge.capacity -= sent;
            graph[edge.to][edge.reverse].capacity += sent;
            return sent;
        }
        return 0;
    }

    long long max_flow(int source, int sink) {
        long long result = 0;
        while (build_levels(source, sink)) {
            fill(next_edge.begin(), next_edge.end(), 0);
            while (long long pushed = send_flow(source, sink, LLONG_MAX)) {
                result += pushed;
            }
        }
        return result;
    }

    vector<bool> reachable_from(int source) {
        vector<bool> seen(n, false);
        queue<int> q;
        seen[source] = true;
        q.push(source);
        while (not q.empty()) {
            int u = q.front();
            q.pop();
            for (const Edge& edge: graph[u]) {
                if (edge.capacity > 0 and not seen[edge.to]) {
                    seen[edge.to] = true;
                    q.push(edge.to);
                }
            }
        }
        return seen;
    }
};

/**
 * Builds a Gomory–Hu tree for all-pairs minimum cuts in an undirected weighted
 * graph. The minimum cut between any two vertices equals the lightest edge on
 * their unique tree path, reducing n*(n-1)/2 cut values to n-1 weighted edges.
 * The construction runs n-1 max-flow computations; with the included Dinic
 * implementation this is O(n * V^2 * E) in the usual worst-case bound.
 *
 * capacity must be symmetric and non-negative; capacity[u][v] is the combined
 * weight of parallel edges. The matrix must be square, and all flow and residual
 * capacity sums must fit long long. The returned vector contains {vertex,
 * parent, cut_value} for vertices 1..n-1 and is empty when n <= 1.
 */
vector<GomoryHuEdge> gomory_hu_tree(const vector<vector<long long>>& capacity) {
    int n = capacity.size();
    vector<int> parent(n, 0);
    vector<long long> cut_value(n, 0);

    for (int source = 1; source < n; source++) {
        int sink = parent[source];
        GomoryHuFlow flow(n);
        for (int u = 0; u < n; u++) {
            for (int v = u + 1; v < n; v++) {
                if (capacity[u][v] == 0) {
                    continue;
                }
                flow.add_directed_edge(u, v, capacity[u][v]);
                flow.add_directed_edge(v, u, capacity[u][v]);
            }
        }

        long long current_cut = flow.max_flow(source, sink);
        vector<bool> source_side = flow.reachable_from(source);
        for (int vertex = source + 1; vertex < n; vertex++) {
            if (parent[vertex] == sink and source_side[vertex]) {
                parent[vertex] = source;
            }
        }

        if (source_side[parent[sink]]) {
            parent[source] = parent[sink];
            parent[sink] = source;
            cut_value[source] = cut_value[sink];
            cut_value[sink] = current_cut;
        } else {
            cut_value[source] = current_cut;
        }
    }

    vector<GomoryHuEdge> tree;
    for (int vertex = 1; vertex < n; vertex++) {
        tree.push_back({vertex, parent[vertex], cut_value[vertex]});
    }
    return tree;
}

/** Example: print the three edges representing all pair cuts of a 4-node graph. */
int main() {
    vector<vector<long long>> capacity(4, vector<long long>(4, 0));
    auto add_edge = [&](int u, int v, long long value) {
        capacity[u][v] += value;
        capacity[v][u] += value;
    };
    add_edge(0, 1, 3);
    add_edge(1, 2, 2);
    add_edge(2, 3, 4);
    add_edge(0, 3, 1);

    for (GomoryHuEdge edge: gomory_hu_tree(capacity)) {
        cout << edge.u << " " << edge.v << " " << edge.cut << "\n";
    }
    return 0;
}
// -> 1 0 4
// -> 2 1 3
// -> 3 2 5
