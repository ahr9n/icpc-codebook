int n;
vector<vector<pair<int, long long>>> adj;

/**
 * Bridges & articulation points (undirected). DFS low-links: edge (u,v) is a bridge
 * when low[v] > num[u]; u is a cut vertex when a child has low[v] >= num[u].
 * Caller: br_num.assign(n, -1), run per component. O(V+E)
 */
vector<int> br_low, br_num;
vector<bool> is_cut;
vector<pair<int, int>> bridges;
int br_timer;

void bridge_dfs(int u, int parent) {
    br_low[u] = br_num[u] = br_timer++;
    int children = 0;

    for (auto [v, w]: adj[u]) {
        if (v == parent) continue;
        if (br_num[v] != -1) {
            br_low[u] = min(br_low[u], br_num[v]);
            continue;
        }
        children++;
        bridge_dfs(v, u);
        br_low[u] = min(br_low[u], br_low[v]);
        if (br_low[v] > br_num[u]) bridges.push_back({u, v});
        if (parent != -1 and br_low[v] >= br_num[u]) is_cut[u] = true;
    }
    if (parent == -1 and children > 1) is_cut[u] = true;
}

void find_bridges() {
    br_low.assign(n, 0);
    br_num.assign(n, -1);
    is_cut.assign(n, false);
    bridges.clear();
    br_timer = 0;

    for (int u = 0; u < n; u++)
        if (br_num[u] == -1) bridge_dfs(u, -1);
}
