// Stress: GNU pb_ds ordered_set insert/erase/find_by_order/order_of_key vs std::set.
int stress() {
    mt19937 rng(13);
    long long checks = 0;

    for (int rep = 0; rep < 4000; rep++) {
        OrderedSet tree;
        set<int> brute;

        for (int step = 0; step < 50; step++) {
            int value = (int)(rng() % 25) - 12;
            if (rng() % 2 == 0) {
                tree.insert(value);
                brute.insert(value);
            } else {
                tree.erase(value);
                brute.erase(value);
            }

            vector<int> sorted(brute.begin(), brute.end());
            if (tree.size() != sorted.size()) {
                printf("order-statistics-tree FAIL(size) rep=%d step=%d\n", rep, step);
                return 1;
            }

            for (int k = 0; k < (int)sorted.size(); k++) {
                if (*tree.find_by_order(k) != sorted[k]) {
                    printf("order-statistics-tree FAIL(kth) rep=%d step=%d k=%d\n", rep, step, k);
                    return 1;
                }
                checks++;
            }
            if (tree.find_by_order(sorted.size()) != tree.end()) {
                printf("order-statistics-tree FAIL(end) rep=%d step=%d\n", rep, step);
                return 1;
            }
            checks++;

            for (int key = -14; key <= 14; key++) {
                int ref = lower_bound(sorted.begin(), sorted.end(), key) - sorted.begin();
                if ((int)tree.order_of_key(key) != ref) {
                    printf("order-statistics-tree FAIL(rank) rep=%d step=%d key=%d\n", rep, step,
                           key);
                    return 1;
                }
                checks++;
            }
        }
    }

    printf("order-statistics-tree PASS %lld", checks);
    return 0;
}
