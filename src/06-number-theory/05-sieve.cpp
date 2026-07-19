/**
 * Sieve of Eratosthenes: is_prime[0..n]. O(n log log n)
 */
vector<bool> sieve(int n) {
    vector<bool> is_prime(n + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; (long long)i * i <= n; i++)
        if (is_prime[i])
            for (long long j = (long long)i * i; j <= n; j += i) is_prime[j] = false;
    return is_prime;
}
