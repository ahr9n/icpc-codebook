/**
 * Segment tree (iterative), point update + range query on [l, r].
 * merge = the operation, 0 = its neutral (min: use LINF, max: -LINF). O(log n)
 */
struct SegTree {
    int n;
    vector<long long> tree;

    SegTree(int n) : n(n), tree(2 * n, 0) {}

    long long merge(long long a, long long b) {
        return a + b;
    }

    void update(int i, long long val) {
        tree[i += n] = val;
        while (i > 1) {
            i >>= 1;
            tree[i] = merge(tree[2 * i], tree[2 * i + 1]);
        }
    }

    long long query(int l, int r) {
        long long res = 0;
        for (l += n, r += n + 1; l < r; l >>= 1, r >>= 1) {
            if (l & 1) res = merge(res, tree[l++]);
            if (r & 1) res = merge(res, tree[--r]);
        }
        return res;
    }
};

/**
 * Example: build values 1..5, range-sum, then point-update and re-query.
 */
int main() {
    SegTree st(5);
    for (int i = 0; i < 5; i++) st.update(i, i + 1);
    cout << st.query(1, 3) << "\n";  // -> 9
    st.update(2, 10);
    cout << st.query(0, 4) << "\n";  // -> 22
    return 0;
}
