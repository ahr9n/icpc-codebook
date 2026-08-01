/**
 * Feasible circulation with lower and upper edge bounds. For each edge u -> v,
 * first commit its lower bound and leave capacity (upper - lower). The committed
 * flow creates a balance at every vertex. A super-source supplies positive
 * balances, a super-sink absorbs negative balances, and a max-flow saturating
 * every super-source edge exists exactly when the original circulation is
 * feasible. solve(flow) returns flows in add_edge order, or false and an empty
 * vector when no circulation exists.
 *
 * Contract: 0 <= lower <= upper and all capacities, balances, and total flow fit
 * in long long. Parallel edges and self-loops are allowed. With Dinic, complexity
 * is O(V^2 * E) time and O(V + E) memory.
 */
struct LowerBoundCirculation {
    struct InputEdge {
        int from, to;
        long long lower, upper;
    };

    struct Dinic {
        struct Edge {
            int to;
            long long capacity;
        };

        vector<Edge> edges;
        vector<vector<int>> adj;
        vector<int> level, next_edge;

        Dinic(int n) : adj(n), level(n), next_edge(n) {}

        int add_edge(int from, int to, long long capacity) {
            int id = edges.size();
            adj[from].push_back(id);
            edges.push_back({to, capacity});
            adj[to].push_back(id + 1);
            edges.push_back({from, 0});
            return id;
        }

        bool bfs(int source, int sink) {
            fill(level.begin(), level.end(), -1);
            queue<int> q;
            level[source] = 0;
            q.push(source);
            while (not q.empty()) {
                int u = q.front();
                q.pop();
                for (int id: adj[u]) {
                    int v = edges[id].to;
                    if (edges[id].capacity > 0 and level[v] == -1) {
                        level[v] = level[u] + 1;
                        q.push(v);
                    }
                }
            }
            return level[sink] != -1;
        }

        long long dfs(int u, int sink, long long pushed) {
            if (u == sink or pushed == 0) {
                return pushed;
            }
            for (int& index = next_edge[u]; index < (int)adj[u].size(); index++) {
                int id = adj[u][index];
                int v = edges[id].to;
                if (edges[id].capacity == 0 or level[v] != level[u] + 1) {
                    continue;
                }
                long long sent = dfs(v, sink, min(pushed, edges[id].capacity));
                if (sent > 0) {
                    edges[id].capacity -= sent;
                    edges[id ^ 1].capacity += sent;
                    return sent;
                }
            }
            return 0;
        }

        long long max_flow(int source, int sink) {
            long long flow = 0;
            while (bfs(source, sink)) {
                fill(next_edge.begin(), next_edge.end(), 0);
                while (true) {
                    long long pushed = dfs(source, sink, LINF);
                    if (pushed == 0) {
                        break;
                    }
                    flow += pushed;
                }
            }
            return flow;
        }
    };

    int n;
    vector<InputEdge> input_edges;

    LowerBoundCirculation(int n) : n(n) {}

    void add_edge(int from, int to, long long lower, long long upper) {
        input_edges.push_back({from, to, lower, upper});
    }

    bool solve(vector<long long>& flow) {
        int super_source = n;
        int super_sink = n + 1;
        Dinic dinic(n + 2);
        vector<long long> balance(n);
        vector<int> residual_edge(input_edges.size());

        for (int i = 0; i < (int)input_edges.size(); i++) {
            InputEdge edge = input_edges[i];
            residual_edge[i] = dinic.add_edge(edge.from, edge.to, edge.upper - edge.lower);
            balance[edge.from] -= edge.lower;
            balance[edge.to] += edge.lower;
        }

        long long required = 0;
        for (int v = 0; v < n; v++) {
            if (balance[v] > 0) {
                dinic.add_edge(super_source, v, balance[v]);
                required += balance[v];
            } else if (balance[v] < 0) {
                dinic.add_edge(v, super_sink, -balance[v]);
            }
        }

        if (dinic.max_flow(super_source, super_sink) != required) {
            flow.clear();
            return false;
        }

        flow.resize(input_edges.size());
        for (int i = 0; i < (int)input_edges.size(); i++) {
            long long residual_capacity = input_edges[i].upper - input_edges[i].lower;
            long long extra = residual_capacity - dinic.edges[residual_edge[i]].capacity;
            flow[i] = input_edges[i].lower + extra;
        }
        return true;
    }
};

/** Example: a three-edge cycle whose feasible flow is two on every edge. */
int main() {
    LowerBoundCirculation circulation(3);
    circulation.add_edge(0, 1, 2, 4);
    circulation.add_edge(1, 2, 1, 3);
    circulation.add_edge(2, 0, 2, 3);

    vector<long long> flow;
    cout << circulation.solve(flow) << "\n";
    for (int i = 0; i < (int)flow.size(); i++) {
        if (i > 0) {
            cout << " ";
        }
        cout << flow[i];
    }
    cout << "\n";
    return 0;
}
// -> 1
// -> 2 2 2
