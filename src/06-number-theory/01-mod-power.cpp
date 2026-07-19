const long long MOD = 1e9 + 7;

/**
 * Fast modular exponentiation: b^e mod m by squaring. O(log e)
 */
long long power(long long b, long long e, long long m = MOD) {
    long long r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = r * b % m;
        b = b * b % m;
        e >>= 1;
    }
    return r;
}
