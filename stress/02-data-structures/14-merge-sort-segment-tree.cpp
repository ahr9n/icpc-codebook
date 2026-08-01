// Stress: MergeSortTree count_leq / count_in vs naive scan over every [l, r].
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
        }
    printf("merge-sort-segment-tree PASS %lld", checks);
    return 0;
}
