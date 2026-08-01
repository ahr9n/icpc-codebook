/**
 * Knuth's optimization: speeds up the interval DP
 *   dp[i][j] = min over i <= k < j of dp[i][k] + dp[k+1][j] + cost(i, j)
 * from O(n^3) to O(n^2). Demo cost = weight of the merged range (prefix-sum),
 * the classic optimal-merge / "merge stones" cost.
 *
 * Precondition: cost must satisfy the quadrangle inequality
 *   cost(a, c) + cost(b, d) <= cost(a, d) + cost(b, c)   for a <= b <= c <= d
 * and be monotone on intervals (cost(b, c) <= cost(a, d) when [b,c] subset [a,d]).
 * Non-negative prefix-sum range weights satisfy both. Under these, the best
 * split indices are monotone, shrinking the search windows to O(n^2) total.
 * Contract: weights are non-negative and all prefix sums and candidate costs
 * fit in long long. An empty sequence costs 0.
 */
long long knuth_merge_cost(const vector<long long>& weight) {
    int n = weight.size();
    if (n == 0) {
        return 0;
    }
    vector<long long> pre(n + 1, 0);
    for (int i = 0; i < n; i++) {
        pre[i + 1] = pre[i] + weight[i];
    }

    vector<vector<long long>> dp(n, vector<long long>(n, 0));
    vector<vector<int>> opt(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++) {
        opt[i][i] = i;
    }

    for (int i = n - 1; i >= 0; i--) {
        for (int j = i + 1; j < n; j++) {
            dp[i][j] = LINF;
            int lo = opt[i][j - 1];
            int hi = min(j - 1, opt[i + 1][j]);
            for (int k = lo; k <= hi; k++) {
                long long cost = dp[i][k] + dp[k + 1][j] + pre[j + 1] - pre[i];
                if (cost < dp[i][j]) {
                    dp[i][j] = cost;
                    opt[i][j] = k;
                }
            }
        }
    }
    return dp[0][n - 1];
}

/** Example: optimally merge four adjacent piles with total cost 190. */
int main() {
    vector<long long> weight = {10, 20, 30, 40};
    cout << knuth_merge_cost(weight) << "\n";  // -> 190
    return 0;
}
