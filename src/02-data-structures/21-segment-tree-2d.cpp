/**
 * 2D segment tree (tree of trees): a segment tree over rows, each node holding a
 * segment tree over columns. Point update of one cell + rectangle fold over
 * [r1, r2] x [c1, c2]. O(log rows * log cols) per op, O(rows * cols) memory.
 *
 * merge = the operation, its neutral seeds every fold (sum: 0; max: -LINF, and
 * init res to -LINF). This is the point-update variant only: full 2D lazy
 * range-update needs a much heavier structure and is out of scope.
 */
struct SegTree2D {
    int rows, cols;
    vector<vector<long long>> tree;

    SegTree2D(int rows, int cols)
        : rows(rows), cols(cols), tree(2 * rows, vector<long long>(2 * cols, 0)) {}

    long long merge(long long a, long long b) {
        return a + b;
    }

    void update(int r, int c, long long val) {
        int row_leaf = r + rows, col_leaf = c + cols;
        tree[row_leaf][col_leaf] = val;
        for (int j = col_leaf; j > 1; j >>= 1) {
            tree[row_leaf][j >> 1] = merge(tree[row_leaf][j], tree[row_leaf][j ^ 1]);
        }
        for (int i = row_leaf >> 1; i >= 1; i >>= 1) {
            for (int j = col_leaf; j >= 1; j >>= 1) {
                tree[i][j] = merge(tree[2 * i][j], tree[2 * i + 1][j]);
            }
        }
    }

    long long query_cols(int row_node, int c1, int c2) {
        long long res = 0;
        for (int l = c1 + cols, r = c2 + cols + 1; l < r; l >>= 1, r >>= 1) {
            if (l & 1) {
                res = merge(res, tree[row_node][l++]);
            }
            if (r & 1) {
                res = merge(res, tree[row_node][--r]);
            }
        }
        return res;
    }

    long long query(int r1, int c1, int r2, int c2) {
        long long res = 0;
        for (int l = r1 + rows, r = r2 + rows + 1; l < r; l >>= 1, r >>= 1) {
            if (l & 1) {
                res = merge(res, query_cols(l++, c1, c2));
            }
            if (r & 1) {
                res = merge(res, query_cols(--r, c1, c2));
            }
        }
        return res;
    }
};

/** Example: point-set a grid, then query two rectangles and the whole grid. */
int main() {
    SegTree2D st(3, 4);
    long long grid[3][4] = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}};
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            st.update(i, j, grid[i][j]);
        }
    }
    cout << st.query(0, 1, 1, 2) << "\n";
    cout << st.query(2, 3, 2, 3) << "\n";
    cout << st.query(0, 0, 2, 3) << "\n";
    return 0;
}
// -> 18
// -> 12
// -> 78
