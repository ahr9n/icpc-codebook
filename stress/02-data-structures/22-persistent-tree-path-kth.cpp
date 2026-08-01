// Stress: persistent four-root tree-path kth vs explicitly collecting and sorting
// every queried path in random rooted trees.
int stress() {
    mt19937 rng(22);
    long long checks = 0;

    for (int rep = 0; rep < 4000; rep++) {
        int n = 1 + rng() % 30;
        vector<long long> values(n);
        for (long long& value: values) {
            value = (long long)(rng() % 21) - 10;
        }

        vector<vector<int>> adj(n);
        PersistentTreePathKth tree(values);
        for (int v = 1; v < n; v++) {
            int u = rng() % v;
            adj[u].push_back(v);
            adj[v].push_back(u);
            tree.add_edge(u, v);
        }
        int root = rng() % n;
        tree.build(root);

        vector<int> parent(n, -1), depth(n);
        function<void(int, int)> dfs = [&](int u, int p) {
            parent[u] = p;
            for (int v: adj[u]) {
                if (v == p) {
                    continue;
                }
                depth[v] = depth[u] + 1;
                dfs(v, u);
            }
        };
        dfs(root, -1);

        for (int query = 0; query < 30; query++) {
            int u = rng() % n;
            int v = rng() % n;
            int a = u;
            int b = v;
            vector<long long> path;
            while (depth[a] > depth[b]) {
                path.push_back(values[a]);
                a = parent[a];
            }
            while (depth[b] > depth[a]) {
                path.push_back(values[b]);
                b = parent[b];
            }
            while (a != b) {
                path.push_back(values[a]);
                path.push_back(values[b]);
                a = parent[a];
                b = parent[b];
            }
            path.push_back(values[a]);
            sort(path.begin(), path.end());

            for (int k = 1; k <= (int)path.size(); k++) {
                long long got = tree.kth(u, v, k);
                if (got != path[k - 1]) {
                    printf("persistent-tree-path-kth FAIL rep=%d u=%d v=%d k=%d got=%lld "
                           "want=%lld\n",
                           rep, u, v, k, got, path[k - 1]);
                    return 1;
                }
                checks++;
            }
        }
    }

    printf("persistent-tree-path-kth PASS %lld", checks);
    return 0;
}
