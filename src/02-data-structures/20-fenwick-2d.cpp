/**
 * 2D Fenwick / BIT: point update + rectangle sum on an R x C grid in
 * O(log R * log C). External indices are 0-based; the tree is 1-indexed
 * internally, hence the ++ / -- on the boundary. query is inclusion-exclusion
 * over prefix(r, c) = sum of the box [(0,0), (r,c)].
 */
struct BIT2D {
    int rows, cols;
    vector<vector<long long>> tree;

    BIT2D(int rows, int cols)
        : rows(rows), cols(cols), tree(rows + 1, vector<long long>(cols + 1, 0)) {}

    void add(int r, int c, long long val) {
        for (int i = r + 1; i <= rows; i += i & -i) {
            for (int j = c + 1; j <= cols; j += j & -j) {
                tree[i][j] += val;
            }
        }
    }

    long long prefix(int r, int c) {
        long long sum = 0;
        for (int i = r + 1; i > 0; i -= i & -i) {
            for (int j = c + 1; j > 0; j -= j & -j) {
                sum += tree[i][j];
            }
        }
        return sum;
    }

    long long query(int r1, int c1, int r2, int c2) {
        return prefix(r2, c2) - prefix(r1 - 1, c2) - prefix(r2, c1 - 1) + prefix(r1 - 1, c1 - 1);
    }
};

/** Example: point additions followed by two rectangle-sum queries. */
int main() {
    BIT2D bit(3, 4);
    bit.add(0, 0, 5);
    bit.add(1, 2, 3);
    bit.add(2, 3, 7);
    bit.add(1, 1, 2);
    cout << bit.query(0, 0, 1, 2) << "\n";
    cout << bit.query(1, 1, 2, 3) << "\n";
    return 0;
}
// -> 10
// -> 12
