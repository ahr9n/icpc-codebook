int n, W;
const long long LINF = 0x3f3f3f3f3f3f3f3fLL;
vector<int> wt, val;
vector<long long> dp;

/**
 * 0/1 Knapsack by value (use when W is huge but sum(val) small).
 * dp[v] = min weight reaching value v; answer = max v with dp[v] <= W. O(n*sumV)
 */
long long knapsack_by_value() {
    int sum_v = accumulate(val.begin(), val.end(), 0);
    dp.assign(sum_v + 1, LINF);
    dp[0] = 0;
    for (int i = 0; i < n; i++)
        for (int v = sum_v; v >= val[i]; v--) dp[v] = min(dp[v], dp[v - val[i]] + wt[i]);
    for (int v = sum_v; v >= 0; v--)
        if (dp[v] <= W) return v;
    return 0;
}
