// Stress: MergeSortTree count_leq / count_in / kth vs naive scans and sorting.
int stress() {
    mt19937 rng(99);
    long long checks = 0;
    for (int n = 1; n <= 40; n++)
        for (int rep = 0; rep < 30; rep++) {
            vector<int> values(n);
            for (auto& x: values) x = (int)(rng() % 21) - 10;
            MergeSortTree mst(values);

            for (int l = 0; l < n; l++)
                for (int r = l; r < n; r++)
                    for (int t = 0; t < 4; t++) {
                        int v = (int)(rng() % 25) - 12;
                        long long ref_leq = 0;
                        for (int i = l; i <= r; i++)
                            if (values[i] <= v) ref_leq++;
                        if (mst.count_leq(l, r, v) != ref_leq) {
                            printf("FAIL count_leq n=%d l=%d r=%d v=%d\n", n, l, r, v);
                            return 1;
                        }
                        checks++;

                        int a = (int)(rng() % 25) - 12, b = (int)(rng() % 25) - 12;
                        int lo = min(a, b), hi = max(a, b);
                        long long ref_in = 0;
                        for (int i = l; i <= r; i++)
                            if (values[i] >= lo and values[i] <= hi) ref_in++;
                        if (mst.count_in(l, r, lo, hi) != ref_in) {
                            printf("FAIL count_in n=%d l=%d r=%d lo=%d hi=%d\n", n, l, r, lo, hi);
                            return 1;
                        }
                        checks++;
                    }

            for (int l = 0; l < n; l++) {
                for (int r = l; r < n; r++) {
                    vector<int> sorted(values.begin() + l, values.begin() + r + 1);
                    sort(sorted.begin(), sorted.end());
                    for (int k = 1; k <= (int)sorted.size(); k++) {
                        if (mst.kth(l, r, k) != sorted[k - 1]) {
                            printf("FAIL kth n=%d l=%d r=%d k=%d\n", n, l, r, k);
                            return 1;
                        }
                        checks++;
                    }
                }
            }
        }

    vector<int> extremes = {INT_MIN, -1, 0, INT_MAX};
    MergeSortTree mst(extremes);
    vector<array<int, 4>> cases = {
        {INT_MIN, INT_MIN, 0, 0},
        {INT_MIN, INT_MAX, 0, 3},
        {0, INT_MAX, 2, 3},
    };
    for (auto [lo, hi, first, last]: cases) {
        long long ref = 0;
        for (int i = first; i <= last; i++) {
            if (lo <= extremes[i] and extremes[i] <= hi) {
                ref++;
            }
        }
        if (mst.count_in(first, last, lo, hi) != ref) {
            printf("FAIL extreme lo=%d hi=%d\n", lo, hi);
            return 1;
        }
        checks++;
    }
    vector<int> sorted_extremes = extremes;
    sort(sorted_extremes.begin(), sorted_extremes.end());
    for (int k = 1; k <= (int)sorted_extremes.size(); k++) {
        if (mst.kth(0, (int)extremes.size() - 1, k) != sorted_extremes[k - 1]) {
            printf("FAIL extreme kth k=%d\n", k);
            return 1;
        }
        checks++;
    }
    printf("merge-sort-segment-tree PASS %lld", checks);
    return 0;
}
