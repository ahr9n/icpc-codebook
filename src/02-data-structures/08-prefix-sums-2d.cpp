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
