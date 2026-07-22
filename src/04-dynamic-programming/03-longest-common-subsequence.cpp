string s, t;
vector<vector<long long>> dp;
vector<vector<long long>> mem;

/**
 * Longest common subsequence of s, t. dp[i][j] = LCS of the two prefixes. O(|s|*|t|)
 */
int lcs() {
    int p = s.size(), q = t.size();
    dp.assign(p + 1, vector<long long>(q + 1, 0));
    for (int i = 1; i <= p; i++)
        for (int j = 1; j <= q; j++)
            if (s[i - 1] == t[j - 1])
                dp[i][j] = dp[i - 1][j - 1] + 1;
            else
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
    return dp[p][q];
}

// top-down: mem[i][j] = LCS of the suffixes s[i..], t[j..].
// Caller: mem.assign(s.size() + 1, vector<long long>(t.size() + 1, -1)), lcs_rec(0, 0).
long long lcs_rec(int i, int j) {
    if (i == (int)s.size() or j == (int)t.size()) return 0;

    long long& ret = mem[i][j];
    if (~ret) return ret;

    if (s[i] == t[j]) return ret = 1 + lcs_rec(i + 1, j + 1);
    return ret = max(lcs_rec(i + 1, j), lcs_rec(i, j + 1));
}
