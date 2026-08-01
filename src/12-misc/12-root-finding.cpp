/**
 * Newton and secant root finding for f(x) = 0. Newton uses f'(x) and converges
 * quadratically near a simple root; secant approximates the derivative from two
 * previous points and converges superlinearly without a derivative. Both use
 * O(iterations) function evaluations and O(1) memory.
 *
 * These are local, non-bracketing methods: convergence is not guaranteed.
 * Newton can fail at a zero derivative, and secant can fail when its two function
 * values coincide. A result is converged only when |f(root)| <= tolerance;
 * otherwise the returned point is merely the last finite iterate. Contract:
 * tolerance > 0, max_iterations >= 0, and f is reasonably scaled near the root.
 */
struct RootResult {
    long double root;
    bool converged;
    int iterations;
};

RootResult newton_root(function<long double(long double)> f,
                       function<long double(long double)> derivative, long double start,
                       long double tolerance, int max_iterations) {
    long double current = start;
    for (int iteration = 0; iteration < max_iterations; iteration++) {
        long double value = f(current);
        if (abs(value) <= tolerance) {
            return {current, true, iteration};
        }
        long double slope = derivative(current);
        if (slope == 0) {
            return {current, false, iteration};
        }
        long double next = current - value / slope;
        if (not isfinite(next)) {
            return {current, false, iteration};
        }
        current = next;
    }
    return {current, abs(f(current)) <= tolerance, max_iterations};
}

RootResult secant_root(function<long double(long double)> f, long double first, long double second,
                       long double tolerance, int max_iterations) {
    long double first_value = f(first);
    long double second_value = f(second);
    if (abs(first_value) <= tolerance) {
        return {first, true, 0};
    }

    for (int iteration = 0; iteration < max_iterations; iteration++) {
        if (abs(second_value) <= tolerance) {
            return {second, true, iteration};
        }
        long double denominator = second_value - first_value;
        if (denominator == 0) {
            return {second, false, iteration};
        }
        long double next = second - second_value * (second - first) / denominator;
        if (not isfinite(next)) {
            return {second, false, iteration};
        }
        first = second;
        first_value = second_value;
        second = next;
        second_value = f(second);
    }
    return {second, abs(second_value) <= tolerance, max_iterations};
}

/** Example: Newton finds sqrt(2), while secant finds cos(x) = x. */
int main() {
    const long double tolerance = 1e-12L;
    const int max_iterations = 100;

    RootResult square_root =
        newton_root([](long double x) { return x * x - 2; }, [](long double x) { return 2 * x; }, 1,
                    tolerance, max_iterations);
    RootResult fixed_point =
        secant_root([](long double x) { return cos(x) - x; }, 0, 1, tolerance, max_iterations);

    cout << square_root.converged << " " << fixed << setprecision(10) << square_root.root << "\n";
    cout << fixed_point.converged << " " << fixed << setprecision(10) << fixed_point.root << "\n";
    return 0;
}
// -> 1 1.4142135624
// -> 1 0.7390851332
