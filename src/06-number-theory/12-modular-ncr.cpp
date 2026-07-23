const long long MOD = 1e9 + 7;

/**
 * Modular nCr: precompute factorials + inverse factorials once, then O(1) per query.
 * Call ncr_init(maxN) before ncr(n, r). Uses power()/inv() above.
 */
vector<long long> fact, inv_fact;

void ncr_init(int n) {
    fact.assign(n + 1, 1);
    inv_fact.assign(n + 1, 1);
    for (int i = 1; i <= n; i++) fact[i] = fact[i - 1] * i % MOD;
    inv_fact[n] = inv(fact[n]);
    for (int i = n; i > 0; i--) inv_fact[i - 1] = inv_fact[i] * i % MOD;
}

long long ncr(int n, int r) {
    if (r < 0 or r > n) return 0;
    return fact[n] * inv_fact[r] % MOD * inv_fact[n - r] % MOD;
}

/**
 * Example: C(5, 2) modulo MOD after precomputing factorials.
 */
int main() {
    ncr_init(1000);
    cout << ncr(5, 2) << "\n";  // -> 10
    return 0;
}
