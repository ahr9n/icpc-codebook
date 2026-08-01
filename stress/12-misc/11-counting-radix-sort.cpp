// Stress: counting_sort and radix_sort vs std::sort over many random arrays.
//
// Stability note: LSD radix is correct only if every byte pass is stable, so a
// correct radix result on multi-byte, duplicate-heavy values (where an unstable
// pass would scramble the higher-byte order among equal low bytes) empirically
// confirms the stable-counting mechanism that counting_sort uses too.
int stress() {
    mt19937 rng(11);
    long long checks = 0;
    for (int rep = 0; rep < 4000; rep++) {
        int n = rng() % 60;

        // counting_sort: non-negative keys in [0, max_val], often many duplicates.
        int max_val = rng() % 40;
        vector<int> a(n);
        for (auto& x: a) x = rng() % (max_val + 1);
        vector<int> ref = a;
        sort(ref.begin(), ref.end());
        if (counting_sort(a, max_val) != ref) {
            printf("FAIL counting rep=%d n=%d\n", rep, n);
            return 1;
        }

        // radix_sort: full signed range including 0, negatives, and large values.
        vector<int> r(n);
        for (auto& x: r) x = (int)(rng() ^ (rng() << 1));
        vector<int> ref_r = r;
        sort(ref_r.begin(), ref_r.end());
        radix_sort(r);
        if (r != ref_r) {
            printf("FAIL radix rep=%d n=%d\n", rep, n);
            return 1;
        }

        // radix_sort on multi-byte values sharing low bytes: exercises stability.
        int base = (int)(rng() % 7) - 3;
        vector<int> d(n);
        for (auto& x: d) x = base + (int)((rng() % 5) << 16);
        vector<int> ref_d = d;
        sort(ref_d.begin(), ref_d.end());
        radix_sort(d);
        if (d != ref_d) {
            printf("FAIL radix-dup rep=%d n=%d\n", rep, n);
            return 1;
        }
        checks += 3;
    }

    // Edge cases: empty and single element.
    vector<int> empty_v;
    if (not counting_sort(empty_v, 0).empty()) return 1;
    radix_sort(empty_v);
    if (not empty_v.empty()) return 1;
    vector<int> one = {-7};
    radix_sort(one);
    if (one != vector<int>{-7} or counting_sort({3}, 3) != vector<int>{3}) return 1;
    checks += 4;

    vector<int> extremes = {INT_MAX, 0, INT_MIN, -1, 1, INT_MAX, INT_MIN};
    vector<int> ref_extremes = extremes;
    sort(ref_extremes.begin(), ref_extremes.end());
    radix_sort(extremes);
    if (extremes != ref_extremes) {
        printf("FAIL radix extremes\n");
        return 1;
    }
    checks++;

    printf("counting-radix-sort PASS %lld", checks);
    return 0;
}
