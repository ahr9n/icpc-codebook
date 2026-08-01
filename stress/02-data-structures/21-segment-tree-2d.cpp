// Stress: 2D segtree point-set + rectangle-sum vs a naive grid recomputed by hand.
int stress() {
    mt19937 rng(11);
    long long checks = 0;
    for (int rep = 0; rep < 400; rep++) {
        int rows = 1 + rng() % 8, cols = 1 + rng() % 8;
        SegTree2D st(rows, cols);
        vector<vector<long long>> grid(rows, vector<long long>(cols, 0));

        for (int step = 0; step < 30; step++) {
            int r = rng() % rows, c = rng() % cols;
            long long cell = (long long)(rng() % 401) - 200;
            st.update(r, c, cell);
            grid[r][c] = cell;

            for (int r1 = 0; r1 < rows; r1++)
                for (int r2 = r1; r2 < rows; r2++)
                    for (int c1 = 0; c1 < cols; c1++)
                        for (int c2 = c1; c2 < cols; c2++) {
                            long long ref = 0;
                            for (int i = r1; i <= r2; i++)
                                for (int j = c1; j <= c2; j++) ref += grid[i][j];
                            if (st.query(r1, c1, r2, c2) != ref) {
                                printf("FAIL rows=%d cols=%d r1=%d c1=%d r2=%d c2=%d\n", rows, cols,
                                       r1, c1, r2, c2);
                                return 1;
                            }
                            checks++;
                        }
        }
    }
    printf("segment-tree-2d PASS %lld", checks);
    return 0;
}
