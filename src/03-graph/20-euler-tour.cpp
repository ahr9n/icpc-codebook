vector<vector<int>> g;

/**
 * Euler tour (tree flattening): map each subtree to a contiguous range
 * [tin[u], tout[u]], so subtree queries become range queries on a BIT / segment tree.
 * Caller: tin.assign(n, 0), tout.assign(n, 0), timer = 0, euler(root, -1). O(V)
 */
vector<int> tin, tout;
int timer;

void euler(int u, int parent) {
    tin[u] = timer++;
    for (int v: g[u])
        if (v != parent) euler(v, u);
    tout[u] = timer - 1;
}

/**
 * Example: Euler tour of a tree; the root subtree spans [0, n-1].
 */
int main() {
    int n = 5;
    g.assign(n, {});
    g[0].push_back(1);
    g[1].push_back(0);
    g[0].push_back(2);
    g[2].push_back(0);
    g[1].push_back(3);
    g[3].push_back(1);
    g[1].push_back(4);
    g[4].push_back(1);
    tin.assign(n, 0);
    tout.assign(n, 0);
    timer = 0;
    euler(0, -1);
    cout << tout[0] << "\n";  // -> 4
    return 0;
}
