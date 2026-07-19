vector<int> coins;

/**
 * Edit distance with reconstruction: min insert/delete/replace to turn s into t.
 * dp table, then walk back to emit the operations. O(|s|*|t|)
 */
int edit_distance() {
    int p = s.size(), q = t.size();
    dp.assign(p + 1, vector<long long>(q + 1, 0));
    for (int i = 0; i <= p; i++) dp[i][0] = i;
    for (int j = 0; j <= q; j++) dp[0][j] = j;

    for (int i = 1; i <= p; i++)
        for (int j = 1; j <= q; j++)
            if (s[i - 1] == t[j - 1])
                dp[i][j] = dp[i - 1][j - 1];
            else
                dp[i][j] = 1 + min({dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]});
    return dp[p][q];
}
