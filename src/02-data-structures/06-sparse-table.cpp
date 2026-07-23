/**
 * Sparse table: O(1) range query for idempotent ops (min/max/gcd) after O(n log n)
 * build. NOT for sum. query on inclusive [l, r]; here it answers range minimum.
 */
struct SparseTable {
    vector<vector<long long>> table;

    SparseTable(vector<long long>& a) {
        int n = a.size(), logn = 1;
        while ((1 << logn) < n) logn++;
        table.assign(logn + 1, vector<long long>(n));
        table[0] = a;
        for (int k = 1; k <= logn; k++)
            for (int i = 0; i + (1 << k) <= n; i++)
                table[k][i] = min(table[k - 1][i], table[k - 1][i + (1 << (k - 1))]);
    }

    long long query(int l, int r) {
        int k = __lg(r - l + 1);
        return min(table[k][l], table[k][r - (1 << k) + 1]);
    }
};

/**
 * Example: build over an array, then answer two inclusive range-min queries.
 */
int main() {
    vector<long long> a = {5, 2, 8, 1, 9, 3};
    SparseTable st(a);
    cout << st.query(0, 2) << "\n";  // -> 2
    cout << st.query(3, 5) << "\n";  // -> 1
    return 0;
}
