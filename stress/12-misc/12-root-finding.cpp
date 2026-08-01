// Stress: Newton and secant roots of x^2-value vs a high-iteration bisection
// oracle, plus deterministic zero-derivative and equal-value failure cases.
int stress() {
    mt19937 rng(12);
    const long double tolerance = 1e-10L;
    const int max_iterations = 200;
    long long checks = 0;

    for (int rep = 0; rep < 5000; rep++) {
        long double value = ((long double)(1 + rng() % 1000000)) / 100;
        auto f = [=](long double x) { return x * x - value; };
        auto derivative = [](long double x) { return 2 * x; };

        long double lo = 0;
        long double starting_hi = max((long double)1, value);
        long double hi = starting_hi;
        for (int iteration = 0; iteration < 250; iteration++) {
            long double mid = (lo + hi) / 2;
            if (f(mid) < 0) {
                lo = mid;
            } else {
                hi = mid;
            }
        }
        long double want = (lo + hi) / 2;

        RootResult newton = newton_root(f, derivative, starting_hi, tolerance, max_iterations);
        RootResult secant = secant_root(f, 0, starting_hi, tolerance, max_iterations);
        long double allowed_error = 1e-10L * max((long double)1, want);
        if (not newton.converged or abs(newton.root - want) > allowed_error) {
            printf("root-finding FAIL(newton) rep=%d value=%.18Lf root=%.18Lf residual=%.18Lf "
                   "iterations=%d\n",
                   rep, value, newton.root, f(newton.root), newton.iterations);
            return 1;
        }
        checks++;
        if (not secant.converged or abs(secant.root - want) > allowed_error) {
            printf("root-finding FAIL(secant) rep=%d value=%.18Lf root=%.18Lf residual=%.18Lf "
                   "iterations=%d\n",
                   rep, value, secant.root, f(secant.root), secant.iterations);
            return 1;
        }
        checks++;
    }

    RootResult flat =
        newton_root([](long double x) { return x * x + 1; }, [](long double x) { return 2 * x; }, 0,
                    tolerance, max_iterations);
    if (flat.converged) {
        printf("root-finding FAIL(zero-derivative)\n");
        return 1;
    }
    checks++;

    RootResult equal_values =
        secant_root([](long double x) { return x * x + 1; }, -1, 1, tolerance, max_iterations);
    if (equal_values.converged) {
        printf("root-finding FAIL(equal-values)\n");
        return 1;
    }
    checks++;

    printf("root-finding PASS %lld", checks);
    return 0;
}
