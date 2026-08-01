/**
 * Longest common subsequence in linear memory.
 *
 * Two pieces:
 *   lcs_length  -- LCS *length only*, keeping just two DP rows: O(n*m) time,
 *                  O(min(n,m)) memory.
 *   hirschberg  -- the actual LCS *string* by divide-and-conquer: still
 *                  O(n*m) time but O(n+m) memory, versus O(n*m) for a full
 *                  DP table you would backtrack through.
 *
 * Why Hirschberg saves memory: the classic reconstruction stores the whole
 * n*m table so it can walk back from the corner. Here we never keep a table.
 * We split the first string at its middle row, use two linear-space score
 * passes (forward over prefixes, backward over suffixes) to learn *where* an
 * optimal LCS crosses that row in the second string, then recurse on the two
 * halves. Each level allocates only O(m) rows; the split point is all we carry
 * across the recursion, so peak memory stays O(n+m).
 */

// Last row of the LCS DP of a against b: row[j] = LCS(a, b[0..j)). O(|b|) memory.
vector<long long> lcs_row(const string& a, const string& b) {
    int m = b.size();
    vector<long long> prev(m + 1, 0), cur(m + 1, 0);
    for (int i = 1; i <= (int)a.size(); i++) {
        for (int j = 1; j <= m; j++)
            if (a[i - 1] == b[j - 1])
                cur[j] = prev[j - 1] + 1;
            else
                cur[j] = max(prev[j], cur[j - 1]);
        swap(prev, cur);
    }
    return prev;
}

// Warmup: LCS length in O(min(n,m)) memory by rolling the row along the shorter string.
long long lcs_length(const string& a, const string& b) {
    if (a.size() < b.size()) return lcs_length(b, a);
    return lcs_row(a, b).back();
}

string hirschberg(const string& a, const string& b) {
    int n = a.size(), m = b.size();
    if (n == 0) return "";
    if (n == 1) {
        // One character contributes to the LCS iff it appears somewhere in b.
        for (char c: b)
            if (c == a[0]) return string(1, a[0]);
        return "";
    }

    int mid = n / 2;
    string a_left = a.substr(0, mid);
    string a_right = a.substr(mid);

    // score_l[k]      = LCS(a_left,  b[0..k)).
    // score_r_rev[j]  = LCS(a_right, b[m-j..m)) -- suffix scores via reversed strings,
    //                   so the backward pass reuses the same forward row routine.
    vector<long long> score_l = lcs_row(a_left, b);
    string a_right_rev(a_right.rbegin(), a_right.rend());
    string b_rev(b.rbegin(), b.rend());
    vector<long long> score_r_rev = lcs_row(a_right_rev, b_rev);

    // Split b at the k that lets the two halves of a claim the most together.
    int best_k = 0;
    long long best = -1;
    for (int k = 0; k <= m; k++) {
        long long total = score_l[k] + score_r_rev[m - k];
        if (total > best) {
            best = total;
            best_k = k;
        }
    }

    return hirschberg(a_left, b.substr(0, best_k)) + hirschberg(a_right, b.substr(best_k));
}

/**
 * Example: LCS of "AGGTAB" and "GXTXAYB" is "GTAB", length 4.
 */
int main() {
    string a = "AGGTAB", b = "GXTXAYB";
    cout << lcs_length(a, b) << "\n";  // -> 4
    cout << hirschberg(a, b) << "\n";  // -> GTAB
    return 0;
}
