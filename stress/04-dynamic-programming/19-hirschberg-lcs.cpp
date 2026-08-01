// Stress: hirschberg + lcs_length vs a full O(n*m) DP; also verify the returned
// string is a common subsequence of both inputs and has optimal length.
long long lcs_dp(const string& a, const string& b) {
    int n = a.size(), m = b.size();
    vector<vector<long long>> dp(n + 1, vector<long long>(m + 1, 0));
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++) {
            if (a[i - 1] == b[j - 1])
                dp[i][j] = dp[i - 1][j - 1] + 1;
            else
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
        }
    return dp[n][m];
}

bool is_subsequence(const string& s, const string& text) {
    int k = 0;
    for (char c: text)
        if (k < (int)s.size() and s[k] == c) k++;
    return k == (int)s.size();
}

int stress() {
    mt19937 rng(19);
    long long checks = 0;

    string long_repeated(100000, 'x');
    if (hirschberg(long_repeated, "x") != "x" or not hirschberg(long_repeated, "").empty()) {
        printf("FAIL long-skinny\n");
        return 1;
    }
    checks += 2;

    for (int trial = 0; trial < 6000; trial++) {
        int alphabet = 1 + rng() % 4;  // small alphabet -> longer shared subsequences
        int na = rng() % 9;
        int nb = rng() % 9;
        string a(na, 'a'), b(nb, 'a');
        for (auto& c: a) c = 'a' + rng() % alphabet;
        for (auto& c: b) c = 'a' + rng() % alphabet;

        long long ref = lcs_dp(a, b);
        if (lcs_length(a, b) != ref) {
            printf("FAIL length a=%s b=%s\n", a.c_str(), b.c_str());
            return 1;
        }

        string got = hirschberg(a, b);
        if ((long long)got.size() != ref or not is_subsequence(got, a) or
            not is_subsequence(got, b)) {
            printf("FAIL string a=%s b=%s got=%s ref=%lld\n", a.c_str(), b.c_str(), got.c_str(),
                   ref);
            return 1;
        }
        checks++;
    }

    printf("hirschberg-lcs PASS %lld", checks);
    return 0;
}
