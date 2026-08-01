vector<vector<int>> adj;
vector<int> color, sub, answer, freq;
int distinct_colors = 0, heavy_skip = -1;

/**
 * DSU on tree ("sack" / small-to-large): answer a subtree query for every vertex
 * offline. Here answer[u] = number of distinct colors in u's subtree, kept in a
 * global freq[] with a live distinct count. dfs(u) recurses light children first
 * and rolls their contribution back, then recurses the heavy child and KEEPS it,
 * so u only has to re-add itself plus its light subtrees on top of what the heavy
 * child left behind.
 *
 * O(n log n): a vertex's color is (re)added only while it sits inside a LIGHT
 * subtree, and every light edge on the path to the root at least halves the
 * ancestor's size, so each vertex is touched O(log n) times total.
 *
 * Caller: size color[0..n-1], build adj, then solve(root). Colors are indices
 * into freq, so they must be 0..max_color.
 */
void calc_size(int u, int parent) {
    sub[u] = 1;
    for (int v: adj[u]) {
        if (v == parent) {
            continue;
        }
        calc_size(v, u);
        sub[u] += sub[v];
    }
}

void update(int u, int parent, int delta) {
    int c = color[u];
    if (delta > 0) {
        if (freq[c] == 0) {
            distinct_colors++;
        }
        freq[c]++;
    } else {
        freq[c]--;
        if (freq[c] == 0) {
            distinct_colors--;
        }
    }
    for (int v: adj[u]) {
        if (v != parent and v != heavy_skip) {
            update(v, u, delta);
        }
    }
}

void dfs(int u, int parent, bool keep) {
    int heavy = -1;
    for (int v: adj[u]) {
        if (v == parent) {
            continue;
        }
        if (heavy == -1 or sub[v] > sub[heavy]) {
            heavy = v;
        }
    }

    for (int v: adj[u]) {
        if (v == parent or v == heavy) {
            continue;
        }
        dfs(v, u, false);
    }
    if (heavy != -1) {
        dfs(heavy, u, true);
    }

    heavy_skip = heavy;
    update(u, parent, +1);
    heavy_skip = -1;
    answer[u] = distinct_colors;

    if (not keep) {
        update(u, parent, -1);
    }
}

void solve(int root, int n) {
    sub.assign(n, 0);
    answer.assign(n, 0);
    freq.assign(*max_element(color.begin(), color.end()) + 1, 0);
    distinct_colors = 0;
    calc_size(root, -1);
    dfs(root, -1, true);
}

/** Example: distinct-color counts for the root and one child subtree. */
int main() {
    int n = 7;
    color = {1, 2, 1, 3, 2, 1, 4};
    adj.assign(n, {});
    int edges[6][2] = {{0, 1}, {0, 2}, {1, 3}, {1, 4}, {2, 5}, {2, 6}};
    for (auto& e: edges) {
        adj[e[0]].push_back(e[1]);
        adj[e[1]].push_back(e[0]);
    }

    solve(0, n);
    cout << answer[0] << " " << answer[1] << "\n";
    return 0;
}
// -> 4 2
