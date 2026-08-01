int n;
int groups;
vector<long long> prefix;
vector<vector<long long>> dp;

/**
 * Divide-and-conquer DP optimization for dp[g][i] = min over j<i of
 * dp[g-1][j] + cost(j, i): partition the first i elements into g contiguous
 * groups, cost(j, i) being the price of the group covering a[j..i-1].
 *
 * The plain DP tries every split j for every (g, i): O(groups * n^2). This
 * speeds each level to O(n log n) because the best split opt(g, i) is
 * non-decreasing in i, so once compute() pins opt for the middle index it
 * bounds the search for the two halves. That monotonicity is guaranteed when
 * cost satisfies the quadrangle inequality
 *     cost(a, c) + cost(b, d) <= cost(a, d) + cost(b, c)  for a <= b <= c <= d.
 * Here cost is the squared segment sum over non-negative values, which obeys it.
 * Total O(groups * n log n). Contract: cost must satisfy the inequality above,
 * otherwise the pruned search can miss the true optimum.
 */
long long cost(int j, int i) {
    long long segment_sum = prefix[i] - prefix[j];
    return segment_sum * segment_sum;
}

void compute(int level, int l, int r, int opt_low, int opt_high) {
    if (l > r) return;

    int mid = (l + r) / 2;
    long long best = LINF;
    int best_split = -1;
    for (int j = opt_low; j <= min(mid - 1, opt_high); j++) {
        if (dp[level - 1][j] >= LINF) continue;

        long long candidate = dp[level - 1][j] + cost(j, mid);
        if (candidate < best) {
            best = candidate;
            best_split = j;
        }
    }
    dp[level][mid] = best;

    if (best_split == -1) {
        compute(level, l, mid - 1, opt_low, opt_high);
        compute(level, mid + 1, r, opt_low, opt_high);
        return;
    }
    compute(level, l, mid - 1, opt_low, best_split);
    compute(level, mid + 1, r, best_split, opt_high);
}

long long min_partition_cost() {
    dp.assign(groups + 1, vector<long long>(n + 1, LINF));
    for (int i = 1; i <= n; i++) dp[1][i] = cost(0, i);

    for (int level = 2; level <= groups; level++) compute(level, 1, n, 0, n);
    return dp[groups][n];
}

/**
 * Example: split {1, 3, 2, 4, 5, 1} into 3 contiguous groups so the sum of
 * squared group sums is smallest. Best split is {1,3} {2,4} {5,1}: 16+36+36.
 */
int main() {
    vector<int> a = {1, 3, 2, 4, 5, 1};
    n = a.size();
    groups = 3;

    prefix.assign(n + 1, 0);
    for (int i = 0; i < n; i++) prefix[i + 1] = prefix[i] + a[i];

    cout << min_partition_cost() << "\n";  // -> 88
    return 0;
}
