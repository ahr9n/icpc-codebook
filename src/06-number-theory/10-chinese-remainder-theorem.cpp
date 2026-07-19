/**
 * CRT for two congruences: x = r1 (mod m1), x = r2 (mod m2).
 * Returns {x, lcm(m1,m2)}; {0, -1} if incompatible. O(log m)
 */
pair<long long, long long> crt(long long r1, long long m1, long long r2, long long m2) {
    long long x, y;
    long long g = ext_gcd(m1, m2, x, y);
    if ((r2 - r1) % g != 0) return {0, -1};
    long long lcm = m1 / g * m2;
    long long res = r1 + (r2 - r1) / g % (m2 / g) * x % (m2 / g) * m1;
    return {(res % lcm + lcm) % lcm, lcm};
}

/**
 * Example: CRT merge x=2 (mod 3), x=3 (mod 5) -> 8 (mod 15); modular nCr(5,2)=10.
 */
int main() {
    auto [x, m] = crt(2, 3, 3, 5);
    cout << x << ' ' << m << '\n';

    ncr_init(1000);
    cout << ncr(5, 2) << '\n';

    return 0;
}
