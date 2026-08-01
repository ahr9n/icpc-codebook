struct DirectedEdge {
    int from, to;
    long long cost;
};

/**
 * Chu–Liu–Edmonds finds a minimum-cost arborescence rooted at `root`: every
 * other vertex has one incoming edge and is reachable from the root. Choose the
 * cheapest incoming edge per vertex; if those edges contain a cycle, contract
 * it and subtract each destination's chosen cost from edges entering the new
 * super-vertex. Repeating until no cycle remains takes O(VE) time and O(V+E)
 * memory. Edge costs may be negative and parallel edges are allowed.
 *
 * Returns {false, 0} when some vertex cannot be reached by any arborescence;
 * otherwise returns {true, minimum_cost}. Contract: n >= 1, 0 <= root < n,
 * edge endpoints are valid, and all reduced costs and cost sums fit long long.
 */
pair<bool, long long> directed_mst(int n, int root, vector<DirectedEdge> edges) {
    long long answer = 0;

    while (true) {
        vector<long long> incoming(n, 0);
        vector<int> parent(n, -1);
        for (const DirectedEdge& edge: edges) {
            if (edge.from != edge.to and (parent[edge.to] == -1 or edge.cost < incoming[edge.to])) {
                incoming[edge.to] = edge.cost;
                parent[edge.to] = edge.from;
            }
        }
        incoming[root] = 0;
        parent[root] = root;
        for (int vertex = 0; vertex < n; vertex++) {
            if (parent[vertex] == -1) {
                return {false, 0};
            }
        }

        int component_count = 0;
        vector<int> component(n, -1);
        vector<int> seen(n, -1);
        for (int start = 0; start < n; start++) {
            answer += incoming[start];
            int vertex = start;
            while (seen[vertex] != start and component[vertex] == -1 and vertex != root) {
                seen[vertex] = start;
                vertex = parent[vertex];
            }
            if (vertex != root and component[vertex] == -1) {
                component[vertex] = component_count;
                for (int u = parent[vertex]; u != vertex; u = parent[u]) {
                    component[u] = component_count;
                }
                component_count++;
            }
        }

        if (component_count == 0) {
            return {true, answer};
        }
        for (int vertex = 0; vertex < n; vertex++) {
            if (component[vertex] == -1) {
                component[vertex] = component_count++;
            }
        }

        for (DirectedEdge& edge: edges) {
            int old_to = edge.to;
            edge.from = component[edge.from];
            edge.to = component[edge.to];
            if (edge.from != edge.to) {
                edge.cost -= incoming[old_to];
            }
        }
        root = component[root];
        n = component_count;
    }
}

/** Example: the cheapest rooted arborescence costs 9. */
int main() {
    vector<DirectedEdge> edges = {{0, 1, 5}, {0, 2, 4}, {1, 2, 1}, {2, 1, 2}, {1, 3, 3}, {2, 3, 6}};
    auto [exists, cost] = directed_mst(4, 0, edges);
    cout << exists << " " << cost << "\n";
    return 0;
}
// -> 1 9
