/**
 * Matrix-chain multiplication: least scalar multiplications to multiply a chain
 * of matrices, where matrix i has shape dims[i-1] x dims[i]. Interval DP over
 * sub-chains [i, j]: try every split point k, paying dp[i][k] + dp[k+1][j] plus
 * dims[i-1] * dims[k] * dims[j] to combine the two halves. Only the shapes
 * matter, never the entries. dims has n+1 entries for n matrices. O(n^3).
 */
long long min_mult_cost(vector<long long>& dims) {
    int n = dims.size() - 1;
    vector<vector<long long>> dp(n + 1, vector<long long>(n + 1, 0));
    for (int len = 2; len <= n; len++)
        for (int i = 1; i + len - 1 <= n; i++) {
            int j = i + len - 1;
            dp[i][j] = LLONG_MAX;
            for (int k = i; k < j; k++) {
                long long cost = dp[i][k] + dp[k + 1][j] + dims[i - 1] * dims[k] * dims[j];
                dp[i][j] = min(dp[i][j], cost);
            }
        }
    return dp[1][n];
}

/**
 * Example: four matrices with shapes 40x20, 20x30, 30x10, 10x30.
 */
int main() {
    vector<long long> dims = {40, 20, 30, 10, 30};
    cout << min_mult_cost(dims) << "\n";  // -> 26000
    return 0;
}
