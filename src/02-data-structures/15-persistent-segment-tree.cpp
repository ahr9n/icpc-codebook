/**
 * Persistent segment tree — k-th smallest value in a range [l, r]. Build one
 * immutable version per array prefix over the coordinate-compressed value axis:
 * version[i] adds +1 at a[i]'s compressed slot on top of version[i-1], copying
 * only the O(log n) nodes on the touched path so every older version stays a
 * valid root (the "version array" that answers historic queries online). The
 * value histogram of a[l..r] is version[r] minus version[l-1], so descend both
 * roots together, steering by the left-subtree count difference, to reach the
 * k-th smallest. Build O(n log n), each query O(log n).
 * Queries are 1-indexed: 1 <= l <= r <= n and 1 <= k <= r - l + 1.
 */
struct PersistentSegTree {
    vector<int> left_child, right_child, subtree_count;
    vector<int> roots;
    vector<long long> sorted_values;

    int new_node(int lc, int rc, int cnt) {
        left_child.push_back(lc);
        right_child.push_back(rc);
        subtree_count.push_back(cnt);
        return (int)subtree_count.size() - 1;
    }

    int insert(int prev, int lo, int hi, int pos) {
        if (lo == hi) {
            return new_node(0, 0, subtree_count[prev] + 1);
        }
        int mid = (lo + hi) / 2;
        int lc = left_child[prev], rc = right_child[prev];
        if (pos <= mid) {
            lc = insert(lc, lo, mid, pos);
        } else {
            rc = insert(rc, mid + 1, hi, pos);
        }
        return new_node(lc, rc, subtree_count[lc] + subtree_count[rc]);
    }

    PersistentSegTree(const vector<long long>& a) {
        sorted_values = a;
        sort(sorted_values.begin(), sorted_values.end());
        sorted_values.erase(unique(sorted_values.begin(), sorted_values.end()),
                            sorted_values.end());
        int m = sorted_values.size();

        // Node 0 is the shared empty node: it is its own child, so descending
        // past a leaf or into an untouched branch keeps counting zero.
        new_node(0, 0, 0);
        roots.push_back(0);

        for (auto x: a) {
            int pos =
                lower_bound(sorted_values.begin(), sorted_values.end(), x) - sorted_values.begin();
            roots.push_back(insert(roots.back(), 0, m - 1, pos));
        }
    }

    long long query(int l, int r, int k) {
        int lo = 0, hi = (int)sorted_values.size() - 1;
        int old_root = roots[l - 1], new_root = roots[r];
        while (lo < hi) {
            int mid = (lo + hi) / 2;
            int left_cnt =
                subtree_count[left_child[new_root]] - subtree_count[left_child[old_root]];
            if (k <= left_cnt) {
                old_root = left_child[old_root];
                new_root = left_child[new_root];
                hi = mid;
            } else {
                k -= left_cnt;
                old_root = right_child[old_root];
                new_root = right_child[new_root];
                lo = mid + 1;
            }
        }
        return sorted_values[lo];
    }
};

/** Example: three k-th-smallest queries on different array ranges. */
int main() {
    vector<long long> a = {5, 2, 6, 2, 9, 3, 7};
    PersistentSegTree pst(a);

    cout << pst.query(2, 5, 3) << '\n';
    cout << pst.query(1, 7, 1) << '\n';
    cout << pst.query(3, 6, 2) << '\n';

    return 0;
}
// -> 6
// -> 2
// -> 3
