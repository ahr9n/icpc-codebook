/**
 * Unified linear sieve for multiplicative functions in O(n). One pass builds:
 *   spf[x]  smallest prime factor           primes  primes in [2, n]
 *   mu[x]   Möbius: 0 if x has a squared prime factor, else (-1)^(#distinct primes)
 *   phi[x]  Euler totient (also standalone in 08-euler-phi.cpp)
 *
 * The pattern generalizes to any multiplicative f: extend f(i * p) from f(i)
 * using whether p is the smallest prime factor of i * p. If p does not divide i
 * then gcd(i, p) = 1 and f(i * p) = f(i) * f(p); if p divides i then i * p gains
 * a higher power of p and the value follows f's rule on prime powers (for mu the
 * repeated prime forces 0, for phi it multiplies by p). We stop at the first p
 * dividing i so every composite is written exactly once, by its spf. Contract:
 * n >= 1.
 */
struct MultiplicativeSieve {
    vector<int> spf, primes, mu, phi;
};

MultiplicativeSieve multiplicative_sieve(int n) {
    MultiplicativeSieve s;
    s.spf.assign(n + 1, 0);
    s.mu.assign(n + 1, 0);
    s.phi.assign(n + 1, 0);
    s.mu[1] = 1;
    s.phi[1] = 1;
    for (int i = 2; i <= n; i++) {
        if (s.spf[i] == 0) {
            s.spf[i] = i;
            s.primes.push_back(i);
            s.mu[i] = -1;
            s.phi[i] = i - 1;
        }
        for (int p: s.primes) {
            if ((long long)i * p > n) {
                break;
            }
            s.spf[i * p] = p;
            if (i % p == 0) {
                s.mu[i * p] = 0;
                s.phi[i * p] = s.phi[i] * p;
                break;
            }
            s.mu[i * p] = -s.mu[i];
            s.phi[i * p] = s.phi[i] * (p - 1);
        }
    }
    return s;
}

/**
 * Möbius application: count squarefree integers in [1, n]. A number is squarefree
 * iff no d > 1 with d*d dividing it, so by inclusion-exclusion over squared prime
 * factors the count is sum_{d >= 1} mu[d] * floor(n / (d*d)). Terms vanish once
 * d*d > n, so mu up to floor(sqrt n) suffices. O(sqrt n) given the sieve.
 * Contract: n >= 0 and s was built through at least floor(sqrt(n)).
 */
long long count_squarefree(long long n, const MultiplicativeSieve& s) {
    long long total = 0;
    for (long long d = 1; d <= n / d; d++) {
        total += (long long)s.mu[d] * (n / (d * d));
    }
    return total;
}

/** Example: print initial Möbius values and two squarefree counts. */
int main() {
    MultiplicativeSieve s = multiplicative_sieve(1000);

    for (int i = 1; i <= 10; i++) {
        cout << s.mu[i] << " ";
    }
    cout << "\n";  // -> 1 -1 -1 0 -1 1 -1 0 0 1

    cout << count_squarefree(10, s) << "\n";   // -> 7
    cout << count_squarefree(100, s) << "\n";  // -> 61
    return 0;
}
