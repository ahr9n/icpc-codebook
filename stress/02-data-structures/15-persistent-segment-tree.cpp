// Stress: PersistentSegTree k-th smallest in [l, r] vs a sorted subarray.
int stress() {
    mt19937 rng(2024);
    long long checks = 0;
    for (int n = 1; n <= 40; n++)
        for (int rep = 0; rep < 30; rep++) {
            vector<long long> a(n);
            for (auto& x: a) x = (long long)(rng() % 15) - 7;  // narrow range forces duplicates
            PersistentSegTree pst(a);

            for (int l = 1; l <= n; l++)
                for (int r = l; r <= n; r++) {
                    vector<long long> sub(a.begin() + l - 1, a.begin() + r);
                    sort(sub.begin(), sub.end());
                    for (int k = 1; k <= r - l + 1; k++) {  // k == 1 and k == len are the bounds
                        long long got = pst.query(l, r, k);
                        if (got != sub[k - 1]) {
                            printf("FAIL n=%d l=%d r=%d k=%d got=%lld ref=%lld\n", n, l, r, k, got,
                                   sub[k - 1]);
                            return 1;
                        }
                        checks++;
                    }
                }
        }
    printf("persistent-segment-tree PASS %lld", checks);
    return 0;
}
