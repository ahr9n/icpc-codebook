/**
 * Matrix exponentiation: mat^p under MOD. Linear recurrences (Fibonacci, path counts,
 * DP transitions) in O(k^3 log p). Start from the identity matrix.
 */
vector<vector<long long>> mat_pow(vector<vector<long long>> base, long long exp) {
    int dim = base.size();
    vector<vector<long long>> result(dim, vector<long long>(dim, 0));
    for (int i = 0; i < dim; i++) result[i][i] = 1;
    while (exp) {
        if (exp & 1) result = mat_mul(result, base);
        base = mat_mul(base, base);
        exp >>= 1;
    }
    return result;
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
