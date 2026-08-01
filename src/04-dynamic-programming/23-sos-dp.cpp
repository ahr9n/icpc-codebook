/**
 * Sum over Subsets (SOS DP) — zeta transform on the subset lattice.
 * Given a[mask] over all masks of `bits` bits, overwrite it with
 *   f[mask] = sum of a[sub] over every submask sub of mask.
 * O(bits * 2^bits): one pass per bit, adding the value of the mask with that
 * bit cleared. Doing it bit-by-bit is what turns the naive O(3^bits)
 * submask enumeration into O(bits * 2^bits) — each bit is folded once.
 * Flip the += direction (over_zeta) to sum over supersets instead. These are
 * the zeta transforms underlying subset convolution and Mobius inversion.
 */
void subset_zeta(vector<long long>& a, int bits) {
    for (int b = 0; b < bits; b++)
        for (int mask = 0; mask < (1 << bits); mask++)
            if (mask & (1 << b)) a[mask] += a[mask ^ (1 << b)];
}

void superset_zeta(vector<long long>& a, int bits) {
    for (int b = 0; b < bits; b++)
        for (int mask = 0; mask < (1 << bits); mask++)
            if (not(mask & (1 << b))) a[mask] += a[mask | (1 << b)];
}

/**
 * Example: bits = 3, a[mask] = mask. f[0b111] sums a over all 8 submasks
 * (0..7 = 28); f[0b101] sums a[0], a[1], a[4], a[5] = 10.
 */
int main() {
    int bits = 3;
    vector<long long> a(1 << bits);
    for (int mask = 0; mask < (1 << bits); mask++) a[mask] = mask;

    vector<long long> f = a;
    subset_zeta(f, bits);
    cout << f[0b111] << "\n";  // -> 28
    cout << f[0b101] << "\n";  // -> 10

    vector<long long> g = a;
    superset_zeta(g, bits);
    cout << g[0b000] << "\n";  // sum over all masks 0..7 -> 28
    cout << g[0b100] << "\n";  // a[4]+a[5]+a[6]+a[7]     -> 22
    return 0;
}
