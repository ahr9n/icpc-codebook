long long mod_power(long long base, long long exponent) {
    long long result = 1;
    base = (base % MOD + MOD) % MOD;
    while (exponent > 0) {
        if (exponent & 1) {
            result = result * base % MOD;
        }
        base = base * base % MOD;
        exponent >>= 1;
    }
    return result;
}

/**
 * Evaluates the unique polynomial of degree below n passing through n points
 * (x[i], y[i]) at target, modulo the prime MOD. The x-coordinates must be
 * distinct modulo MOD, and x and y must have the same positive size. Direct
 * Lagrange interpolation takes O(n^2 + n log MOD) time and O(1) auxiliary
 * memory; use factorial prefix/suffix products for the common special case
 * x[i] = i when O(n) per evaluation matters.
 */
long long lagrange_interpolate(const vector<long long>& x, const vector<long long>& y,
                               long long target) {
    int n = x.size();
    target = (target % MOD + MOD) % MOD;
    long long answer = 0;

    for (int i = 0; i < n; i++) {
        long long numerator = 1;
        long long denominator = 1;
        long long xi = (x[i] % MOD + MOD) % MOD;
        for (int j = 0; j < n; j++) {
            if (i == j) {
                continue;
            }
            long long xj = (x[j] % MOD + MOD) % MOD;
            numerator = numerator * ((target - xj + MOD) % MOD) % MOD;
            denominator = denominator * ((xi - xj + MOD) % MOD) % MOD;
        }
        long long yi = (y[i] % MOD + MOD) % MOD;
        long long term = yi * numerator % MOD * mod_power(denominator, MOD - 2) % MOD;
        answer = (answer + term) % MOD;
    }
    return answer;
}

/** Example: recover p(x) = x^2 + 2x + 3 from three sampled values. */
int main() {
    vector<long long> x = {0, 1, 2};
    vector<long long> y = {3, 6, 11};  // p(x) = x^2 + 2x + 3
    cout << lagrange_interpolate(x, y, 5) << "\n";
    cout << lagrange_interpolate(x, y, 1) << "\n";
    return 0;
}
// -> 38
// -> 6
