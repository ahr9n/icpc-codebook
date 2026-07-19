const long long MOD = 1e9 + 7;

/**
 * Matrix multiply under MOD. k x k square matrices. O(k^3)
 */
vector<vector<long long>> mat_mul(vector<vector<long long>>& a, vector<vector<long long>>& b) {
    int k = a.size();
    vector<vector<long long>> c(k, vector<long long>(k, 0));
    for (int i = 0; i < k; i++)
        for (int l = 0; l < k; l++)
            if (a[i][l])
                for (int j = 0; j < k; j++) c[i][j] = (c[i][j] + a[i][l] * b[l][j]) % MOD;
    return c;
}
