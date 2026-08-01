int n;
struct Edge {
    int from, to;
    long long cost;
};
vector<Edge> edges;

/**
 * Karp's minimum mean cycle: the smallest possible (sum of edge weights) / (edge
 * count) over every directed cycle. Self-loops count as length-1 cycles; edge
 * weights may be negative. O(V*E).
 *
 * dp[k][v] = min weight of a walk of EXACTLY k edges ending at v, started from a
 * virtual all-sources vertex (dp[0][*] = 0). Any walk of n edges repeats a
 * vertex, so a finite dp[n][*] certifies a cycle; a DAG leaves them all LINF.
 * Karp's theorem: min mean = min over v of max over k in [0, n-1] of
 * (dp[n][v] - dp[k][v]) / (n - k).
 *
 * Returns false and leaves `mean` untouched when the graph has no cycle. To
 * recover the cycle, remember the argmin v* and the argmax k*, then walk the dp
 * predecessors back from (n, v*) to layer k* and read the repeated vertex.
 */
bool min_mean_cycle(long double& mean) {
    vector<vector<long long>> dp(n + 1, vector<long long>(n, LINF));
    for (int v = 0; v < n; v++) {
        dp[0][v] = 0;
    }

    for (int k = 1; k <= n; k++) {
        for (Edge& e: edges) {
            if (dp[k - 1][e.from] < LINF) {
                dp[k][e.to] = min(dp[k][e.to], dp[k - 1][e.from] + e.cost);
            }
        }
    }

    bool found = false;
    long double best = 0;

    for (int v = 0; v < n; v++) {
        if (dp[n][v] >= LINF) {
            continue;
        }

        long double worst = -1;
        bool seen = false;
        for (int k = 0; k < n; k++) {
            if (dp[k][v] >= LINF) {
                continue;
            }
            long double avg = (long double)(dp[n][v] - dp[k][v]) / (n - k);
            if (not seen or avg > worst) {
                worst = avg;
                seen = true;
            }
        }

        if (not found or worst < best) {
            best = worst;
            found = true;
        }
    }

    if (not found) {
        return false;
    }
    mean = best;
    return true;
}

/** Example: choose the unit-mean 2-cycle over a cycle of mean two. */
int main() {
    n = 4;
    edges.clear();
    edges.push_back({0, 1, 1});
    edges.push_back({1, 2, 2});
    edges.push_back({2, 0, 3});
    edges.push_back({2, 3, 1});
    edges.push_back({3, 2, 1});

    long double mean;
    bool has_cycle = min_mean_cycle(mean);
    cout << has_cycle << " " << fixed << setprecision(4) << mean << "\n";
    return 0;
}
// -> 1 1.0000
