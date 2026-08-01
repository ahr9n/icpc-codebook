// Stress: SegTreeBeats chmin/sum/max vs a naive array over random operations.
int stress() {
    mt19937 rng(13);
    long long checks = 0;
    for (int rep = 0; rep < 3000; rep++) {
        int n = 1 + rng() % 40;
        vector<long long> a(n);
        for (auto& x: a) x = (long long)(rng() % 201) - 100;
        SegTreeBeats st(a);
        int ops = 20 + rng() % 40;
        for (int o = 0; o < ops; o++) {
            int l = rng() % n;
            int r = l + rng() % (n - l);
            int kind = rng() % 3;
            if (kind == 0) {
                long long x = (long long)(rng() % 201) - 100;
                for (int i = l; i <= r; i++) a[i] = min(a[i], x);
                st.chmin(l, r, x);
                continue;
            }
            if (kind == 1) {
                long long ref = 0;
                for (int i = l; i <= r; i++) ref += a[i];
                if (st.range_sum(l, r) != ref) {
                    printf("FAIL sum l=%d r=%d got=%lld want=%lld\n", l, r, st.range_sum(l, r),
                           ref);
                    return 1;
                }
                checks++;
                continue;
            }
            long long ref = a[l];
            for (int i = l; i <= r; i++) ref = max(ref, a[i]);
            if (st.range_max(l, r) != ref) {
                printf("FAIL max l=%d r=%d got=%lld want=%lld\n", l, r, st.range_max(l, r), ref);
                return 1;
            }
            checks++;
        }
    }
    printf("segment-tree-beats PASS %lld", checks);
    return 0;
}
