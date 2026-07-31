/**
 * Suffix array (prefix doubling) + Kasai LCP.
 * sa[i] = start of the i-th smallest suffix; sort by (rank, next rank), doubling the
 * compared length each round. O(n log^2 n). lcp[i] = LCP(sa[i-1], sa[i]), lcp[0] = 0.
 * Simplest form; for the O(n log n) radix-sort build see 07-suffix-array-radix.
 */
vector<int> suffix_array(string& s) {
    int n = s.size();
    vector<int> sa(n), rank(n), tmp(n);
    for (int i = 0; i < n; i++) sa[i] = i, rank[i] = s[i];
    for (int k = 1; k < n; k <<= 1) {
        auto key = [&](int i) { return make_pair(rank[i], i + k < n ? rank[i + k] : -1); };
        sort(sa.begin(), sa.end(), [&](int a, int b) { return key(a) < key(b); });
        tmp[sa[0]] = 0;
        for (int i = 1; i < n; i++) tmp[sa[i]] = tmp[sa[i - 1]] + (key(sa[i - 1]) < key(sa[i]));
        rank = tmp;
        if (rank[sa[n - 1]] == n - 1) break;
    }
    return sa;
}

vector<int> lcp_array(string& s, vector<int>& sa) {
    int n = s.size();
    vector<int> rank(n), lcp(n, 0);
    for (int i = 0; i < n; i++) rank[sa[i]] = i;
    int k = 0;
    for (int i = 0; i < n; i++) {
        if (rank[i] == 0) {
            k = 0;
            continue;
        }
        int j = sa[rank[i] - 1];
        while (i + k < n and j + k < n and s[i + k] == s[j + k]) k++;
        lcp[rank[i]] = k;
        if (k) k--;
    }
    return lcp;
}

/**
 * Example: suffix array and LCP of "banana".
 */
int main() {
    string s = "banana";
    vector<int> sa = suffix_array(s);
    vector<int> lcp = lcp_array(s, sa);
    for (int x: sa) cout << x << " ";  // -> 5 3 1 0 4 2
    cout << "\n";
    for (int x: lcp) cout << x << " ";  // -> 0 1 3 0 0 2
    cout << "\n";
    return 0;
}
