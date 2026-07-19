const long long MOD = 1e9 + 7;
int D;
string num;
vector<vector<long long>> dp;

/**
 * Digit DP (EDPC Digit Sum): count x in [0, num] with digit-sum % D == 0.
 * State (pos, rem, tight); cache the non-tight branch only. Subtract 1 to drop x = 0.
 * Caller: dp.assign(len, vector<long long>(D, -1)). O(len * D * 10)
 */
long long digit_dp(int pos, int rem, bool tight) {
    if (pos == (int)num.size()) return rem == 0;
    if (not tight and dp[pos][rem] != -1) return dp[pos][rem];

    int hi = tight ? num[pos] - '0' : 9;
    long long res = 0;
    for (int d = 0; d <= hi; d++)
        res = (res + digit_dp(pos + 1, (rem + d) % D, tight and d == hi)) % MOD;

    if (not tight) dp[pos][rem] = res;
    return res;
}

/**
 * Example: digit DP counts x in [0, 100] whose digit sum divides by 3 (set num, dp, D).
 */
int main() {
    num = "100";
    D = 3;
    dp.assign(num.size(), vector<long long>(D, -1));
    cout << digit_dp(0, 0, true) << '\n';
    return 0;
}
