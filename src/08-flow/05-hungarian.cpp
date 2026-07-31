/**
 * Hungarian algorithm (Kuhn-Munkres) for the assignment problem: given an n x n
 * cost matrix, pick exactly one cell per row and per column so the total cost is
 * minimum -- a min-cost perfect matching in a complete bipartite graph. Keeps
 * dual potentials u, v and, for each new row, grows a shortest augmenting path
 * (Dijkstra-like on reduced costs) that never breaks the potentials' feasibility.
 * Costs may be negative. Returns the minimum total cost; on return match_col[r]
 * is the column assigned to row r. Internals are 1-indexed, the interface 0-indexed.
 * O(n^3).
 */
long long hungarian(const vector<vector<long long>>& cost, vector<int>& match_col) {
    const long long INF = 1e18;
    int n = cost.size();
    vector<long long> u(n + 1, 0), v(n + 1, 0);
    vector<int> p(n + 1, 0), way(n + 1, 0);
    for (int i = 1; i <= n; i++) {
        p[0] = i;
        int j0 = 0;
        vector<long long> min_slack(n + 1, INF);
        vector<bool> used(n + 1, false);
        do {
            used[j0] = true;
            int i0 = p[j0], next_j = -1;
            long long delta = INF;
            for (int j = 1; j <= n; j++) {
                if (used[j]) continue;
                long long slack = cost[i0 - 1][j - 1] - u[i0] - v[j];
                if (slack < min_slack[j]) {
                    min_slack[j] = slack;
                    way[j] = j0;
                }
                if (min_slack[j] < delta) {
                    delta = min_slack[j];
                    next_j = j;
                }
            }
            for (int j = 0; j <= n; j++) {
                if (used[j]) {
                    u[p[j]] += delta;
                    v[j] -= delta;
                } else {
                    min_slack[j] -= delta;
                }
            }
            j0 = next_j;
        } while (p[j0] != 0);
        do {
            int prev_j = way[j0];
            p[j0] = p[prev_j];
            j0 = prev_j;
        } while (j0 != 0);
    }
    match_col.assign(n, 0);
    for (int j = 1; j <= n; j++) match_col[p[j] - 1] = j - 1;
    return -v[0];
}

/**
 * Example: cost {{4,1,3},{2,0,5},{3,2,2}}. The optimal assignment picks
 * (row0->col1)=1, (row1->col0)=2, (row2->col2)=2 for total 5.
 */
int main() {
    vector<vector<long long>> cost = {{4, 1, 3}, {2, 0, 5}, {3, 2, 2}};
    vector<int> match_col;
    long long best = hungarian(cost, match_col);
    cout << best << "\n";  // -> 5
    for (int col: match_col) cout << col << " ";
    cout << "\n";  // -> 1 0 2
    return 0;
}
