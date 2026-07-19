/**
 * Kuhn bipartite matching: augmenting paths from each left node. adj_l[u] = right
 * neighbors of left node u. Returns the maximum matching size. O(V * E)
 */
vector<vector<int>> adj_l;
vector<int> match_r;
vector<bool> used_r;

bool try_kuhn(int u) {
    for (int v: adj_l[u]) {
        if (used_r[v]) continue;
        used_r[v] = true;
        if (match_r[v] == -1 or try_kuhn(match_r[v])) {
            match_r[v] = u;
            return true;
        }
    }
    return false;
}

int bipartite_matching(int left, int right) {
    match_r.assign(right, -1);
    int res = 0;
    for (int u = 0; u < left; u++) {
        used_r.assign(right, false);
        if (try_kuhn(u)) res++;
    }
    return res;
}
