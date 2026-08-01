struct GridPath {
    long long sum;
    vector<pair<int, int>> cells;
};

/**
 * DP path reconstruction pattern on a weighted grid. Finds a maximum-sum path
 * from the top-left to the bottom-right using only down/right moves, and stores
 * the predecessor chosen for every state so the optimal path can be recovered.
 * O(rows*cols) time and memory. Contract: grid is non-empty and rectangular,
 * and every candidate path sum fits in long long. Ties prefer a move from above.
 */
GridPath max_grid_path(const vector<vector<long long>>& grid) {
    int rows = grid.size(), cols = grid[0].size();
    vector<vector<long long>> dp(rows, vector<long long>(cols, 0));
    vector<vector<pair<int, int>>> parent(rows, vector<pair<int, int>>(cols, {-1, -1}));
    dp[0][0] = grid[0][0];

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            if (row == 0 and col == 0) {
                continue;
            }
            if (col == 0 or (row > 0 and dp[row - 1][col] >= dp[row][col - 1])) {
                dp[row][col] = dp[row - 1][col] + grid[row][col];
                parent[row][col] = {row - 1, col};
            } else {
                dp[row][col] = dp[row][col - 1] + grid[row][col];
                parent[row][col] = {row, col - 1};
            }
        }
    }

    vector<pair<int, int>> cells;
    pair<int, int> at = {rows - 1, cols - 1};
    while (at.first != -1) {
        cells.push_back(at);
        at = parent[at.first][at.second];
    }
    reverse(cells.begin(), cells.end());
    return {dp[rows - 1][cols - 1], cells};
}

/** Example: recover a maximum-sum down/right path through a 3x4 grid. */
int main() {
    vector<vector<long long>> grid = {
        {5, 1, 2, 1},
        {2, 10, -5, 2},
        {1, 2, 20, 3},
    };
    GridPath path = max_grid_path(grid);
    cout << path.sum << "\n";  // -> 42
    for (auto [row, col]: path.cells) {
        cout << "(" << row << "," << col << ") ";
    }
    cout << "\n";  // -> (0,0) (1,0) (1,1) (2,1) (2,2) (2,3)
    return 0;
}
