long long matrix_power(long long base, long long exponent) {
    long long result = 1;
    while (exponent > 0) {
        if (exponent & 1) {
            result = result * base % MOD;
        }
        base = base * base % MOD;
        exponent >>= 1;
    }
    return result;
}

/**
 * Kirchhoff's matrix-tree theorem counts spanning trees of an undirected
 * multigraph modulo the prime MOD. Build its Laplacian (degree on the diagonal,
 * minus edge multiplicity off-diagonal), delete any one row and column, and
 * take the resulting determinant. Parallel edges count as distinct choices;
 * self-loops do not affect a spanning tree. O(n^3 + E) time and O(n^2) memory.
 * Contract: n >= 1, all endpoints are in 0..n-1, and MOD is prime.
 */
long long count_spanning_trees(int n, const vector<pair<int, int>>& edges) {
    if (n == 1) {
        return 1;
    }

    vector<vector<long long>> matrix(n - 1, vector<long long>(n - 1, 0));
    for (auto [u, v]: edges) {
        if (u == v) {
            continue;
        }
        if (u < n - 1) {
            matrix[u][u] = (matrix[u][u] + 1) % MOD;
        }
        if (v < n - 1) {
            matrix[v][v] = (matrix[v][v] + 1) % MOD;
        }
        if (u < n - 1 and v < n - 1) {
            matrix[u][v] = (matrix[u][v] - 1 + MOD) % MOD;
            matrix[v][u] = (matrix[v][u] - 1 + MOD) % MOD;
        }
    }

    long long determinant = 1;
    for (int col = 0; col < n - 1; col++) {
        int pivot = col;
        while (pivot < n - 1 and matrix[pivot][col] == 0) {
            pivot++;
        }
        if (pivot == n - 1) {
            return 0;
        }
        if (pivot != col) {
            swap(matrix[pivot], matrix[col]);
            determinant = (MOD - determinant) % MOD;
        }

        long long pivot_value = matrix[col][col];
        determinant = determinant * pivot_value % MOD;
        long long inverse = matrix_power(pivot_value, MOD - 2);
        for (int row = col + 1; row < n - 1; row++) {
            long long factor = matrix[row][col] * inverse % MOD;
            for (int j = col; j < n - 1; j++) {
                matrix[row][j] = (matrix[row][j] - factor * matrix[col][j] % MOD + MOD) % MOD;
            }
        }
    }
    return determinant;
}

/** Example: K4 has 16 spanning trees; a parallel edge raises a triangle to 5. */
int main() {
    vector<pair<int, int>> complete_graph;
    for (int u = 0; u < 4; u++) {
        for (int v = u + 1; v < 4; v++) {
            complete_graph.push_back({u, v});
        }
    }
    cout << count_spanning_trees(4, complete_graph) << "\n";
    cout << count_spanning_trees(3, {{0, 1}, {0, 1}, {1, 2}, {0, 2}}) << "\n";
    return 0;
}
// -> 16
// -> 5
