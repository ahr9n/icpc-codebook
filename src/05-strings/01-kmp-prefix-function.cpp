/**
 * KMP prefix function: pi[i] = longest proper prefix of s[0..i] that is also a suffix.
 * Match pattern in text via prefix_function(pat + '#' + text). O(n)
 */
vector<int> prefix_function(string& s) {
    int len = s.size();
    vector<int> pi(len, 0);
    for (int i = 1; i < len; i++) {
        int j = pi[i - 1];
        while (j > 0 and s[i] != s[j]) j = pi[j - 1];
        if (s[i] == s[j]) j++;
        pi[i] = j;
    }
    return pi;
}

/**
 * Example: count occurrences of pat in text via KMP on pat + '#' + text (pi == |pat|).
 */
int main() {
    string pat = "ab", text = "ababab";
    string combined = pat + '#' + text;
    vector<int> pi = prefix_function(combined);

    int occ = 0;
    for (int val: pi)
        if (val == (int)pat.size()) occ++;
    cout << occ << '\n';

    StringHash sh(text);
    cout << (sh.get(0, 1) == sh.get(2, 3)) << '\n';

    return 0;
}
