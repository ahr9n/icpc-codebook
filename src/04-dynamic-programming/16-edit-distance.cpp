string s, t;
vector<vector<long long>> dp;
vector<vector<long long>> mem;

/**
 * Edit distance: min insert/delete/replace to turn s into t via a DP table. O(|s|*|t|)
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

// top-down: mem[i][j] = edit distance between suffixes s[i..], t[j..].
// Caller: mem.assign(s.size() + 1, vector<long long>(t.size() + 1, -1)), edit_distance_rec(0, 0).
long long edit_distance_rec(int i, int j) {
    if (i == (int)s.size()) return t.size() - j;
    if (j == (int)t.size()) return s.size() - i;

    long long& ret = mem[i][j];
    if (~ret) return ret;

    if (s[i] == t[j]) return ret = edit_distance_rec(i + 1, j + 1);
    return ret = 1 + min({edit_distance_rec(i + 1, j), edit_distance_rec(i, j + 1),
                          edit_distance_rec(i + 1, j + 1)});
}

/**
 * Example: edit distance from "kitten" to "sitting" is 3.
 */
int main() {
    s = "kitten";
    t = "sitting";
    cout << edit_distance() << "\n";  // -> 3

    mem.assign(s.size() + 1, vector<long long>(t.size() + 1, -1));
    cout << edit_distance_rec(0, 0) << "\n";  // -> 3
    return 0;
}
