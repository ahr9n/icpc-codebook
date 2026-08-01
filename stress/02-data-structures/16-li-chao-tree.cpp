// Stress: LiChao lower-envelope query vs brute min over every inserted line.
int stress() {
    mt19937 rng(11);
    long long checks = 0;
    for (int rep = 0; rep < 4000; rep++) {
        long long x_lo = -(long long)(rng() % 40) - 1;
        long long x_hi = (long long)(rng() % 40) + 1;
        LiChao tree(x_lo, x_hi);
        vector<Line> inserted;
        int ops = 1 + rng() % 30;
        for (int o = 0; o < ops; o++) {
            if (inserted.empty() or rng() % 2 == 0) {
                Line line{(long long)(rng() % 101) - 50, (long long)(rng() % 101) - 50};
                inserted.push_back(line);
                tree.add_line(line);
                continue;
            }
            long long x = x_lo + (long long)(rng() % (x_hi - x_lo + 1));
            long long ref = LINF;
            for (auto line: inserted) ref = min(ref, line.eval(x));
            if (tree.query(x) != ref) {
                printf("FAIL x=%lld got=%lld want=%lld\n", x, tree.query(x), ref);
                return 1;
            }
            checks++;
        }
    }
    printf("li-chao-tree PASS %lld", checks);
    return 0;
}
