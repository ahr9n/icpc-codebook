/**
 * Two pointers / sliding window: longest subarray with sum <= k (non-negative a).
 * Expand right, shrink left while the window breaks the constraint. O(n)
 */
int longest_window(vector<long long>& a, long long k) {
    int best = 0;
    long long sum = 0;
    for (int l = 0, r = 0; r < (int)a.size(); r++) {
        sum += a[r];
        while (sum > k) sum -= a[l++];
        best = max(best, r - l + 1);
    }
    return best;
}
