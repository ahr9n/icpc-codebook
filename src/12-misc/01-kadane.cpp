/**
 * Kadane: max sum of a contiguous subarray. Track best-ending-here vs restart. O(n)
 */
long long max_subarray(const vector<long long>& a) {
    long long best = a[0], cur = a[0];
    for (int i = 1; i < (int)a.size(); i++) {
        cur = max(a[i], cur + a[i]);
        best = max(best, cur);
    }
    return best;
}
