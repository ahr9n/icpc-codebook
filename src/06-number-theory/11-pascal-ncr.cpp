/**
 * nCr via Pascal's triangle (additive, non-modular). C[i][j] = C[i-1][j-1] + C[i-1][j].
 * Good for small n where exact big values fit. O(n^2) build.
 */
vector<vector<long long>> pascal(int n) {
    vector<vector<long long>> c(n + 1, vector<long long>(n + 1, 0));
    for (int i = 0; i <= n; i++) {
        c[i][0] = 1;
        for (int j = 1; j <= i; j++) c[i][j] = c[i - 1][j - 1] + c[i - 1][j];
    }
    return c;
}
