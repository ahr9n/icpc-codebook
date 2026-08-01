// Recover random low-degree polynomials from values at distinct points and
// compare interpolation with direct coefficient evaluation.
int stress() {
    mt19937 rng(909);
    long long checks = 0;

    auto evaluate = [](const vector<long long>& coefficient, long long at) {
        long long value = 0;
        at = (at % MOD + MOD) % MOD;
        for (int i = (int)coefficient.size() - 1; i >= 0; i--) {
            value = (value * at + coefficient[i]) % MOD;
        }
        return value;
    };

    for (int rep = 0; rep < 5000; rep++) {
        int n = 1 + (int)(rng() % 9);
        vector<long long> coefficient(n);
        for (long long& value: coefficient) {
            value = rng() % 100000;
        }

        vector<long long> x;
        set<long long> used;
        while ((int)x.size() < n) {
            long long candidate = (long long)(int)(rng() % 101) - 50;
            long long normalized = (candidate % MOD + MOD) % MOD;
            if (used.insert(normalized).second) {
                x.push_back(candidate);
            }
        }
        vector<long long> y(n);
        for (int i = 0; i < n; i++) {
            y[i] = evaluate(coefficient, x[i]);
        }

        for (int trial = 0; trial < 20; trial++) {
            long long target = (long long)(int)(rng() % 201) - 100;
            long long expected = evaluate(coefficient, target);
            long long actual = lagrange_interpolate(x, y, target);
            if (actual != expected) {
                printf("lagrange FAIL rep=%d trial=%d expected=%lld actual=%lld\n", rep, trial,
                       expected, actual);
                return 1;
            }
            checks++;
        }
    }

    printf("lagrange-interpolation PASS %lld", checks);
    return 0;
}
