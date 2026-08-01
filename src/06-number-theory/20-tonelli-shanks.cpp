/**
 * Tonelli-Shanks: a modular square root. Solves x^2 == a (mod p) for an odd
 * prime p, returning one root r (the other is p - r) or -1 when a is a quadratic
 * non-residue. O(log^2 p).
 *
 * Contract: p must be an odd prime and 0 <= a < p. a == 0 returns 0. The p == 3
 * mod 4 case has a closed form a^((p+1)/4); the general loop is only entered when
 * p == 1 mod 4, where it walks down the 2-adic ladder of the group of order
 * p - 1 to cancel the non-square part one squaring at a time.
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

long long sqrt_mod(long long a, long long p) {
    a %= p;
    if (a == 0) return 0;
    // Euler's criterion: a is a residue iff a^((p-1)/2) == 1.
    if (pow_mod(a, (p - 1) / 2, p) != 1) return -1;
    if (p % 4 == 3) return pow_mod(a, (p + 1) / 4, p);

    long long q = p - 1;
    int s = 0;
    while ((q & 1) == 0) q >>= 1, s++;

    long long z = 2;
    while (pow_mod(z, (p - 1) / 2, p) != p - 1) z++;

    int m = s;
    long long c = pow_mod(z, q, p);
    long long t = pow_mod(a, q, p);
    long long r = pow_mod(a, (q + 1) / 2, p);
    while (t != 1) {
        int i = 0;
        long long cur = t;
        while (cur != 1) cur = mul_mod(cur, cur, p), i++;
        long long b = pow_mod(c, 1LL << (m - i - 1), p);
        m = i;
        c = mul_mod(b, b, p);
        t = mul_mod(t, c, p);
        r = mul_mod(r, b, p);
    }
    return r;
}

/**
 * Example: a root of 2 modulo 113 (a prime that is 1 mod 4).
 */
int main() {
    long long p = 113, a = 2;
    long long r = sqrt_mod(a, p);
    cout << r << " " << mul_mod(r, r, p) << "\n";  // -> 62 2
    return 0;
}
