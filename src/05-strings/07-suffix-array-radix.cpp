/**
 * Suffix array in O(n log n): same prefix doubling as 06, but each round sorts by the
 * (rank, next rank) pair with two counting-sort passes (radix) instead of std::sort,
 * dropping a log factor. Faster for n around 10^6; Kasai LCP below is unchanged. O(n log n).
 */
vector<int> suffix_array(string& s) {
    int n = s.size();
    vector<int> sa(n), rank(n), tmp(n);
    int sz = max(256, n + 1);
    vector<int> cnt(sz + 1, 0);
    for (int i = 0; i < n; i++) sa[i] = i, rank[i] = (unsigned char)s[i];
    for (int k = 1; k < n; k <<= 1) {
        auto second = [&](int i) { return i + k < n ? rank[i + k] + 1 : 0; };
        auto first = [&](int i) { return rank[i]; };
        fill(cnt.begin(), cnt.end(), 0);
        for (int i = 0; i < n; i++) cnt[second(i)]++;
        for (int i = 1; i < (int)cnt.size(); i++) cnt[i] += cnt[i - 1];
        for (int i = n - 1; i >= 0; i--) tmp[--cnt[second(i)]] = i;
        fill(cnt.begin(), cnt.end(), 0);
        for (int i = 0; i < n; i++) cnt[first(i)]++;
        for (int i = 1; i < (int)cnt.size(); i++) cnt[i] += cnt[i - 1];
        for (int i = n - 1; i >= 0; i--) sa[--cnt[first(tmp[i])]] = tmp[i];
        tmp[sa[0]] = 0;
        for (int i = 1; i < n; i++) {
            bool same = rank[sa[i]] == rank[sa[i - 1]] and second(sa[i]) == second(sa[i - 1]);
            tmp[sa[i]] = tmp[sa[i - 1]] + (same ? 0 : 1);
        }
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
 * Example: suffix array and LCP of "banana" (same output as 06, built in O(n log n)).
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
