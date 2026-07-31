/**
 * Manacher: all palindromic radii in O(n). Transform to ^#a#b#...#$ so odd and even
 * palindromes are uniform; radius[i] = palindrome radius at transformed position i, which is
 * exactly the palindrome length in the original string. The radius array -- not just the
 * single longest -- is the reusable primitive: it gives every center's largest 0-mismatch
 * palindrome, the base case for tricks like "palindrome after <= 1 edit" (extend each center
 * past its radius with one LCE query to reach the 2nd mismatch).
 */
vector<int> manacher(const string& s) {
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
    return radius;
}

string longest_palindrome(const string& s) {
    vector<int> radius = manacher(s);
    int best = 0, best_center = 0;
    for (int i = 1; i + 1 < (int)radius.size(); i++)
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
