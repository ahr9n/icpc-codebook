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

/**
 * Example: maximum matching in a small bipartite graph.
 */
int main() {
    int left = 3, right = 3;
    adj_l.assign(left, {});
    adj_l[0] = {0, 1};
    adj_l[1] = {0};
    adj_l[2] = {1, 2};
    cout << bipartite_matching(left, right) << "\n";  // -> 3
    return 0;
}
