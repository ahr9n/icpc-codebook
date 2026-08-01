// Stress: ConvexHullTrick two ways. (1) minimum over lines at each query point vs
// a direct min-over-lines brute, respecting the decreasing-slope / increasing-x
// preconditions. (2) the hop-DP from the demo vs a plain O(n^2) DP.
int stress() {
    mt19937 rng(20);
    long long checks = 0;

    // (1) Envelope minimum vs brute over all inserted lines.
    for (int trial = 0; trial < 3000; trial++) {
        int lines = 1 + rng() % 30;
        vector<long long> slope(lines), intercept(lines);
        long long m = (long long)(rng() % 41) + 20;  // start high, strictly decrease
        ConvexHullTrick hull;
        for (int i = 0; i < lines; i++) {
            m -= 1 + rng() % 5;
            long long b = (long long)(rng() % 401) - 200;
            slope[i] = m;
            intercept[i] = b;
            hull.add_line(m, b);
        }

        int queries = 1 + rng() % 30;
        long long x = (long long)(rng() % 21) - 10;
        for (int q = 0; q < queries; q++) {
            x += rng() % 4;  // non-decreasing
            long long ref = LINF;
            for (int i = 0; i < lines; i++) ref = min(ref, slope[i] * x + intercept[i]);
            if (hull.query(x) != ref) {
                printf("FAIL envelope trial=%d x=%lld\n", trial, x);
                return 1;
            }
            checks++;
        }
    }

    // (2) CHT-accelerated hop DP vs plain O(n^2) DP.
    for (int trial = 0; trial < 2000; trial++) {
        int n = 1 + rng() % 20;
        vector<long long> x(n);
        long long pos = 0;
        for (int i = 0; i < n; i++) {
            pos += rng() % 5;  // sorted positions
            x[i] = pos;
        }
        long long toll = rng() % 10;

        ConvexHullTrick hull;
        vector<long long> dp(n);
        dp[0] = 0;
        hull.add_line(-2 * x[0], dp[0] + x[0] * x[0]);
        for (int i = 1; i < n; i++) {
            dp[i] = x[i] * x[i] + toll + hull.query(x[i]);
            hull.add_line(-2 * x[i], dp[i] + x[i] * x[i]);
        }

        vector<long long> ref(n, LINF);
        ref[0] = 0;
        for (int i = 1; i < n; i++)
            for (int j = 0; j < i; j++)
                ref[i] = min(ref[i], ref[j] + (x[i] - x[j]) * (x[i] - x[j]) + toll);

        if (dp[n - 1] != ref[n - 1]) {
            printf("FAIL dp trial=%d got=%lld ref=%lld\n", trial, dp[n - 1], ref[n - 1]);
            return 1;
        }
        checks++;
    }

    // Wide coefficients make the cross products exceed 64 bits while every
    // queried line value remains representable.
    for (int trial = 0; trial < 2000; trial++) {
        int lines = 3 + rng() % 20;
        vector<long long> slope(lines), intercept(lines);
        long long m = 5000000000000LL;
        ConvexHullTrick hull;
        for (int i = 0; i < lines; i++) {
            m -= 100000000000LL + (long long)(rng() % 500000) * 1000000;
            long long b = ((long long)(rng() % 8000001) - 4000000) * 1000000;
            slope[i] = m;
            intercept[i] = b;
            hull.add_line(m, b);
        }

        for (long long x = -3; x <= 3; x++) {
            long long ref = LINF;
            for (int i = 0; i < lines; i++) {
                ref = min(ref, slope[i] * x + intercept[i]);
            }
            if (hull.query(x) != ref) {
                printf("FAIL wide trial=%d x=%lld\n", trial, x);
                return 1;
            }
            checks++;
        }
    }

    printf("convex-hull-trick PASS %lld", checks);
    return 0;
}
