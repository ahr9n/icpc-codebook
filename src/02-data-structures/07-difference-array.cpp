/**
 * Difference array: O(1) range-add, then one prefix pass to recover final values.
 * diff[l] += v, diff[r+1] -= v; prefix-sum diff to get a[]. O(n) total.
 */
vector<long long> difference_array(int len, vector<tuple<int, int, long long>>& updates) {
    vector<long long> diff(len + 1, 0);
    for (auto [l, r, v]: updates) {
        diff[l] += v;
        diff[r + 1] -= v;
    }
    for (int i = 1; i < len; i++) diff[i] += diff[i - 1];
    diff.pop_back();
    return diff;
}
