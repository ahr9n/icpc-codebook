/**
 * Miller-Rabin deterministic primality for any n up to 9.2e18 (signed 64-bit).
 * Write n-1 = d * 2^s; n is prime iff for every witness a, either a^d == 1 or
 * some a^(d*2^i) == n-1 mod n. The twelve prime witnesses below are proven to
 * give an exact answer well past 2^64, so there are no probabilistic failures.
 * A dedicated __int128 mul_mod avoids the overflow the section's power() would
 * hit at large moduli. O(witnesses * log n).
 */
long long mul_mod(long long a, long long b, long long mod) {
    return (long long)((__int128)a * b % mod);
}

long long pow_mod(long long base, long long exp, long long mod) {
    long long res = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1) res = mul_mod(res, base, mod);
        base = mul_mod(base, base, mod);
        exp >>= 1;
    }
    return res;
}

bool is_prime(long long n) {
    if (n < 2) return false;
    for (long long p: {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37})
        if (n % p == 0) return n == p;
    long long d = n - 1;
    int s = 0;
    while ((d & 1) == 0) d >>= 1, s++;
    for (long long a: {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37}) {
        long long x = pow_mod(a, d, n);
        if (x == 1 or x == n - 1) continue;
        bool composite = true;
        for (int i = 0; i < s - 1; i++) {
            x = mul_mod(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/**
 * Example: 1e9+7 is prime; its neighbour 1e9+8 is not.
 */
int main() {
    cout << is_prime(1000000007LL) << " " << is_prime(1000000008LL) << "\n";  // -> 1 0
    return 0;
}
