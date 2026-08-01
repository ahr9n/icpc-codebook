/**
 * Prüfer sequence bijection between labeled trees on vertices 0..n-1 and
 * sequences of length n-2. prufer_encode repeatedly removes the smallest leaf;
 * prufer_decode reverses that process. O(n log n) time and O(n) extra memory.
 * Contract: encode receives a simple connected undirected tree with n >= 2 and
 * symmetric adjacency lists; every decode entry is in 0..code.size()+1.
 */
vector<int> prufer_encode(const vector<vector<int>>& tree) {
    int n = tree.size();
    vector<int> degree(n);
    priority_queue<int, vector<int>, greater<int>> leaves;
    for (int vertex = 0; vertex < n; vertex++) {
        degree[vertex] = tree[vertex].size();
        if (degree[vertex] == 1) {
            leaves.push(vertex);
        }
    }

    vector<int> code;
    for (int step = 0; step < n - 2; step++) {
        int leaf = leaves.top();
        leaves.pop();
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
        if (degree[parent] == 1) {
            leaves.push(parent);
        }
    }
    return code;
}

vector<vector<int>> prufer_decode(const vector<int>& code) {
    int n = code.size() + 2;
    vector<int> degree(n, 1);
    for (int vertex: code) {
        degree[vertex]++;
    }

    priority_queue<int, vector<int>, greater<int>> leaves;
    for (int vertex = 0; vertex < n; vertex++) {
        if (degree[vertex] == 1) {
            leaves.push(vertex);
        }
    }

    vector<vector<int>> tree(n);
    for (int parent: code) {
        int leaf = leaves.top();
        leaves.pop();
        tree[leaf].push_back(parent);
        tree[parent].push_back(leaf);
        degree[leaf]--;
        degree[parent]--;
        if (degree[parent] == 1) {
            leaves.push(parent);
        }
    }

    int first = leaves.top();
    leaves.pop();
    int second = leaves.top();
    tree[first].push_back(second);
    tree[second].push_back(first);
    return tree;
}

/** Example: decode {3,3,3,4} into its six-vertex labeled tree. */
int main() {
    vector<int> code = {3, 3, 3, 4};
    vector<vector<int>> tree = prufer_decode(code);
    for (int vertex = 0; vertex < (int)tree.size(); vertex++) {
        for (int neighbor: tree[vertex]) {
            if (vertex < neighbor) {
                cout << vertex << "-" << neighbor << " ";
            }
        }
    }
    cout << "\n";  // -> 0-3 1-3 2-3 3-4 4-5
    return 0;
}
