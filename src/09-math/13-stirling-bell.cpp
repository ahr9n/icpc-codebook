struct StirlingTables {
    vector<vector<long long>> first;
    vector<vector<long long>> second;
    vector<long long> bell;
};

/**
 * Stirling numbers and Bell numbers modulo MOD through order n.
 * first[i][k] is the unsigned Stirling number of the first kind: permutations
 * of i elements with exactly k cycles. second[i][k] counts partitions of i
 * labeled elements into k non-empty unlabeled sets. bell[i] counts all set
 * partitions of i elements. O(n^2) time and memory. Contract: n >= 0.
 */
StirlingTables stirling_bell(int n) {
    StirlingTables table;
    table.first.assign(n + 1, vector<long long>(n + 1, 0));
    table.second.assign(n + 1, vector<long long>(n + 1, 0));
    table.bell.assign(n + 1, 0);
    table.first[0][0] = 1;
    table.second[0][0] = 1;
    table.bell[0] = 1;

    for (int size = 1; size <= n; size++) {
        for (int groups = 1; groups <= size; groups++) {
            table.first[size][groups] =
                (table.first[size - 1][groups - 1] + (size - 1LL) * table.first[size - 1][groups]) %
                MOD;
            table.second[size][groups] =
                (table.second[size - 1][groups - 1] + groups * table.second[size - 1][groups]) %
                MOD;
            table.bell[size] = (table.bell[size] + table.second[size][groups]) % MOD;
        }
    }
    return table;
}

/** Example: selected Stirling numbers and Bell(5). */
int main() {
    StirlingTables table = stirling_bell(5);
    cout << table.first[5][2] << "\n";   // -> 50
    cout << table.second[5][3] << "\n";  // -> 25
    cout << table.bell[5] << "\n";       // -> 52
    return 0;
}
