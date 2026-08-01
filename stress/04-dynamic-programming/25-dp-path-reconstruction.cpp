// Stress the returned score and path against exhaustive enumeration of all
// monotone paths on small random grids.
long long brute_path(const vector<vector<long long>>& grid, int row, int col) {
    int rows = grid.size(), cols = grid[0].size();
    if (row == rows - 1 and col == cols - 1) {
        return grid[row][col];
    }
    if (row + 1 == rows) {
        return grid[row][col] + brute_path(grid, row, col + 1);
    }
    if (col + 1 == cols) {
        return grid[row][col] + brute_path(grid, row + 1, col);
    }
    return grid[row][col] + max(brute_path(grid, row + 1, col), brute_path(grid, row, col + 1));
}

bool valid_path(const vector<vector<long long>>& grid, const GridPath& path) {
    int rows = grid.size(), cols = grid[0].size();
    if (path.cells.empty() or path.cells.front() != pair<int, int>{0, 0} or
        path.cells.back() != pair<int, int>{rows - 1, cols - 1}) {
        return false;
    }
    long long sum = 0;
    for (int i = 0; i < (int)path.cells.size(); i++) {
        auto [row, col] = path.cells[i];
        if (row < 0 or row >= rows or col < 0 or col >= cols) {
            return false;
        }
        sum += grid[row][col];
        if (i > 0) {
            int row_step = row - path.cells[i - 1].first;
            int col_step = col - path.cells[i - 1].second;
            if (row_step + col_step != 1 or row_step < 0 or col_step < 0) {
                return false;
            }
        }
    }
    return sum == path.sum;
}

int stress() {
    mt19937 rng(425);
    long long checks = 0;

    vector<vector<long long>> very_negative = {
        {-3000000000000000000LL, -3000000000000000000LL},
    };
    GridPath edge = max_grid_path(very_negative);
    if (edge.sum != -6000000000000000000LL or not valid_path(very_negative, edge)) {
        printf("FAIL very-negative\n");
        return 1;
    }
    checks++;

    for (int trial = 0; trial < 10000; trial++) {
        int rows = 1 + rng() % 6;
        int cols = 1 + rng() % 6;
        vector<vector<long long>> grid(rows, vector<long long>(cols));
        for (vector<long long>& values: grid) {
            for (long long& value: values) {
                value = (long long)(rng() % 31) - 15;
            }
        }
        GridPath path = max_grid_path(grid);
        long long expected = brute_path(grid, 0, 0);
        if (path.sum != expected or not valid_path(grid, path)) {
            printf("FAIL trial=%d rows=%d cols=%d got=%lld expected=%lld\n", trial, rows, cols,
                   path.sum, expected);
            return 1;
        }
        checks++;
    }
    printf("dp-path-reconstruction PASS %lld", checks);
    return 0;
}
