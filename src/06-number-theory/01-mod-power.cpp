const long long MOD = 1e9 + 7;

/**
 * Fast modular exponentiation: b^e mod m by squaring. O(log e)
 */
long long power(long long base, long long exp, long long mod = MOD) {
    long long res = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1) res = res * base % mod;
        base = base * base % mod;
        exp >>= 1;
    }
    return res;
}
