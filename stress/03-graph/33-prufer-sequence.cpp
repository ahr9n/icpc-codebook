// Stress the bijection exhaustively for small codes and against a slow encoder
// that scans all vertices to choose each next leaf.
vector<int> slow_encode(vector<vector<int>> tree) {
    int n = tree.size();
    vector<int> degree(n);
    for (int vertex = 0; vertex < n; vertex++) {
        degree[vertex] = tree[vertex].size();
    }

    vector<int> code;
    for (int step = 0; step < n - 2; step++) {
        int leaf = 0;
        while (degree[leaf] != 1) {
            leaf++;
        }
        int parent = -1;
        for (int neighbor: tree[leaf]) {
            if (degree[neighbor] > 0) {
                parent = neighbor;
                break;
            }
        }
        code.push_back(parent);
        degree[leaf] = 0;
        degree[parent]--;
    }
    return code;
}

bool valid_tree(const vector<vector<int>>& tree) {
    int n = tree.size();
    int degree_sum = 0;
    for (const vector<int>& neighbors: tree) {
        degree_sum += neighbors.size();
    }
    if (degree_sum != 2 * (n - 1)) {
        return false;
    }
    vector<int> seen(n, 0);
    queue<int> bfs;
    bfs.push(0);
    seen[0] = 1;
    while (not bfs.empty()) {
        int vertex = bfs.front();
        bfs.pop();
        for (int neighbor: tree[vertex]) {
            if (not seen[neighbor]) {
                seen[neighbor] = 1;
                bfs.push(neighbor);
            }
        }
    }
    return accumulate(seen.begin(), seen.end(), 0) == n;
}

int stress() {
    mt19937 rng(333);
    long long checks = 0;

    for (int n = 2; n <= 6; n++) {
        long long code_count = 1;
        for (int i = 0; i < n - 2; i++) {
            code_count *= n;
        }
        for (long long mask = 0; mask < code_count; mask++) {
            long long value = mask;
            vector<int> code(n - 2);
            for (int& vertex: code) {
                vertex = value % n;
                value /= n;
            }
            vector<vector<int>> tree = prufer_decode(code);
            if (not valid_tree(tree) or prufer_encode(tree) != code or slow_encode(tree) != code) {
                printf("FAIL exhaustive n=%d mask=%lld\n", n, mask);
                return 1;
            }
            checks++;
        }
    }

    for (int trial = 0; trial < 10000; trial++) {
        int n = 2 + rng() % 60;
        vector<int> code(n - 2);
        for (int& vertex: code) {
            vertex = rng() % n;
        }
        vector<vector<int>> tree = prufer_decode(code);
        if (not valid_tree(tree) or prufer_encode(tree) != code or slow_encode(tree) != code) {
            printf("FAIL random trial=%d n=%d\n", trial, n);
            return 1;
        }
        checks++;
    }
    printf("prufer-sequence PASS %lld", checks);
    return 0;
}
