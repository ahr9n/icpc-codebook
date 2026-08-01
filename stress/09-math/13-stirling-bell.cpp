// Stress against direct enumeration of permutations and restricted-growth
// strings, which independently represent cycles and set partitions.
vector<vector<long long>> brute_first(int limit) {
    vector<vector<long long>> count(limit + 1, vector<long long>(limit + 1, 0));
    count[0][0] = 1;
    for (int n = 1; n <= limit; n++) {
        vector<int> permutation(n);
        iota(permutation.begin(), permutation.end(), 0);
        do {
            vector<int> seen(n, 0);
            int cycles = 0;
            for (int start = 0; start < n; start++) {
                if (seen[start]) {
                    continue;
                }
                cycles++;
                int at = start;
                while (not seen[at]) {
                    seen[at] = 1;
                    at = permutation[at];
                }
            }
            count[n][cycles]++;
        } while (next_permutation(permutation.begin(), permutation.end()));
    }
    return count;
}

void enumerate_partitions(int index, int n, int max_label, vector<int>& label,
                          vector<vector<long long>>& count) {
    if (index == n) {
        count[n][max_label + 1]++;
        return;
    }
    for (int next = 0; next <= max_label + 1; next++) {
        label[index] = next;
        enumerate_partitions(index + 1, n, max(max_label, next), label, count);
    }
}

vector<vector<long long>> brute_second(int limit) {
    vector<vector<long long>> count(limit + 1, vector<long long>(limit + 1, 0));
    count[0][0] = 1;
    for (int n = 1; n <= limit; n++) {
        vector<int> label(n, 0);
        enumerate_partitions(1, n, 0, label, count);
    }
    return count;
}

int stress() {
    const int limit = 8;
    StirlingTables table = stirling_bell(limit);
    vector<vector<long long>> first = brute_first(limit);
    vector<vector<long long>> second = brute_second(limit);
    vector<long long> bell(limit + 1, 0);
    for (int n = 0; n <= limit; n++) {
        for (int groups = 0; groups <= n; groups++) {
            bell[n] += second[n][groups];
        }
    }

    mt19937 rng(913);
    long long checks = 0;
    for (int trial = 0; trial < 10000; trial++) {
        int n = rng() % (limit + 1);
        int groups = rng() % (limit + 1);
        if (table.first[n][groups] != first[n][groups] % MOD or
            table.second[n][groups] != second[n][groups] % MOD or table.bell[n] != bell[n] % MOD) {
            printf("FAIL n=%d groups=%d\n", n, groups);
            return 1;
        }
        checks++;
    }
    printf("stirling-bell PASS %lld", checks);
    return 0;
}
