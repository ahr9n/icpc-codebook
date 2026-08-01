/**
 * Merge-sort (merge-sort tree) segment tree over a static array. Each node keeps
 * its range's values in sorted order, so a count of elements <= v within index
 * range [l, r] is O(log^2 n): O(log n) covering nodes, each an upper_bound.
 * count_in returns how many indices in [l, r] hold a value in [lo, hi].
 * Build is O(n log n); the array is fixed after construction (no point update).
 * kth binary-searches the sorted value domain using count_leq, taking O(log^3 n).
 * Contract: values is non-empty, ranges are valid and inclusive, and kth uses
 * 1 <= k <= r - l + 1.
 */
struct MergeSortTree {
    int n;
    vector<vector<int>> tree;
    vector<int> sorted_values;

    MergeSortTree(const vector<int>& values)
        : n(values.size()), tree(4 * n), sorted_values(values) {
        sort(sorted_values.begin(), sorted_values.end());
        sorted_values.erase(unique(sorted_values.begin(), sorted_values.end()),
                            sorted_values.end());
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
        if (r < lo or hi < l) {
            return 0;
        }
        if (l <= lo and hi <= r) {
            return upper_bound(tree[i].begin(), tree[i].end(), v) - tree[i].begin();
        }
        int mid = (lo + hi) / 2;
        return count_leq(2 * i, lo, mid, l, r, v) + count_leq(2 * i + 1, mid + 1, hi, l, r, v);
    }

    long long count_leq(int l, int r, int v) {
        return count_leq(1, 0, n - 1, l, r, v);
    }

    long long count_lt(int i, int lo, int hi, int l, int r, int v) {
        if (r < lo or hi < l) {
            return 0;
        }
        if (l <= lo and hi <= r) {
            return lower_bound(tree[i].begin(), tree[i].end(), v) - tree[i].begin();
        }
        int mid = (lo + hi) / 2;
        return count_lt(2 * i, lo, mid, l, r, v) + count_lt(2 * i + 1, mid + 1, hi, l, r, v);
    }

    long long count_lt(int l, int r, int v) {
        return count_lt(1, 0, n - 1, l, r, v);
    }

    long long count_in(int l, int r, int lo, int hi) {
        return count_leq(l, r, hi) - count_lt(l, r, lo);
    }

    int kth(int l, int r, int k) {
        int lo = 0;
        int hi = sorted_values.size() - 1;
        while (lo < hi) {
            int mid = (lo + hi) / 2;
            if (count_leq(l, r, sorted_values[mid]) >= k) {
                hi = mid;
            } else {
                lo = mid + 1;
            }
        }
        return sorted_values[lo];
    }
};

/** Example: threshold, interval, and k-th-smallest queries. */
int main() {
    vector<int> values = {5, 2, 8, 2, 6, 1, 9, 3};
    MergeSortTree mst(values);

    cout << mst.count_leq(0, 7, 5) << "\n";
    cout << mst.count_leq(2, 5, 6) << "\n";
    cout << mst.count_in(0, 7, 2, 6) << "\n";
    cout << mst.count_in(1, 4, 3, 9) << "\n";
    cout << mst.kth(1, 6, 4) << "\n";
    return 0;
}
// -> 5
// -> 3
// -> 5
// -> 2
// -> 6
