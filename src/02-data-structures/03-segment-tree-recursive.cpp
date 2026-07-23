/**
 * Recursive segment tree over a mergeable node struct. Point update + range query.
 * Change Node/merge for any associative (even non-commutative) info. O(log n)
 */
struct Node {
    long long val = 0;
};

struct SegTreeRec {
    int n;
    vector<Node> tree;

    SegTreeRec(int n) : n(n), tree(4 * n) {}

    Node merge(Node l, Node r) {
        return {l.val + r.val};
    }

    void update(int i, int s, int e, int pos, long long val) {
        if (s == e) {
            tree[i].val = val;
            return;
        }
        int mid = (s + e) / 2;
        if (pos <= mid)
            update(2 * i, s, mid, pos, val);
        else
            update(2 * i + 1, mid + 1, e, pos, val);
        tree[i] = merge(tree[2 * i], tree[2 * i + 1]);
    }

    Node query(int i, int s, int e, int l, int r) {
        if (r < s or e < l) return Node{};
        if (l <= s and e <= r) return tree[i];
        int mid = (s + e) / 2;
        return merge(query(2 * i, s, mid, l, r), query(2 * i + 1, mid + 1, e, l, r));
    }
};

/**
 * Example: build values 1..5, range-sum, then point-update and re-query.
 */
int main() {
    int n = 5;
    SegTreeRec st(n);
    for (int i = 0; i < n; i++) st.update(1, 0, n - 1, i, i + 1);
    cout << st.query(1, 0, n - 1, 1, 3).val << "\n";  // -> 9
    st.update(1, 0, n - 1, 2, 10);
    cout << st.query(1, 0, n - 1, 0, 4).val << "\n";  // -> 22
    return 0;
}
