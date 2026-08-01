/**
 * Merge-sort (merge-sort tree) segment tree over a static array. Each node keeps
 * its range's values in sorted order, so a count of elements <= v within index
 * range [l, r] is O(log^2 n): O(log n) covering nodes, each an upper_bound.
 * count_in returns how many indices in [l, r] hold a value in [lo, hi].
 * Build is O(n log n); the array is fixed after construction (no point update).
 * For the k-th smallest in [l, r]: binary-search the value v with
 * count_leq(l, r, v) >= k -- one more log factor, O(log^2 n * log(max_val)).
 */
struct MergeSortTree {
    int n;
    vector<vector<int>> tree;

    MergeSortTree(const vector<int>& values) : n(values.size()), tree(4 * n) {
        build(1, 0, n - 1, values);
    }

    void build(int i, int lo, int hi, const vector<int>& values) {
        if (lo == hi) {
            tree[i] = {values[lo]};
            return;
        }
        int mid = (lo + hi) / 2;
        build(2 * i, lo, mid, values);
        build(2 * i + 1, mid + 1, hi, values);
        merge(tree[2 * i].begin(), tree[2 * i].end(), tree[2 * i + 1].begin(),
              tree[2 * i + 1].end(), back_inserter(tree[i]));
    }

    long long count_leq(int i, int lo, int hi, int l, int r, int v) {
        if (r < lo or hi < l) return 0;
        if (l <= lo and hi <= r) {
            return upper_bound(tree[i].begin(), tree[i].end(), v) - tree[i].begin();
        }
        int mid = (lo + hi) / 2;
        return count_leq(2 * i, lo, mid, l, r, v) + count_leq(2 * i + 1, mid + 1, hi, l, r, v);
    }

    long long count_leq(int l, int r, int v) {
        return count_leq(1, 0, n - 1, l, r, v);
    }

    long long count_in(int l, int r, int lo, int hi) {
        return count_leq(l, r, hi) - count_leq(l, r, lo - 1);
    }
};

/**
 * Example: values with duplicates and gaps; count values <= v and within [lo,hi].
 */
int main() {
    vector<int> values = {5, 2, 8, 2, 6, 1, 9, 3};
    MergeSortTree mst(values);

    cout << mst.count_leq(0, 7, 5) << "\n";    // -> 5
    cout << mst.count_leq(2, 5, 6) << "\n";    // -> 3
    cout << mst.count_in(0, 7, 2, 6) << "\n";  // -> 5
    cout << mst.count_in(1, 4, 3, 9) << "\n";  // -> 2
    return 0;
}
