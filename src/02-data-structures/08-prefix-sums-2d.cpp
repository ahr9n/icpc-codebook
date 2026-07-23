/**
 * 2D prefix sums: build once, then any submatrix sum in O(1) by inclusion-exclusion.
 * pre[i+1][j+1] = a[i][j] + top + left - corner. O(nm) build.
 */
struct Prefix2D {
    vector<vector<long long>> pre;

    Prefix2D(vector<vector<long long>>& a) {
        int rows = a.size(), cols = a[0].size();
        pre.assign(rows + 1, vector<long long>(cols + 1, 0));
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                pre[i + 1][j + 1] = a[i][j] + pre[i][j + 1] + pre[i + 1][j] - pre[i][j];
    }

    long long query(int r1, int c1, int r2, int c2) {
        return pre[r2 + 1][c2 + 1] - pre[r1][c2 + 1] - pre[r2 + 1][c1] + pre[r1][c1];
    }
};

/**
 * Example: build over a 3x3 grid, then sum two submatrices in O(1).
 */
int main() {
    vector<vector<long long>> a = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    Prefix2D p(a);
    cout << p.query(0, 0, 1, 1) << "\n";  // -> 12
    cout << p.query(1, 1, 2, 2) << "\n";  // -> 28
    return 0;
}
