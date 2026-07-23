int n;
vector<vector<int>> g;
vector<int> comp;

/**
 * Tarjan SCC: label each vertex with its strongly-connected component in comp[].
 * One DFS with low-links and a stack; comp ids come out in reverse topological order.
 * Caller: comp.assign(n, -1), then run for each unvisited node. O(V+E)
 */
vector<int> tj_low, tj_num, tj_stk;
vector<bool> tj_on;
int tj_timer, tj_cnt;

void scc_dfs(int u) {
    tj_low[u] = tj_num[u] = tj_timer++;
    tj_stk.push_back(u);
    tj_on[u] = true;

    for (int v: g[u]) {
        if (tj_num[v] == -1) {
            scc_dfs(v);
            tj_low[u] = min(tj_low[u], tj_low[v]);
        } else if (tj_on[v])
            tj_low[u] = min(tj_low[u], tj_num[v]);
    }

    if (tj_low[u] == tj_num[u]) {
        while (true) {
            int v = tj_stk.back();
            tj_stk.pop_back();
            tj_on[v] = false;
            comp[v] = tj_cnt;
            if (v == u) break;
        }
        tj_cnt++;
    }
}

int tarjan_scc() {
    tj_low.assign(n, 0);
    tj_num.assign(n, -1);
    tj_on.assign(n, false);
    tj_stk.clear();
    comp.assign(n, -1);
    tj_timer = tj_cnt = 0;

    for (int u = 0; u < n; u++)
        if (tj_num[u] == -1) scc_dfs(u);
    return tj_cnt;
}

/**
 * Example: two SCCs {0,1,2} and {3,4} linked by edge 2 -> 3.
 */
int main() {
    n = 5;
    g.assign(n, {});
    g[0].push_back(1);
    g[1].push_back(2);
    g[2].push_back(0);
    g[2].push_back(3);
    g[3].push_back(4);
    g[4].push_back(3);
    cout << tarjan_scc() << "\n";  // -> 2
    return 0;
}
