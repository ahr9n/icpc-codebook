const long long MOD = 1e9 + 7;

/**
 * Modular inverse (prime m) via Fermat. Use for division under mod.
 */
long long inv(long long a, long long mod = MOD) {
    return power(a, mod - 2, mod);
}
