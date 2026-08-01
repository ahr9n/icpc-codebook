// Stress: 2D BIT point-add + rectangle-sum vs a naive grid recomputed by hand.
int stress() {
    mt19937 rng(7);
    long long checks = 0;
    for (int rep = 0; rep < 400; rep++) {
        int rows = 1 + rng() % 8, cols = 1 + rng() % 8;
        BIT2D bit(rows, cols);
        vector<vector<long long>> grid(rows, vector<long long>(cols, 0));

        for (int step = 0; step < 30; step++) {
            int r = rng() % rows, c = rng() % cols;
            long long delta = (long long)(rng() % 401) - 200;
            bit.add(r, c, delta);
            grid[r][c] += delta;

            for (int r1 = 0; r1 < rows; r1++)
                for (int r2 = r1; r2 < rows; r2++)
                    for (int c1 = 0; c1 < cols; c1++)
                        for (int c2 = c1; c2 < cols; c2++) {
                            long long ref = 0;
                            for (int i = r1; i <= r2; i++)
                                for (int j = c1; j <= c2; j++) ref += grid[i][j];
                            if (bit.query(r1, c1, r2, c2) != ref) {
                                printf("FAIL rows=%d cols=%d r1=%d c1=%d r2=%d c2=%d\n", rows, cols,
                                       r1, c1, r2, c2);
                                return 1;
                            }
                            checks++;
                        }
        }
    }
    printf("fenwick-2d PASS %lld", checks);
    return 0;
}
