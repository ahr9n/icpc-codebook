/**
 * Catalan numbers mod p: Cat(n) = C(2n, n) / (n+1) = C(2n, n) - C(2n, n+1).
 * Counts balanced bracket sequences, full binary trees, polygon triangulations,
 * distinct BSTs, and monotone lattice paths staying below the diagonal.
 * The subtraction form sidesteps a modular inverse of (n+1). Uses ncr()/MOD
 * above; call ncr_init(2 * maxN) first. O(1) per query.
 */
long long catalan(int n) {
    return (ncr(2 * n, n) - ncr(2 * n, n + 1) + MOD) % MOD;
}

/**
 * Example: the first six Catalan numbers Cat(0..5).
 */
int main() {
    ncr_init(1000);
    for (int n = 0; n <= 5; n++) cout << catalan(n) << " \n"[n == 5];
    // -> 1 1 2 5 14 42
    return 0;
}
