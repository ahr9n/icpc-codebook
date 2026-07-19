/**
 * Polynomial string hashing (single mod). Prefix hashes + powers, then substring
 * hash of s[l..r] in O(1). Compare substrings by equal hash. O(n) build.
 */
struct StringHash {
    long long mod = 1e9 + 9, base = 131;
    vector<long long> h, p;

    StringHash(string& s) {
        int len = s.size();
        h.assign(len + 1, 0);
        p.assign(len + 1, 1);
        for (int i = 0; i < len; i++) {
            h[i + 1] = (h[i] * base + s[i]) % mod;
            p[i + 1] = p[i] * base % mod;
        }
    }

    long long get(int l, int r) {
        return ((h[r + 1] - h[l] * p[r - l + 1]) % mod + mod) % mod;
    }
};
