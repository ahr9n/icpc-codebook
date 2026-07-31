/**
 * Meet in the middle: count subsets of a whose sum equals target. Split in half,
 * enumerate all 2^(n/2) subset sums of each, match with a sorted lookup. O(2^(n/2) * n)
 */
long long meet_in_middle(vector<long long>& a, long long target) {
    int half = a.size() / 2, rest = a.size() - half;
    vector<long long> left, right;
    for (int mask = 0; mask < (1 << half); mask++) {
        long long sum = 0;
        for (int i = 0; i < half; i++)
            if (mask & (1 << i)) sum += a[i];
        left.push_back(sum);
    }
    for (int mask = 0; mask < (1 << rest); mask++) {
        long long sum = 0;
        for (int i = 0; i < rest; i++)
            if (mask & (1 << i)) sum += a[half + i];
        right.push_back(sum);
    }
    sort(right.begin(), right.end());

    long long count = 0;
    for (long long s: left) {
        long long need = target - s;
        auto lo = lower_bound(right.begin(), right.end(), need);
        auto hi = upper_bound(right.begin(), right.end(), need);
        count += hi - lo;
    }
    return count;
}
