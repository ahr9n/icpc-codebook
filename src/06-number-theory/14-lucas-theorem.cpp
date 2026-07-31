/**
 * Lucas' theorem: C(n, r) mod p for a small prime p but astronomically large
 * n, r. Split n and r into base-p digits and multiply the per-digit binomials
 * C(n_i, r_i) mod p; a digit with r_i > n_i makes the whole product zero.
 * Each small binomial multiplies out in O(p) with a Fermat inverse. Requires p
 * prime. O(log_p(n) * p) per query. Uses power() above.
 */
long long small_ncr(long long n, long long r, long long p) {
    if (r > n) return 0;
    long long num = 1, den = 1;
    for (long long i = 0; i < r; i++) {
        num = num * ((n - i) % p) % p;
        den = den * ((i + 1) % p) % p;
    }
    return num * power(den, p - 2, p) % p;
}

long long lucas(long long n, long long r, long long p) {
    if (r == 0) return 1;
    return small_ncr(n % p, r % p, p) * lucas(n / p, r / p, p) % p;
}

/**
 * Example: C(10, 3) = 120, so C(10, 3) mod 13 = 120 - 9*13 = 3.
 */
int main() {
    cout << lucas(10, 3, 13) << "\n";  // -> 3
    return 0;
}
