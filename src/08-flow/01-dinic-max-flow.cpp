const long long LINF = 0x3f3f3f3f3f3f3f3fLL;

/**
 * Dinic max flow. BFS builds a level graph, DFS sends blocking flows along it.
 * add_edge(u, v, cap) then max_flow(s, t). O(V^2 * E) general, fast in practice.
 */
struct Dinic {
    struct Edge {
        int to;
        long long cap;
    };
    vector<Edge> edges;
    vector<vector<int>> g;
    vector<int> level, it;
    int n;

    Dinic(int n) : g(n), level(n), it(n), n(n) {}

    void add_edge(int u, int v, long long cap) {
        g[u].push_back(edges.size());
        edges.push_back({v, cap});
        g[v].push_back(edges.size());
        edges.push_back({u, 0});
    }

    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        level[s] = 0;
        q.push(s);
        while (not q.empty()) {
            int u = q.front();
            q.pop();
            for (int id: g[u]) {
                if (edges[id].cap > 0 and level[edges[id].to] == -1) {
                    level[edges[id].to] = level[u] + 1;
                    q.push(edges[id].to);
                }
            }
        }
        return level[t] != -1;
    }

    long long dfs(int u, int t, long long pushed) {
        if (u == t or pushed == 0) return pushed;
        for (int& i = it[u]; i < (int)g[u].size(); i++) {
            int id = g[u][i], v = edges[id].to;
            if (edges[id].cap <= 0 or level[v] != level[u] + 1) continue;
            long long tr = dfs(v, t, min(pushed, edges[id].cap));
            if (tr > 0) {
                edges[id].cap -= tr;
                edges[id ^ 1].cap += tr;
                return tr;
            }
        }
        return 0;
    }

    long long max_flow(int s, int t) {
        long long flow = 0;
        while (bfs(s, t)) {
            fill(it.begin(), it.end(), 0);
            while (long long f = dfs(s, t, LINF)) flow += f;
        }
        return flow;
    }
};

/**
 * Example: 2-unit source->sink flow, and a 2-edge bipartite matching.
 */
int main() {
    Dinic din(4);
    din.add_edge(0, 1, 1);
    din.add_edge(0, 2, 1);
    din.add_edge(1, 3, 1);
    din.add_edge(2, 3, 1);
    cout << din.max_flow(0, 3) << '\n';

    adj_l = {{0}, {1}};
    cout << bipartite_matching(2, 2) << '\n';
    return 0;
}
