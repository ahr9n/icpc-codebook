/**
 * Expected-value DP via the law of total expectation: the expected cost from a
 * state is one step plus the probability-weighted expected cost of the states
 * it reaches. Solve states in an order where every transition points at
 * already-solved states (here: cells nearer the goal first).
 *
 *   E[i] = 1 + Σ_f  p_f * E[next(i, f)]        with E[goal] = 0
 *
 * When a state can transition back to itself with probability stay_prob, do not
 * recurse into it — isolate that term algebraically:
 *
 *   E[i] = (1 + Σ_{j≠i} p_ij * E[j]) / (1 - stay_prob)
 *
 * Both demos count expected fair-die throws on a linear board of length n.
 * O(n * faces) time, O(n) space.
 */

// Overshooting the last cell still finishes the walk (no self-loop).
double expected_throws_to_finish(int n, int faces) {
    vector<double> expected(n + 1, 0.0);
    double face_prob = 1.0 / faces;

    for (int i = n - 1; i >= 0; i--) {
        double reachable = 0.0;
        for (int f = 1; f <= faces; f++) {
            int next = i + f;
            if (next < n) reachable += expected[next];
        }
        expected[i] = 1.0 + face_prob * reachable;
    }
    return expected[0];
}

// Overshooting the goal keeps you in place: a self-loop resolved by the algebra
// above (goal must be landed on exactly).
double expected_throws_land_exact(int n, int faces) {
    vector<double> expected(n + 1, 0.0);
    double face_prob = 1.0 / faces;

    for (int i = n - 1; i >= 0; i--) {
        double reachable = 0.0;
        double stay_prob = 0.0;
        for (int f = 1; f <= faces; f++) {
            int next = i + f;
            if (next <= n)
                reachable += expected[next];
            else
                stay_prob += face_prob;
        }
        expected[i] = (1.0 + face_prob * reachable) / (1.0 - stay_prob);
    }
    return expected[0];
}

int main() {
    cout << fixed << setprecision(4);
    cout << "reach cell >= 10 : " << expected_throws_to_finish(10, 6) << "\n";
    cout << "land exactly 10  : " << expected_throws_land_exact(10, 6) << "\n";
    return 0;
}
// -> reach cell >= 10 : 3.3237
// -> land exactly 10  : 7.5880
