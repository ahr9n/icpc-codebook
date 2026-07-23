vector<int> a;
vector<int> memo;

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

// top-down. O(n²)
int lis_rec(int i) {
    int& ret = memo[i];
    if (~ret) return ret;

    ret = 1;
    for (int j = 0; j < i; j++)
        if (a[j] < a[i]) ret = max(ret, 1 + lis_rec(j));

    return ret;
}

/**
 * Example: LIS of {10,9,2,5,3,7,101,18} has length 4 (e.g. 2,3,7,18).
 */
int main() {
    a = {10, 9, 2, 5, 3, 7, 101, 18};
    cout << lis() << "\n";  // -> 4

    int n = a.size();
    memo.assign(n, -1);
    int ans = 0;
    for (int i = 0; i < n; i++) ans = max(ans, lis_rec(i));
    cout << ans << "\n";  // -> 4
    return 0;
}