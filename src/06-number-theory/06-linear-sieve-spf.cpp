/**
 * Linear sieve: smallest prime factor spf[x] for all x <= n in O(n).
 * Factorize any x <= n fast by dividing out spf[x] repeatedly.
 */
vector<int> linear_sieve(int n) {
    vector<int> spf(n + 1, 0), primes;
    for (int i = 2; i <= n; i++) {
        if (spf[i] == 0) {
            spf[i] = i;
            primes.push_back(i);
        }
        for (int p: primes) {
            if (p > spf[i] or (long long) i * p > n) break;
            spf[i * p] = p;
        }
    }
    return spf;
}
