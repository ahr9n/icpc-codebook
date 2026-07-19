int n, m;
const int INF = 0x3f3f3f3f;
vector<vector<int>> grid;
bool is_valid(int i, int j) {
    return i >= 0 and j >= 0 and i < n and j < m;
}

/**
 * Min-effort grid path: Dijkstra where cost = MAX step, not sum. O(nm log nm)
 */
int min_bottleneck_grid() {
    n = grid.size(), m = grid[0].size();
    int dr[] = {1, -1, 0, 0}, dc[] = {0, 0, 1, -1};
    vector<vector<int>> effort(n, vector<int>(m, INF));

    struct Cell {
        int cost, r, c;

        bool operator<(const Cell& o) const {
            return cost > o.cost;
        }
    };
    priority_queue<Cell> pq;

    effort[0][0] = 0;
    pq.push({0, 0, 0});

    while (not pq.empty()) {
        Cell cur = pq.top();
        pq.pop();
        if (cur.r == n - 1 and cur.c == m - 1) return cur.cost;
        if (cur.cost > effort[cur.r][cur.c]) continue;

        for (int k = 0; k < 4; k++) {
            int nr = cur.r + dr[k], nc = cur.c + dc[k];
            if (not is_valid(nr, nc)) continue;

            int nd = max(cur.cost, abs(grid[cur.r][cur.c] - grid[nr][nc]));
            if (nd < effort[nr][nc]) {
                effort[nr][nc] = nd;
                pq.push({nd, nr, nc});
            }
        }
    }
    return 0;
}
