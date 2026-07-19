int n, W;
const long long LINF = 0x3f3f3f3f3f3f3f3fLL;
vector<int> wt, val;
vector<long long> dp;
vector<vector<long long>> mem;

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

// top-down: mem[i][v] = min weight for value v from items i.. (LINF if impossible).
// Caller: mem.assign(n, vector<long long>(sum_v + 1, -1)); answer = max v with rec(0,v) <= W.
long long knapsack_by_value_rec(int i, int v) {
    if (v <= 0) return 0;
    if (i == n) return LINF;

    long long& ret = mem[i][v];
    if (~ret) return ret;

    ret = knapsack_by_value_rec(i + 1, v);
    ret = min(ret, knapsack_by_value_rec(i + 1, max(0, v - val[i])) + wt[i]);
    return ret;
}
