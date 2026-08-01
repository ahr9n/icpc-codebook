/**
 * Stoer–Wagner finds the minimum-weight cut of an undirected graph without
 * choosing source and sink. In each maximum-adjacency phase, the last vertex
 * added defines a cut; merging it into the previous vertex preserves every
 * other candidate cut for later phases. O(n^3) time and O(n^2) memory.
 *
 * weights[u][v] must be symmetric and non-negative; parallel edge weights are
 * added together and diagonal entries are ignored. The matrix must be square,
 * and all accumulated weights must fit long long. Returns 0 for n < 2 or a
 * disconnected graph.
 */
long long global_min_cut(vector<vector<long long>> weights) {
    int n = weights.size();
    if (n < 2) {
        return 0;
    }

    vector<int> active(n);
    iota(active.begin(), active.end(), 0);
    long long answer = LLONG_MAX;

    while (active.size() > 1) {
        vector<long long> connection(n, 0);
        vector<bool> added(n, false);
        int previous = -1;

        for (int step = 0; step < (int)active.size(); step++) {
            int selected = -1;
            for (int vertex: active) {
                if (not added[vertex] and
                    (selected == -1 or connection[vertex] > connection[selected])) {
                    selected = vertex;
                }
            }

            if (step + 1 == (int)active.size()) {
                answer = min(answer, connection[selected]);
                for (int vertex: active) {
                    if (vertex == selected or vertex == previous) {
                        continue;
                    }
                    weights[previous][vertex] += weights[selected][vertex];
                    weights[vertex][previous] = weights[previous][vertex];
                }
                active.erase(find(active.begin(), active.end(), selected));
                break;
            }

            added[selected] = true;
            previous = selected;
            for (int vertex: active) {
                if (not added[vertex]) {
                    connection[vertex] += weights[selected][vertex];
                }
            }
        }
    }
    return answer;
}

/** Example: every minimum cut of the unweighted K4 has weight 3. */
int main() {
    vector<vector<long long>> weights(4, vector<long long>(4, 0));
    for (int u = 0; u < 4; u++) {
        for (int v = u + 1; v < 4; v++) {
            weights[u][v] = weights[v][u] = 1;
        }
    }
    cout << global_min_cut(weights) << "\n";
    return 0;
}
// -> 3
