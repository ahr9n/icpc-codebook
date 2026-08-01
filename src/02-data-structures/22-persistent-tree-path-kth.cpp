/**
 * Persistent segment tree for k-th-smallest vertex value on a tree path. Each
 * vertex owns a histogram version for the root-to-vertex path. For query u-v,
 * combining versions u + v - lca - parent(lca) leaves exactly the path's value
 * multiset; descending the four roots by their left-count difference finds the
 * k-th value. Binary lifting supplies the LCA.
 *
 * Build takes O(n log n) time and memory after coordinate compression; lca and
 * kth each take O(log n). Contract: values is non-empty; add n-1 edges forming a
 * connected tree, call build(root) once, and query 1 <= k <= the number of
 * vertices on path u-v.
 */
struct PersistentTreePathKth {
    vector<vector<int>> adj, up;
    vector<int> depth, parent, roots, value_position;
    vector<int> left_child, right_child, subtree_count;
    vector<long long> sorted_values;
    int levels;

    PersistentTreePathKth(const vector<long long>& values)
        : adj(values.size()), depth(values.size()), parent(values.size()), roots(values.size()),
          value_position(values.size()), sorted_values(values) {
        sort(sorted_values.begin(), sorted_values.end());
        sorted_values.erase(unique(sorted_values.begin(), sorted_values.end()),
                            sorted_values.end());
        for (int i = 0; i < (int)values.size(); i++) {
            value_position[i] = lower_bound(sorted_values.begin(), sorted_values.end(), values[i]) -
                                sorted_values.begin();
        }
        levels = 1;
        while ((1 << levels) <= (int)values.size()) {
            levels++;
        }
        up.assign(levels, vector<int>(values.size()));
        new_node(0, 0, 0);
    }

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    int new_node(int left, int right, int count) {
        left_child.push_back(left);
        right_child.push_back(right);
        subtree_count.push_back(count);
        return (int)subtree_count.size() - 1;
    }

    int insert(int previous, int lo, int hi, int position) {
        if (lo == hi) {
            return new_node(0, 0, subtree_count[previous] + 1);
        }
        int mid = (lo + hi) / 2;
        int left = left_child[previous];
        int right = right_child[previous];
        if (position <= mid) {
            left = insert(left, lo, mid, position);
        } else {
            right = insert(right, mid + 1, hi, position);
        }
        return new_node(left, right, subtree_count[left] + subtree_count[right]);
    }

    void dfs(int u, int p) {
        parent[u] = p;
        up[0][u] = (p == -1 ? u : p);
        for (int level = 1; level < levels; level++) {
            up[level][u] = up[level - 1][up[level - 1][u]];
        }
        int previous_root = (p == -1 ? 0 : roots[p]);
        roots[u] = insert(previous_root, 0, (int)sorted_values.size() - 1, value_position[u]);

        for (int v: adj[u]) {
            if (v == p) {
                continue;
            }
            depth[v] = depth[u] + 1;
            dfs(v, u);
        }
    }

    void build(int root) {
        depth[root] = 0;
        dfs(root, -1);
    }

    int lca(int u, int v) {
        if (depth[u] < depth[v]) {
            swap(u, v);
        }
        int difference = depth[u] - depth[v];
        for (int level = 0; level < levels; level++) {
            if (difference & (1 << level)) {
                u = up[level][u];
            }
        }
        if (u == v) {
            return u;
        }
        for (int level = levels - 1; level >= 0; level--) {
            if (up[level][u] != up[level][v]) {
                u = up[level][u];
                v = up[level][v];
            }
        }
        return parent[u];
    }

    long long kth(int u, int v, int k) {
        int ancestor = lca(u, v);
        int ancestor_parent_root = (parent[ancestor] == -1 ? 0 : roots[parent[ancestor]]);
        int root_u = roots[u];
        int root_v = roots[v];
        int root_ancestor = roots[ancestor];
        int lo = 0;
        int hi = (int)sorted_values.size() - 1;

        while (lo < hi) {
            int left_count = subtree_count[left_child[root_u]] + subtree_count[left_child[root_v]] -
                             subtree_count[left_child[root_ancestor]] -
                             subtree_count[left_child[ancestor_parent_root]];
            int mid = (lo + hi) / 2;
            if (k <= left_count) {
                root_u = left_child[root_u];
                root_v = left_child[root_v];
                root_ancestor = left_child[root_ancestor];
                ancestor_parent_root = left_child[ancestor_parent_root];
                hi = mid;
            } else {
                k -= left_count;
                root_u = right_child[root_u];
                root_v = right_child[root_v];
                root_ancestor = right_child[root_ancestor];
                ancestor_parent_root = right_child[ancestor_parent_root];
                lo = mid + 1;
            }
        }
        return sorted_values[lo];
    }
};

/** Example: three k-th-smallest queries across branches of a five-node tree. */
int main() {
    PersistentTreePathKth tree({5, 1, 7, 3, 6});
    tree.add_edge(0, 1);
    tree.add_edge(0, 2);
    tree.add_edge(1, 3);
    tree.add_edge(1, 4);
    tree.build(0);

    cout << tree.kth(3, 2, 3) << "\n";
    cout << tree.kth(4, 2, 2) << "\n";
    cout << tree.kth(3, 4, 3) << "\n";
    return 0;
}
// -> 5
// -> 5
// -> 6
