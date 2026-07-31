/**
 * Manacher: longest palindromic substring in O(n). Transform to ^#a#b#...#$ so odd and
 * even palindromes are uniform; radius[i] = palindrome radius at transformed position i.
 */
string longest_palindrome(string& s) {
    string t = "^";
    for (char c: s) t += '#', t += c;
    t += "#$";
    int n = t.size();
    vector<int> radius(n, 0);
    int center = 0, right = 0;
    for (int i = 1; i < n - 1; i++) {
        if (i < right) radius[i] = min(right - i, radius[2 * center - i]);
        while (t[i + radius[i] + 1] == t[i - radius[i] - 1]) radius[i]++;
        if (i + radius[i] > right) center = i, right = i + radius[i];
    }
    int best = 0, best_center = 0;
    for (int i = 1; i < n - 1; i++)
        if (radius[i] > best) best = radius[i], best_center = i;
    return s.substr((best_center - best) / 2, best);
}

/**
 * Example: longest palindromic substring of "babad".
 */
int main() {
    string s = "babad";
    cout << longest_palindrome(s) << "\n";  // -> bab
    return 0;
}
