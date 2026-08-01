long long burnside_power(long long base, long long exponent) {
    long long result = 1;
    base %= MOD;
    while (exponent > 0) {
        if (exponent & 1) {
            result = result * base % MOD;
        }
        base = base * base % MOD;
        exponent >>= 1;
    }
    return result;
}

/**
 * Burnside/Pólya counting for colorings of an n-bead cycle with `colors`
 * colors. count_necklaces identifies rotations; count_bracelets identifies
 * rotations and reflections. Burnside averages the colorings fixed by each
 * symmetry: a rotation by shift fixes colors on gcd(n, shift) cycles, while a
 * reflection fixes one or two beads plus mirrored pairs depending on parity.
 * Each function takes O(n log n) time and O(1) memory.
 *
 * Contract: n >= 1, colors >= 0, MOD is an odd prime, and n is not divisible
 * by MOD so the group-size inverse exists.
 */
long long count_necklaces(long long n, long long colors) {
    long long fixed_sum = 0;
    for (long long shift = 0; shift < n; shift++) {
        fixed_sum = (fixed_sum + burnside_power(colors, gcd(n, shift))) % MOD;
    }
    return fixed_sum * burnside_power(n, MOD - 2) % MOD;
}

long long count_bracelets(long long n, long long colors) {
    long long fixed_sum = 0;
    for (long long shift = 0; shift < n; shift++) {
        fixed_sum = (fixed_sum + burnside_power(colors, gcd(n, shift))) % MOD;
    }

    if (n & 1) {
        fixed_sum = (fixed_sum + n % MOD * burnside_power(colors, n / 2 + 1)) % MOD;
    } else {
        long long through_beads = burnside_power(colors, n / 2 + 1);
        long long through_edges = burnside_power(colors, n / 2);
        fixed_sum = (fixed_sum + (n / 2) % MOD * (through_beads + through_edges)) % MOD;
    }

    return fixed_sum * burnside_power(2 * (n % MOD) % MOD, MOD - 2) % MOD;
}

/** Example: count small necklaces and bracelets under their symmetry groups. */
int main() {
    cout << count_necklaces(4, 3) << "\n";
    cout << count_bracelets(4, 3) << "\n";
    cout << count_bracelets(5, 2) << "\n";
    return 0;
}
// -> 24
// -> 21
// -> 8
