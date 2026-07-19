/**
 * Trial-division factorization of one n into (prime, exponent) pairs. O(sqrt n)
 */
vector<pair<long long, int>> factorize(long long n) {
    vector<pair<long long, int>> f;
    for (long long p = 2; p * p <= n; p++)
        if (n % p == 0) {
            int cnt = 0;
            while (n % p == 0) n /= p, cnt++;
            f.push_back({p, cnt});
        }
    if (n > 1) f.push_back({n, 1});
    return f;
}
