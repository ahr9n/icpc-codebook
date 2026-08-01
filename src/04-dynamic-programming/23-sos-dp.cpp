/**
 * Sum over Subsets (SOS DP) — zeta transform on the subset lattice.
 * Given a[mask] over all masks of `bits` bits, overwrite it with
 *   f[mask] = sum of a[sub] over every submask sub of mask.
 * O(bits * 2^bits): one pass per bit, adding the value of the mask with that
 * bit cleared. Doing it bit-by-bit is what turns the naive O(3^bits)
 * submask enumeration into O(bits * 2^bits) — each bit is folded once.
 * superset_zeta performs the analogous sum over all supersets. Contract:
 * 0 <= bits < 31, a.size() == 2^bits, and every transformed sum fits in
 * long long.
 */
void subset_zeta(vector<long long>& a, int bits) {
    for (int bit = 0; bit < bits; bit++) {
        for (int mask = 0; mask < (1 << bits); mask++) {
            if (mask & (1 << bit)) {
                a[mask] += a[mask ^ (1 << bit)];
            }
        }
    }
}

void superset_zeta(vector<long long>& a, int bits) {
    for (int bit = 0; bit < bits; bit++) {
        for (int mask = 0; mask < (1 << bits); mask++) {
            if (not(mask & (1 << bit))) {
                a[mask] += a[mask | (1 << bit)];
            }
        }
    }
}

/** Example: subset and superset sums over all three-bit masks. */
int main() {
    int bits = 3;
    vector<long long> a(1 << bits);
    for (int mask = 0; mask < (1 << bits); mask++) {
        a[mask] = mask;
    }

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
