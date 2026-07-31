/**
 * Tarjan SCC: label each vertex with its strongly-connected component in comp[].
 * One DFS with low-links and a stack; comp ids come out in reverse topological
 * order. run() returns the number of components. O(V+E)
 */
struct TarjanScc {
    int n, timer = 0, scc_count = 0;
    vector<vector<int>> g;
    vector<int> comp, low, num, stk;
    vector<bool> on_stack;

    TarjanScc(int n) : n(n), g(n), comp(n, -1), low(n), num(n, -1), on_stack(n) {}

    void add_edge(int u, int v) {
        g[u].push_back(v);
    }

    void dfs(int u) {
        low[u] = num[u] = timer++;
        stk.push_back(u);
        on_stack[u] = true;

        for (int v: g[u]) {
            if (num[v] == -1) {
                dfs(v);
                low[u] = min(low[u], low[v]);
            } else if (on_stack[v])
                low[u] = min(low[u], num[v]);
        }

        if (low[u] == num[u]) {
            while (true) {
                int v = stk.back();
                stk.pop_back();
                on_stack[v] = false;
                comp[v] = scc_count;
                if (v == u) break;
            }
            scc_count++;
        }
    }

    int run() {
        for (int u = 0; u < n; u++)
            if (num[u] == -1) dfs(u);
        return scc_count;
    }
};

/**
 * Example: two SCCs {0,1,2} and {3,4} linked by edge 2 -> 3.
 */
int main() {
    TarjanScc scc(5);
    scc.add_edge(0, 1);
    scc.add_edge(1, 2);
    scc.add_edge(2, 0);
    scc.add_edge(2, 3);
    scc.add_edge(3, 4);
    scc.add_edge(4, 3);
    cout << scc.run() << "\n";  // -> 2
    return 0;
}
