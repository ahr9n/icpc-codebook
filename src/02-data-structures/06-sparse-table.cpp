/**
 * Sparse table: O(1) range query for idempotent ops (min/max/gcd). O(n log n) build.
 * table[i][j] = op over a[i .. i + 2^j - 1]. NOT for sum. query is inclusive [l, r].
 */
struct SparseTable {
    vector<vector<long long>> table;

    SparseTable(vector<long long>& a) {
        int n = a.size();
        if (n == 0) return;

        int logn = __lg(n) + 1;
        table.assign(n, vector<long long>(logn));
        for (int i = 0; i < n; i++) table[i][0] = a[i];
        for (int j = 1; (1 << j) <= n; j++)
            for (int i = 0; i + (1 << j) <= n; i++)
                table[i][j] = min(table[i][j - 1], table[i + (1 << (j - 1))][j - 1]);
    }

    long long query(int l, int r) {
        int j = __lg(r - l + 1);
        return min(table[l][j], table[r - (1 << j) + 1][j]);
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
