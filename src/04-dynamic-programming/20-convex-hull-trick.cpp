/**
 * Convex hull trick (monotonic version): maintain the lower envelope of lines
 * y = m*x + b to answer "minimum value over all lines at x" queries. Both
 * add_line and query are amortized O(1), so a DP whose transition is
 * dp[i] = min_j (dp[j] + m_j*x_i) runs in O(n) total instead of O(n^2).
 *
 * Preconditions for the amortized-O(1) pointer form:
 *   - add_line is called with strictly decreasing slopes (equal slopes are
 *     folded, keeping the smaller intercept);
 *   - query is called with non-decreasing x, so the best-line pointer only
 *     ever advances.
 * Break either precondition and use the Li Chao tree instead
 * (src/02-data-structures/16-li-chao-tree.cpp): it takes lines and queries in
 * arbitrary order at O(log C) each.
 *
 * This is the MIN version. For maxima, feed negated lines and negate the
 * answer, or flip the `<=` in query to `>=` and reverse the slope precondition
 * to strictly increasing.
 */
struct ConvexHullTrick {
    vector<long long> slope, intercept;
    int pointer = 0;

    long long eval(int i, long long x) const {
        return slope[i] * x + intercept[i];
    }

    // Middle line b is redundant when its intersection with a already lies to
    // the right of a's intersection with c, so b never wins the lower envelope.
    bool redundant(int a, int b, int c) const {
        return (intercept[c] - intercept[a]) * (slope[a] - slope[b]) <=
               (intercept[b] - intercept[a]) * (slope[a] - slope[c]);
    }

    void add_line(long long m, long long b) {
        if (not slope.empty() and slope.back() == m) {
            if (intercept.back() <= b) return;
            slope.pop_back();
            intercept.pop_back();
        }
        slope.push_back(m);
        intercept.push_back(b);
        int last = slope.size();
        while (last >= 3 and redundant(last - 3, last - 2, last - 1)) {
            slope[last - 2] = slope[last - 1];
            intercept[last - 2] = intercept[last - 1];
            slope.pop_back();
            intercept.pop_back();
            last--;
        }
    }

    long long query(long long x) {
        if (pointer >= (int)slope.size()) pointer = slope.size() - 1;
        while (pointer + 1 < (int)slope.size() and eval(pointer + 1, x) <= eval(pointer, x))
            pointer++;
        return eval(pointer, x);
    }
};

/**
 * Demo: split a walk over sorted positions x[0..n-1] into hops, each hop
 * j -> i costing (x[i] - x[j])^2 plus a fixed toll, and minimize the total to
 * reach the last position. The transition
 *   dp[i] = min_{j<i} dp[j] + (x[i]-x[j])^2 + toll
 *         = x[i]^2 + toll + min_j ( (dp[j]+x[j]^2) + (-2*x[j])*x[i] )
 * is a line (slope -2*x[j], intercept dp[j]+x[j]^2) queried at x[i]: sorted x
 * makes slopes decreasing and queries increasing, so CHT applies directly.
 */
int main() {
    vector<long long> x = {0, 1, 3, 6, 10, 15};
    long long toll = 4;
    int n = x.size();

    ConvexHullTrick hull;
    vector<long long> dp(n);
    dp[0] = 0;
    hull.add_line(-2 * x[0], dp[0] + x[0] * x[0]);
    for (int i = 1; i < n; i++) {
        dp[i] = x[i] * x[i] + toll + hull.query(x[i]);
        hull.add_line(-2 * x[i], dp[i] + x[i] * x[i]);
    }

    cout << dp[n - 1] << "\n";  // -> 75
    return 0;
}
