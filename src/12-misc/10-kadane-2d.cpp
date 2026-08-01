/**
 * Kadane in 2D: max sum over every axis-aligned submatrix. Fix a top/bottom row
 * pair, collapse the strip between them into per-column sums, then run 1D Kadane
 * (see 01-kadane) on that array. O(R^2 * C). All-negative input yields the
 * least-negative single cell (never 0), matching the 1D convention.
 */
long long max_submatrix(const vector<vector<long long>>& grid) {
    int rows = grid.size(), cols = grid[0].size();
    long long best = grid[0][0];
    for (int top = 0; top < rows; top++) {
        vector<long long> col_sum(cols, 0);
        for (int bottom = top; bottom < rows; bottom++) {
            long long cur = 0;
            for (int c = 0; c < cols; c++) {
                col_sum[c] += grid[bottom][c];
                cur = max(col_sum[c], cur + col_sum[c]);
                best = max(best, cur);
            }
        }
    }
    return best;
}

int main() {
    vector<vector<long long>> grid = {
        {1, 2, -1, -4, -20},
        {-8, -3, 4, 2, 1},
        {3, 8, 10, 1, 3},
        {-4, -1, 1, 7, -6},
    };
    cout << max_submatrix(grid) << "\n";  // -> 29
    return 0;
}
