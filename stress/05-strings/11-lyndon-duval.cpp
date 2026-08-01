// Stress: Duval factorization is a valid non-increasing Lyndon split that
// concatenates back; min_rotation matches brute force over all n rotations.
static bool is_lyndon(const string& w) {
    int len = w.size();
    for (int suffix = 1; suffix < len; suffix++)
        if (w.substr(suffix) <= w)
            return false;
    return true;
}

int stress() {
    mt19937 rng(11);
    const string alphabet = "ab";
    long long checks = 0;
    for (int rep = 0; rep < 20000; rep++) {
        int len = rng() % 9 + 1;
        string s;
        for (int i = 0; i < len; i++)
            s += alphabet[rng() % alphabet.size()];

        vector<string> factors = duval(s);
        string rebuilt;
        for (const string& factor: factors)
            rebuilt += factor;
        if (rebuilt != s) {
            printf("FAIL concat s=%s\n", s.c_str());
            return 1;
        }
        for (const string& factor: factors)
            if (not is_lyndon(factor)) {
                printf("FAIL not-lyndon s=%s factor=%s\n", s.c_str(), factor.c_str());
                return 1;
            }
        for (int i = 1; i < (int)factors.size(); i++)
            if (factors[i - 1] < factors[i]) {
                printf("FAIL increasing s=%s\n", s.c_str());
                return 1;
            }

        int best = 0;
        for (int start = 1; start < len; start++)
            if (s.substr(start) + s.substr(0, start) < s.substr(best) + s.substr(0, best))
                best = start;
        int got = min_rotation(s);
        if (s.substr(got) + s.substr(0, got) != s.substr(best) + s.substr(0, best)) {
            printf("FAIL rotation s=%s got=%d best=%d\n", s.c_str(), got, best);
            return 1;
        }
        checks++;
    }
    printf("lyndon PASS %lld", checks);
    return 0;
}
