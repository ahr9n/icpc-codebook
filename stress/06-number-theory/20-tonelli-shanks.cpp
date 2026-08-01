// Stress: sqrt_mod over every residue of many odd primes. Covers p == 3 mod 4
// (closed form) and p == 1 mod 4 (the Tonelli loop), plus a == 0 and a == 1.
// A reported root r must satisfy r*r == a; a reported non-residue must fail
// Euler's criterion.
bool is_small_prime(long long n) {
    if (n < 2) return false;
    for (long long d = 2; d * d <= n; d++)
        if (n % d == 0) return false;
    return true;
}

int stress() {
    vector<long long> primes;
    for (long long p = 3; p <= 500; p++)
        if (is_small_prime(p)) primes.push_back(p);
    for (long long p: {1009LL, 1013LL, 5003LL, 10007LL, 99991LL, 999983LL, 998244353LL,
                       1000000007LL, 2305843009213693951LL})
        primes.push_back(p);

    long long checks = 0;
    for (long long p: primes) {
        long long limit = min(p, 2000LL);
        for (long long a = 0; a < limit; a++) {
            long long r = sqrt_mod(a, p);
            if (r == -1) {
                if (pow_mod(a % p, (p - 1) / 2, p) == 1) {
                    printf("FAIL p=%lld a=%lld: said non-residue but is residue\n", p, a);
                    return 1;
                }
            } else {
                if (r < 0 or r >= p or mul_mod(r, r, p) != a % p) {
                    printf("FAIL p=%lld a=%lld: bad root %lld\n", p, a, r);
                    return 1;
                }
            }
            checks++;
        }
    }
    printf("tonelli-shanks PASS %lld", checks);
    return 0;
}
