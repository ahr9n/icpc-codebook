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
