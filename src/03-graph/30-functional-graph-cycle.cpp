/**
 * Cycle finding in a functional graph (every node has out-degree 1, so f is a
 * map [0,n) -> [0,n)). Following f from any start eventually loops: the walk is
 * a "rho" — a tail of length mu leading into a cycle of length lambda, entered
 * at node cycle_start. Floyd's tortoise-and-hare finds all three with two
 * pointers, one moving twice as fast, so no visited set is needed.
 * O(mu + lambda) time, O(1) memory.
 *
 * Contract: f[i] is a valid index for every i, and start is in range. A
 * self-loop gives lambda = 1; starting on the cycle gives mu = 0 and
 * cycle_start = start.
 *
 * Brent's algorithm computes the same {mu, lambda} with the same bounds but far
 * fewer applications of f (it doubles the hare's search window instead of
 * advancing the tortoise), which matters when evaluating f is expensive.
 */
struct RhoShape {
    int cycle_start;   // first node on the cycle reached from start
    long long mu;      // tail length: steps before entering the cycle
    long long lambda;  // cycle length
};

RhoShape find_cycle(const vector<int>& f, int start) {
    // Phase 1: the hare gains one step of lead per round, so it eventually laps
    // the tortoise inside the cycle; the collision node is somewhere on it.
    int tortoise = f[start];
    int hare = f[f[start]];
    while (tortoise != hare) {
        tortoise = f[tortoise];
        hare = f[f[hare]];
    }

    // Phase 2: start and the collision node are an exact number of laps apart,
    // so advancing both at unit speed makes them meet at the cycle entrance.
    long long mu = 0;
    tortoise = start;
    while (tortoise != hare) {
        tortoise = f[tortoise];
        hare = f[hare];
        mu++;
    }
    int cycle_start = tortoise;

    // Phase 3: hold one pointer on the entrance and walk the other around once.
    long long lambda = 1;
    hare = f[cycle_start];
    while (hare != cycle_start) {
        hare = f[hare];
        lambda++;
    }

    return {cycle_start, mu, lambda};
}

/**
 * Example: rho with tail 0 -> 1 -> 2 and cycle 2 -> 3 -> 4 -> 2 started at 0, so
 * the cycle begins at node 2 after a tail of length 2 and has length 3.
 */
int main() {
    vector<int> f = {1, 2, 3, 4, 2};
    RhoShape r = find_cycle(f, 0);
    cout << r.cycle_start << " " << r.mu << " " << r.lambda << "\n";  // -> 2 2 3
    return 0;
}
