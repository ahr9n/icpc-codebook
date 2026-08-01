// Stress: WaveletTree kth / count_leq vs naive sort-and-count over random ranges.
int stress() {
    mt19937 rng(17);
    long long checks = 0;
    for (int rep = 0; rep < 4000; rep++) {
        int n = 1 + rng() % 40;
        vector<int> a(n);
        for (auto& x: a) x = (int)(rng() % 21) - 10;
        WaveletTree wt(a);
        int queries = 15 + rng() % 20;
        for (int q = 0; q < queries; q++) {
            int l = rng() % n;
            int r = l + rng() % (n - l);
            vector<int> window(a.begin() + l, a.begin() + r + 1);
            sort(window.begin(), window.end());

            int k = 1 + rng() % (int)window.size();
            if (wt.kth(l, r, k) != window[k - 1]) {
                printf("FAIL kth l=%d r=%d k=%d got=%d want=%d\n", l, r, k, wt.kth(l, r, k),
                       window[k - 1]);
                return 1;
            }
            checks++;

            int x = (int)(rng() % 25) - 12;
            int ref = 0;
            for (int v: window)
                if (v <= x) ref++;
            if (wt.count_leq(l, r, x) != ref) {
                printf("FAIL count l=%d r=%d x=%d got=%d want=%d\n", l, r, x, wt.count_leq(l, r, x),
                       ref);
                return 1;
            }
            checks++;
        }
    }

    vector<int> extremes = {INT_MAX, 0, INT_MIN, INT_MAX, INT_MIN};
    WaveletTree wt(extremes);
    vector<int> sorted = extremes;
    sort(sorted.begin(), sorted.end());
    for (int k = 1; k <= (int)sorted.size(); k++) {
        if (wt.kth(0, (int)extremes.size() - 1, k) != sorted[k - 1]) {
            printf("FAIL extreme kth k=%d\n", k);
            return 1;
        }
        checks++;
    }
    for (int x: {INT_MIN, -1, 0, INT_MAX}) {
        int ref = upper_bound(sorted.begin(), sorted.end(), x) - sorted.begin();
        if (wt.count_leq(0, (int)extremes.size() - 1, x) != ref) {
            printf("FAIL extreme count x=%d\n", x);
            return 1;
        }
        checks++;
    }
    printf("wavelet-tree PASS %lld", checks);
    return 0;
}
