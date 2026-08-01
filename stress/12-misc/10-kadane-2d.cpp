// Stress: 2D Kadane vs naive O(R^2 C^2) sum over every rectangle. Random small
// matrices covering all-negative, single row, single column, 1x1, mixed signs.
int stress() {
    mt19937 rng(11);
    long long checks = 0;
    for (int rep = 0; rep < 4000; rep++) {
        int rows = 1 + rng() % 6, cols = 1 + rng() % 6;
        int lo = -9, span = 19;
        if (rep % 5 == 0) {
            lo = -9;
            span = 9;  // all-negative regime
        }
        vector<vector<long long>> grid(rows, vector<long long>(cols));
        for (auto& row: grid)
            for (auto& x: row) x = lo + (long long)(rng() % span);

        long long ref = grid[0][0];
        for (int t = 0; t < rows; t++)
            for (int b = t; b < rows; b++)
                for (int l = 0; l < cols; l++)
                    for (int r = l; r < cols; r++) {
                        long long sum = 0;
                        for (int i = t; i <= b; i++)
                            for (int j = l; j <= r; j++) sum += grid[i][j];
                        ref = max(ref, sum);
                    }

        long long got = max_submatrix(grid);
        if (got != ref) {
            printf("FAIL rows=%d cols=%d got=%lld ref=%lld\n", rows, cols, got, ref);
            return 1;
        }
        checks++;
    }
    printf("kadane-2d PASS %lld", checks);
    return 0;
}
