/**
 * Matrix exponentiation: mat^p under MOD. Linear recurrences (Fibonacci, path counts,
 * DP transitions) in O(k^3 log p). Start from the identity matrix.
 */
vector<vector<long long>> mat_pow(vector<vector<long long>> a, long long p) {
    int k = a.size();
    vector<vector<long long>> r(k, vector<long long>(k, 0));
    for (int i = 0; i < k; i++) r[i][i] = 1;
    while (p) {
        if (p & 1) r = mat_mul(r, a);
        a = mat_mul(a, a);
        p >>= 1;
    }
    return r;
}

/**
 * Example: nth Fibonacci via [[1,1],[1,0]]^n. res[0][1] is F(n).
 */
int main() {
    long long fib_n = 10;
    vector<vector<long long>> base = {{1, 1}, {1, 0}};
    vector<vector<long long>> res = mat_pow(base, fib_n);
    cout << res[0][1] << '\n';
    return 0;
}
