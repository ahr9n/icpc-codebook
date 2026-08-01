/**
 * Difference constraints: solve a system of x_v - x_u <= w for all constraints.
 *
 * A shortest-path pass keeps the invariant dist[v] <= dist[u] + w on every edge,
 * which is exactly x_v - x_u <= w once we read x_v := dist[v]. So model each
 * constraint x_v - x_u <= w as an edge u -> v of weight w, add a super-source with
 * a 0-weight edge to every variable (guaranteeing all are reachable), and run
 * Bellman-Ford from it. The resulting distances are one feasible assignment; a
 * negative cycle means the constraints sum to 0 < (a negative), a contradiction,
 * so the system is infeasible. The 0-weight super-source edges leave dist all 0
 * initially, so we relax the constraints directly without materializing it.
 *
 * add_constraint(u, v, w) encodes x_v - x_u <= w. To encode others:
 *   x_v - x_u >= w   as   add_constraint(v, u, -w)
 *   x_v - x_u == w   as   both add_constraint(u, v, w) and add_constraint(v, u, -w)
 *   x_v - x_u <  w   as   add_constraint(u, v, w - 1)   (integer variables only)
 * The returned assignment is the tightest one with every x <= 0. O(V * E)
 */
struct DifferenceConstraints {
    struct Constraint {
        int u, v;
        long long w;
    };
    int n;
    vector<Constraint> constraints;

    DifferenceConstraints(int n) : n(n) {}

    void add_constraint(int u, int v, long long w) {
        constraints.push_back({u, v, w});
    }

    pair<bool, vector<long long>> solve() {
        vector<long long> assignment(n, 0);

        for (int round = 0; round < n; round++) {
            bool changed = false;
            for (Constraint& c: constraints)
                if (assignment[c.u] + c.w < assignment[c.v]) {
                    assignment[c.v] = assignment[c.u] + c.w;
                    changed = true;
                }
            if (not changed) return {true, assignment};
        }

        for (Constraint& c: constraints)
            if (assignment[c.u] + c.w < assignment[c.v]) return {false, {}};
        return {true, assignment};
    }
};

/**
 * Example: a chain where each variable sits below the previous one.
 * A contradictory system (x1 - x0 <= 1 with x0 - x1 <= -2) instead returns
 * {false, {}}: those two edges form a negative cycle summing to -1.
 */
int main() {
    DifferenceConstraints dc(4);
    dc.add_constraint(0, 1, -2);  // x1 - x0 <= -2
    dc.add_constraint(1, 2, -3);  // x2 - x1 <= -3
    dc.add_constraint(0, 2, -4);  // x2 - x0 <= -4
    dc.add_constraint(2, 0, 10);  // x0 - x2 <= 10

    auto [feasible, x] = dc.solve();
    cout << feasible << "\n";
    for (long long xi: x) cout << xi << " ";
    cout << "\n";  // -> 1 \n 0 -2 -5 0
    return 0;
}
