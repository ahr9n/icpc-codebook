/**
 * Fibonacci F(n) mod MOD by fast doubling. O(log n)
 * Indexing F(0)=0, F(1)=1. Contract: n >= 0.
 * Recurses on the doubling identities
 *   F(2k)   = F(k) * (2*F(k+1) - F(k))
 *   F(2k+1) = F(k)^2 + F(k+1)^2
 * returning the pair {F(n), F(n+1)} so each halving of n is one call.
 */
pair<long long, long long> fib_pair(long long n) {
    if (n == 0) {
        return {0, 1 % MOD};
    }
    auto [f, g] = fib_pair(n >> 1);
    long long two_g_minus_f = (2 * g - f + 2 * MOD) % MOD;
    long long even = f * two_g_minus_f % MOD;
    long long odd = (f * f + g * g) % MOD;
    if (n & 1) {
        return {odd, (even + odd) % MOD};
    }
    return {even, odd};
}

long long fib(long long n) {
    return fib_pair(n).first;
}

/** Example: Fibonacci numbers at indices 10, 50, and 100 modulo MOD. */
int main() {
    cout << fib(10) << "\n";
    cout << fib(50) << "\n";
    cout << fib(100) << "\n";
    return 0;
}
// -> 55
// -> 586268941
// -> 687995182
