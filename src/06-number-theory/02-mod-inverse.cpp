const long long MOD = 1e9 + 7;

/**
 * Modular inverse (prime m) via Fermat. Use for division under mod.
 */
long long inv(long long a, long long m = MOD) {
    return power(a, m - 2, m);
}
