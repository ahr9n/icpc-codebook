const int HK_INF = 0x3f3f3f3f;

/**
 * Hopcroft-Karp maximum bipartite matching. Each phase BFS-layers the left
 * vertices by shortest augmenting distance, then DFS augments along all
 * shortest vertex-disjoint paths at once; the number of phases is O(sqrt(V)),
 * giving O(E * sqrt(V)) overall -- the fast alternative to Kuhn's O(V * E).
 * add_edge(u, v) links left u to right v; max_matching() returns the size and
 * leaves match_l / match_r as the pairing (-1 = unmatched).
 */
struct HopcroftKarp {
    int n_left, n_right;
    vector<vector<int>> adj;
    vector<int> match_l, match_r, dist;

    HopcroftKarp(int n_left, int n_right) : n_left(n_left), n_right(n_right), adj(n_left) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
    }

    bool bfs() {
        queue<int> q;
        for (int u = 0; u < n_left; u++) {
            if (match_l[u] == -1) {
                dist[u] = 0;
                q.push(u);
            } else
                dist[u] = HK_INF;
        }
        bool found = false;
        while (not q.empty()) {
            int u = q.front();
            q.pop();
            for (int v: adj[u]) {
                int w = match_r[v];
                if (w == -1)
                    found = true;
                else if (dist[w] == HK_INF) {
                    dist[w] = dist[u] + 1;
                    q.push(w);
                }
            }
        }
        return found;
    }

    bool dfs(int u) {
        for (int v: adj[u]) {
            int w = match_r[v];
            if (w == -1 or (dist[w] == dist[u] + 1 and dfs(w))) {
                match_l[u] = v;
                match_r[v] = u;
                return true;
            }
        }
        dist[u] = HK_INF;
        return false;
    }

    int max_matching() {
        match_l.assign(n_left, -1);
        match_r.assign(n_right, -1);
        dist.assign(n_left, 0);
        int result = 0;
        while (bfs())
            for (int u = 0; u < n_left; u++)
                if (match_l[u] == -1 and dfs(u)) result++;
        return result;
    }
};

/**
 * Example: maximum matching in a 3x3 bipartite graph.
 */
int main() {
    HopcroftKarp hk(3, 3);
    hk.add_edge(0, 0);
    hk.add_edge(0, 1);
    hk.add_edge(1, 0);
    hk.add_edge(2, 1);
    hk.add_edge(2, 2);
    cout << hk.max_matching() << '\n';
    return 0;
}
// -> 3
