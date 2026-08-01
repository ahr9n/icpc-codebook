// Stress: eertree distinct-palindrome count and per-position ending counts vs a
// naive set of all palindromic substrings, over many random small-alphabet strings.
bool is_palindrome(const string& s, int l, int r) {
    while (l < r) {
        if (s[l] != s[r]) return false;
        l++;
        r--;
    }
    return true;
}

int stress() {
    mt19937 rng(11);
    long long checks = 0;
    auto run_case = [&](const string& s) {
        Eertree tree;
        set<string> distinct;
        int n = s.size();
        for (int i = 0; i < n; i++) {
            tree.add(s[i]);
            int naive_ending = 0;
            for (int l = 0; l <= i; l++)
                if (is_palindrome(s, l, i)) {
                    distinct.insert(s.substr(l, i - l + 1));
                    naive_ending++;
                }
            if (tree.palindromes_ending_here() != naive_ending) {
                printf("FAIL ending s=%s i=%d got=%d want=%d\n", s.c_str(), i,
                       tree.palindromes_ending_here(), naive_ending);
                return 1;
            }
        }
        if (tree.count_distinct() != (long long)distinct.size()) {
            printf("FAIL distinct s=%s got=%lld want=%zu\n", s.c_str(),
                   tree.count_distinct(), distinct.size());
            return 1;
        }
        checks++;
        return 0;
    };

    for (const string& edge: {string("a"), string(20, 'z'), string("abcdefghij")})
        if (run_case(edge)) return 1;

    for (int rep = 0; rep < 4000; rep++) {
        int len = 1 + rng() % 40;
        int alpha = 1 + rng() % 3;
        string s;
        for (int i = 0; i < len; i++) s.push_back('a' + rng() % alpha);
        if (run_case(s)) return 1;
    }
    printf("palindromic-tree PASS %lld", checks);
    return 0;
}
