/**
 * Centroid decomposition to count unordered node pairs at tree distance <= k.
 * Repeatedly remove a centroid (a node whose every remaining subtree has <= half
 * the component's nodes) so each path passes through exactly one centroid. At a
 * centroid, sort the distances of all reachable nodes and two-pointer the pairs
 * summing to <= k; inclusion-exclusion subtracts pairs sharing a subtree, whose
 * true path never crosses the centroid. Build O(n log n); counting O(n log^2 n).
 */
vector<vector<int>> adj;
vector<bool> removed;
vector<int> sub;
long long pair_count;
int dist_limit;

int calc_size(int u, int p) {
    sub[u] = 1;
    for (int w: adj[u])
        if (w != p and not removed[w]) sub[u] += calc_size(w, u);
    return sub[u];
}

int find_centroid(int u, int p, int comp) {
    for (int w: adj[u])
        if (w != p and not removed[w] and sub[w] > comp / 2) return find_centroid(w, u, comp);
    return u;
}

void collect(int u, int p, int d, vector<int>& out) {
    out.push_back(d);
    for (int w: adj[u])
        if (w != p and not removed[w]) collect(w, u, d + 1, out);
}

long long count_pairs(vector<int> dists) {
    sort(dists.begin(), dists.end());
    long long cnt = 0;
    int lo = 0, hi = (int)dists.size() - 1;
    while (lo < hi) {
        if (dists[lo] + dists[hi] <= dist_limit) {
            cnt += hi - lo;
            lo++;
        } else {
            hi--;
        }
    }
    return cnt;
}

void decompose(int entry) {
    int comp = calc_size(entry, -1);
    int c = find_centroid(entry, -1, comp);
    vector<int> all;
    collect(c, -1, 0, all);
    pair_count += count_pairs(all);
    for (int w: adj[c]) {
        if (removed[w]) continue;
        vector<int> branch;
        collect(w, c, 1, branch);
        pair_count -= count_pairs(branch);
    }
    removed[c] = true;
    for (int w: adj[c])
        if (not removed[w]) decompose(w);
}

/**
 * Example: tree 0-{1,2}, 1-{3,4}, 2-{5,6}. Pairs at distance <= 2 number 13
 * (6 adjacent plus 7 at distance exactly 2).
 */
int main() {
    int n = 7;
    adj.assign(n, {});
    removed.assign(n, false);
    sub.assign(n, 0);
    int edges[6][2] = {{0, 1}, {0, 2}, {1, 3}, {1, 4}, {2, 5}, {2, 6}};
    for (auto& e: edges) {
        adj[e[0]].push_back(e[1]);
        adj[e[1]].push_back(e[0]);
    }
    dist_limit = 2;
    pair_count = 0;
    decompose(0);
    cout << pair_count << "\n";  // -> 13
    return 0;
}
