/**
 * Lazy-propagation segment tree: range-add update + range-sum query. O(log n)
 * Push pending add down before recursing. Build with n leaves, indices [0, n-1].
 */
struct LazySeg {
    int n;
    vector<long long> tree, lazy;

    LazySeg(int n) : n(n), tree(4 * n, 0), lazy(4 * n, 0) {}

    void push(int i, int lo, int hi) {
        if (lazy[i] == 0) return;
        tree[i] += lazy[i] * (hi - lo + 1);
        if (lo != hi) {
            lazy[2 * i] += lazy[i];
            lazy[2 * i + 1] += lazy[i];
        }
        lazy[i] = 0;
    }

    void update(int i, int lo, int hi, int l, int r, long long val) {
        push(i, lo, hi);
        if (r < lo or hi < l) return;
        if (l <= lo and hi <= r) {
            lazy[i] += val;
            push(i, lo, hi);
            return;
        }
        int mid = (lo + hi) / 2;
        update(2 * i, lo, mid, l, r, val);
        update(2 * i + 1, mid + 1, hi, l, r, val);
        tree[i] = tree[2 * i] + tree[2 * i + 1];
    }

    long long query(int i, int lo, int hi, int l, int r) {
        push(i, lo, hi);
        if (r < lo or hi < l) return 0;
        if (l <= lo and hi <= r) return tree[i];
        int mid = (lo + hi) / 2;
        return query(2 * i, lo, mid, l, r) + query(2 * i + 1, mid + 1, hi, l, r);
    }
};

/**
 * Example: recursive segtrees take (node=1, s=0, e=sz-1, ...) at the top call.
 */
int main() {
    int sz = 5;

    SegTreeRec st(sz);
    st.update(1, 0, sz - 1, 2, 10);
    cout << st.query(1, 0, sz - 1, 0, 4).val << '\n';

    LazySeg ls(sz);
    ls.update(1, 0, sz - 1, 1, 3, 5);
    cout << ls.query(1, 0, sz - 1, 0, 4) << '\n';

    return 0;
}
