/**
 * Fenwick / BIT: point update + prefix sum in O(log n). External index is 0-based.
 * range(l, r) = prefix(r) - prefix(l - 1). Use for prefix aggregates, inversions.
 */
struct BIT {
    int n;
    vector<long long> tree;

    BIT(int n) : n(n), tree(n + 1, 0) {}

    void add(int i, long long val) {
        for (i++; i <= n; i += i & -i) tree[i] += val;
    }

    long long prefix(int i) {
        long long sum = 0;
        for (i++; i > 0; i -= i & -i) sum += tree[i];
        return sum;
    }

    long long range(int l, int r) {
        return prefix(r) - prefix(l - 1);
    }
};

/**
 * Example: add values 1..5, then read a prefix sum and a range sum.
 */
int main() {
    BIT bit(5);
    for (int i = 0; i < 5; i++) bit.add(i, i + 1);
    cout << bit.prefix(2) << "\n";    // -> 6
    cout << bit.range(1, 3) << "\n";  // -> 9
    return 0;
}
