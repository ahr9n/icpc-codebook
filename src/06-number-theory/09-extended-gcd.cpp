/**
 * Extended Euclid: returns g = gcd(a, b) and x, y with a*x + b*y = g.
 * Basis for modular inverse of non-prime moduli and CRT. O(log min(a,b))
 */
long long ext_gcd(long long a, long long b, long long& x, long long& y) {
    if (b == 0) {
        x = 1, y = 0;
        return a;
    }
    long long x1, y1;
    long long g = ext_gcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}
