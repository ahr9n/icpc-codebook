vector<int> a;

/**
 * Longest strictly increasing subsequence of a. Patience sorting on tails.
 * For non-decreasing, swap lower_bound -> upper_bound. O(n log n)
 */
int lis() {
    vector<int> tails;
    for (int x: a) {
        auto it = lower_bound(tails.begin(), tails.end(), x);
        if (it == tails.end())
            tails.push_back(x);
        else
            *it = x;
    }
    return tails.size();
}
