// Stress: Treap multiset (insert/erase/kth/order_of_key/size) vs a sorted vector.
int stress() {
    mt19937 rng(12345);
    long long checks = 0;
    for (int rep = 0; rep < 4000; rep++) {
        Treap treap;
        vector<long long> brute;  // kept sorted, mirrors the multiset
        int ops = (int)(rng() % 40) + 1;
        for (int step = 0; step < ops; step++) {
            int kind = rng() % 2;
            long long key = (long long)(rng() % 15) - 3;  // narrow range forces duplicates

            if (kind == 0 or brute.empty()) {
                treap.insert(key);
                brute.insert(lower_bound(brute.begin(), brute.end(), key), key);
            } else {
                bool removed = treap.erase(key);
                auto it = lower_bound(brute.begin(), brute.end(), key);
                bool ref_removed = (it != brute.end() and *it == key);
                if (ref_removed) brute.erase(it);
                if (removed != ref_removed) {
                    printf("FAIL erase key=%lld got=%d ref=%d\n", key, removed, ref_removed);
                    return 1;
                }
            }

            if (treap.size() != (int)brute.size()) {
                printf("FAIL size got=%d ref=%d\n", treap.size(), (int)brute.size());
                return 1;
            }

            int sz = (int)brute.size();
            for (int k = 0; k <= sz; k++) {  // k == sz is out of range -> LINF
                long long got = treap.kth(k);
                long long ref = (k < sz) ? brute[k] : LINF;
                if (got != ref) {
                    printf("FAIL kth k=%d got=%lld ref=%lld\n", k, got, ref);
                    return 1;
                }
                checks++;
            }

            for (long long x = -5; x <= 13; x++) {
                int got = treap.order_of_key(x);
                int ref = (int)(lower_bound(brute.begin(), brute.end(), x) - brute.begin());
                if (got != ref) {
                    printf("FAIL order_of_key x=%lld got=%d ref=%d\n", x, got, ref);
                    return 1;
                }
                checks++;
            }
        }
    }
    printf("treap PASS %lld", checks);
    return 0;
}
