// Stress: SqrtTree range-min vs naive fold over all [l, r], many random arrays.
int stress() {
    mt19937 rng(7);
    long long checks = 0;
    for (int n = 1; n <= 70; n++)
        for (int rep = 0; rep < 20; rep++) {
            vector<long long> a(n);
            for (auto& x: a) x = (long long)(rng() % 401) - 200;
            SqrtTree st(a);
            for (int l = 0; l < n; l++)
                for (int r = l; r < n; r++) {
                    long long ref = a[l];
                    for (int i = l; i <= r; i++) ref = min(ref, a[i]);
                    if (st.query(l, r) != ref) {
                        printf("FAIL n=%d l=%d r=%d\n", n, l, r);
                        return 1;
                    }
                    checks++;
                }
        }
    printf("sqrt-tree PASS %lld", checks);
    return 0;
}
