/**
 * Bisection on a real monotonic function: ~100 halvings reach full double precision.
 * Use for continuous "minimum feasible value" problems. O(iters * check)
 */
double bisect_real(double lo, double hi, function<bool(double)> feasible) {
    for (int it = 0; it < 100; it++) {
        double mid = (lo + hi) / 2;
        if (feasible(mid))
            hi = mid;
        else
            lo = mid;
    }
    return lo;
}
