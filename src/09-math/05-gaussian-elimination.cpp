/**
 * Gaussian elimination with partial pivoting: solve A x = b for a square system
 * with a unique solution. Each column selects the largest-magnitude pivot (for
 * numerical stability), then eliminates that variable from every other row,
 * leaving a diagonal system to read off. The product of the pivots is det(A),
 * so a (near-)zero pivot flags a singular / rank-deficient matrix. O(n^3).
 */
const double EPS = 1e-9;

vector<double> gauss_solve(vector<vector<double>> a, vector<double> b) {
    int n = a.size();
    for (int col = 0; col < n; col++) {
        int pivot = col;
        for (int i = col + 1; i < n; i++)
            if (fabs(a[i][col]) > fabs(a[pivot][col])) pivot = i;
        swap(a[col], a[pivot]);
        swap(b[col], b[pivot]);
        for (int i = 0; i < n; i++) {
            if (i == col) continue;
            double factor = a[i][col] / a[col][col];
            for (int j = col; j < n; j++) a[i][j] -= factor * a[col][j];
            b[i] -= factor * b[col];
        }
    }
    vector<double> x(n);
    for (int i = 0; i < n; i++) x[i] = b[i] / a[i][i];
    return x;
}

/**
 * Example: solve {2x + y = 5, x - y = 1}; the unique solution is x = 2, y = 1.
 */
int main() {
    vector<vector<double>> a = {{2, 1}, {1, -1}};
    vector<double> b = {5, 1};
    vector<double> x = gauss_solve(a, b);
    cout << fixed << setprecision(2) << x[0] << " " << x[1] << "\n";  // -> 2.00 1.00
    return 0;
}
