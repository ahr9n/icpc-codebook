vector<vector<pair<int, long long>>> adj;

/**
 * Euler tour (tree flattening): map each subtree to a contiguous range
 * [tin[u], tout[u]], so subtree queries become range queries on a BIT / segment tree.
 * Caller: tin.assign(n, 0), tout.assign(n, 0), timer = 0, euler(root, -1). O(V)
 */
vector<int> tin, tout;
int timer;

void euler(int u, int parent) {
    tin[u] = timer++;
    for (auto [v, w]: adj[u])
        if (v != parent) euler(v, u);
    tout[u] = timer - 1;
}
