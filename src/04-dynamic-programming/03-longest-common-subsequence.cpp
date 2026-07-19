string s, t;
vector<vector<long long>> dp;

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
