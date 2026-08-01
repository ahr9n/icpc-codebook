// Stress: the linear sieve's spf/mu/phi vs independent per-n trial-division
// factorization, the divisor-sum identities sum_{d|n} mu(d) == [n==1] and
// sum_{d|n} phi(d) == n (accumulated sieve-style), and count_squarefree vs a
// cumulative brute plus a p^2-marking scan for large n.
static long long mu_from_factors(int n) {
    long long mu = 1;
    for (int d = 2; (long long)d * d <= n; d++)
        if (n % d == 0) {
            int e = 0;
            while (n % d == 0) {
                n /= d;
                e++;
            }
            if (e > 1) return 0;
            mu = -mu;
        }
    if (n > 1) mu = -mu;
    return mu;
}

static long long phi_from_factors(int n) {
    long long phi = 1;
    int m = n;
    for (int d = 2; (long long)d * d <= m; d++)
        if (m % d == 0) {
            long long pk = 1;
            while (m % d == 0) {
                m /= d;
                pk *= d;
            }
            phi *= pk - pk / d;
        }
    if (m > 1) phi *= m - 1;
    return phi;
}

static int smallest_prime_factor(int n) {
    for (int d = 2; (long long)d * d <= n; d++)
        if (n % d == 0) return d;
    return n;
}

int stress() {
    const int limit = 30000;
    MultiplicativeSieve s = multiplicative_sieve(limit);
    long long checks = 0;

    vector<int> squarefree_prefix(limit + 1, 0);
    for (int n = 1; n <= limit; n++) {
        if (s.mu[n] != mu_from_factors(n)) {
            printf("FAIL mu n=%d got=%d\n", n, s.mu[n]);
            return 1;
        }
        if (s.phi[n] != phi_from_factors(n)) {
            printf("FAIL phi n=%d got=%d\n", n, s.phi[n]);
            return 1;
        }
        if (n >= 2 and s.spf[n] != smallest_prime_factor(n)) {
            printf("FAIL spf n=%d got=%d\n", n, s.spf[n]);
            return 1;
        }
        bool is_squarefree = mu_from_factors(n) != 0;
        squarefree_prefix[n] = squarefree_prefix[n - 1] + (is_squarefree ? 1 : 0);
        checks++;
    }

    vector<long long> mobius_sum(limit + 1, 0), phi_sum(limit + 1, 0);
    for (int d = 1; d <= limit; d++)
        for (int m = d; m <= limit; m += d) {
            mobius_sum[m] += s.mu[d];
            phi_sum[m] += s.phi[d];
        }
    for (int n = 1; n <= limit; n++) {
        long long expected_mobius = (n == 1) ? 1 : 0;
        if (mobius_sum[n] != expected_mobius) {
            printf("FAIL mobius-identity n=%d got=%lld\n", n, mobius_sum[n]);
            return 1;
        }
        if (phi_sum[n] != n) {
            printf("FAIL phi-identity n=%d got=%lld\n", n, phi_sum[n]);
            return 1;
        }
        checks++;
    }

    for (int n = 1; n <= limit; n++) {
        if (count_squarefree(n, s) != squarefree_prefix[n]) {
            printf("FAIL count_squarefree n=%d got=%lld exp=%d\n", n, count_squarefree(n, s),
                   squarefree_prefix[n]);
            return 1;
        }
        checks++;
    }

    for (long long big: {200000LL, 750000LL, 2000000LL}) {
        vector<char> has_square(big + 1, 0);
        for (long long p = 2; p * p <= big; p++)
            for (long long m = p * p; m <= big; m += p * p) has_square[m] = 1;
        long long brute = 0;
        for (long long k = 1; k <= big; k++)
            if (not has_square[k]) brute++;
        if (count_squarefree(big, s) != brute) {
            printf("FAIL count_squarefree big=%lld got=%lld exp=%lld\n", big,
                   count_squarefree(big, s), brute);
            return 1;
        }
        checks++;
    }

    printf("mobius-multiplicative PASS %lld", checks);
    return 0;
}
