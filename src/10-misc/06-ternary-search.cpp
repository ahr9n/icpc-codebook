/**
 * Ternary search: minimize a strictly unimodal function f over [lo, hi] (reals).
 * For a maximum, flip the comparison. ~200 iters reach full precision.
 */
double ternary_search(double lo, double hi, function<double(double)> f) {
    for (int it = 0; it < 200; it++) {
        double m1 = lo + (hi - lo) / 3, m2 = hi - (hi - lo) / 3;
        if (f(m1) < f(m2))
            hi = m2;
        else
            lo = m1;
    }
    return lo;
}
