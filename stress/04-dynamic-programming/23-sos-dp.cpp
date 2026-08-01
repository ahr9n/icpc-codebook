// Stress: subset_zeta / superset_zeta vs naive submask/supermask sums.
int stress() {
    mt19937 rng(23);
    long long checks = 0;

    // Edge cases: b=0 (single element), all-zero, single nonzero.
    {
        vector<long long> a = {42};
        subset_zeta(a, 0);
        if (a[0] != 42) {
            printf("FAIL b=0\n");
            return 1;
        }
        checks++;
    }

    for (int trial = 0; trial < 4000; trial++) {
        int bits = rng() % 13;  // up to 2^12
        int size = 1 << bits;
        vector<long long> a(size);

        int shape = trial % 3;
        for (int mask = 0; mask < size; mask++) {
            if (shape == 0)
                a[mask] = 0;  // all-zero
            else if (shape == 1)
                a[mask] = (mask == (size - 1)) ? 7 : 0;  // single nonzero
            else
                a[mask] = (long long)(rng() % 2001) - 1000;
        }

        vector<long long> sub = a;
        subset_zeta(sub, bits);
        vector<long long> sup = a;
        superset_zeta(sup, bits);

        // Naive O(3^bits) submask/supermask sums for small bits; sample masks
        // for larger bits to keep the sweep cheap while covering big sizes.
        bool full = bits <= 8;
        for (int mask = 0; mask < size; mask++) {
            if (not full and (rng() % size) != 0) continue;

            long long ref_sub = 0;
            for (int s = mask;; s = (s - 1) & mask) {
                ref_sub += a[s];
                if (s == 0) break;
            }
            if (sub[mask] != ref_sub) {
                printf("FAIL subset bits=%d mask=%d\n", bits, mask);
                return 1;
            }

            long long ref_sup = 0;
            int complement = (size - 1) ^ mask;
            for (int c = complement;; c = (c - 1) & complement) {
                ref_sup += a[mask | c];
                if (c == 0) break;
            }
            if (sup[mask] != ref_sup) {
                printf("FAIL superset bits=%d mask=%d\n", bits, mask);
                return 1;
            }
            checks++;
        }
    }

    printf("sos-dp PASS %lld", checks);
    return 0;
}
