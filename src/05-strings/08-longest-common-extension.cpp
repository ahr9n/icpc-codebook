/**
 * Longest common extension: lce(i, j) = length of the longest common prefix of the suffixes
 * starting at i and j (how far the two positions match before diverging). Binary search on
 * the shared-substring hash -> O(log n) per query after an O(n) build; needs StringHash (03)
 * in scope. This is the reusable core of palindrome / center problems: jump straight to the
 * next mismatch instead of stepping one char at a time. (Suffix array + LCP + sparse table
 * answers the same query in O(1) if you already have those built.)
 */
int lce(StringHash& hash, int len, int i, int j) {
    int lo = 0, hi = len - max(i, j);
    while (lo < hi) {
        int mid = (lo + hi + 1) / 2;
        if (hash.get(i, i + mid - 1) == hash.get(j, j + mid - 1))
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/**
 * Example: how far pairs of suffixes of "ababa" agree.
 */
int main() {
    string s = "ababa";
    StringHash hash(s);
    int len = s.size();
    cout << lce(hash, len, 0, 2) << "\n";  // -> 3  (ababa / aba agree on "aba")
    cout << lce(hash, len, 1, 3) << "\n";  // -> 2  (baba / ba agree on "ba")
    cout << lce(hash, len, 0, 1) << "\n";  // -> 0  (a / b diverge immediately)
    return 0;
}
