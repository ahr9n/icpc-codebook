/**
 * Pollard's rho factorization of a 64-bit n. Iterate x -> x^2 + c mod n and take
 * gcd(|x - y|, n) with y running twice as fast (Floyd cycle detection); a cycle
 * in the pseudo-random sequence surfaces a nontrivial factor. On a full cycle
 * without one, restart with a fresh constant c. Recurse, using Miller-Rabin to
 * stop at primes. Expected O(n^1/4) per split -- 64-bit numbers factor in
 * milliseconds. Uses gcd()/mul_mod()/is_prime() above.
 */
long long pollard(long long n) {
    if (n % 2 == 0) return 2;
    long long c = 1;
    while (true) {
        long long x = 2, y = 2, d = 1;
        while (d == 1) {
            x = (mul_mod(x, x, n) + c) % n;
            y = (mul_mod(y, y, n) + c) % n;
            y = (mul_mod(y, y, n) + c) % n;
            d = gcd(x > y ? x - y : y - x, n);
        }
        if (d != n) return d;
        c++;
    }
}

void factor_rec(long long n, vector<long long>& out) {
    if (n == 1) return;
    if (is_prime(n)) {
        out.push_back(n);
        return;
    }
    long long d = pollard(n);
    factor_rec(d, out);
    factor_rec(n / d, out);
}

vector<long long> factorize_big(long long n) {
    vector<long long> out;
    factor_rec(n, out);
    sort(out.begin(), out.end());
    return out;
}

/**
 * Example: 2^59 - 1 = 179951 * 3203431780337 (a Mersenne semiprime).
 */
int main() {
    for (long long p: factorize_big((1LL << 59) - 1)) cout << p << " ";
    cout << "\n";  // -> 179951 3203431780337
    return 0;
}
