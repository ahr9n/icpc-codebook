/**
 * Inversion count: pairs i < j with a[i] > a[j], counted while merging. O(n log n)
 * Half-open [l, r); mutates a into sorted order. Call as inversions(a, 0, a.size()).
 */
long long inversions(vector<int>& a, int l, int r) {
    if (r - l <= 1) return 0;
    int m = (l + r) / 2;
    long long inv = inversions(a, l, m) + inversions(a, m, r);
    vector<int> tmp;
    int i = l, j = m;
    while (i < m and j < r) {
        if (a[i] <= a[j])
            tmp.push_back(a[i++]);
        else {
            inv += m - i;
            tmp.push_back(a[j++]);
        }
    }
    while (i < m) tmp.push_back(a[i++]);
    while (j < r) tmp.push_back(a[j++]);
    copy(tmp.begin(), tmp.end(), a.begin() + l);
    return inv;
}

/**
 * Example: count inversions in {3, 1, 2}.
 */
int main() {
    vector<int> a = {3, 1, 2};
    cout << inversions(a, 0, a.size()) << "\n";  // -> 2
    return 0;
}
