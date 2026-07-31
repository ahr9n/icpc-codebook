/**
 * Polynomial string hashing (single mod). Prefix hashes + powers, then substring
 * hash of s[l..r] in O(1). Compare substrings by equal hash. O(n) build.
 */
struct StringHash {
    long long mod = 1e9 + 9, base = 131;
    vector<long long> pre_hash, base_pow;

    StringHash(string& s) {
        int len = s.size();
        pre_hash.assign(len + 1, 0);
        base_pow.assign(len + 1, 1);
        for (int i = 0; i < len; i++) {
            pre_hash[i + 1] = (pre_hash[i] * base + s[i]) % mod;
            base_pow[i + 1] = base_pow[i] * base % mod;
        }
    }

    long long get(int l, int r) {
        return ((pre_hash[r + 1] - pre_hash[l] * base_pow[r - l + 1]) % mod + mod) % mod;
    }
};

/**
 * Example: two equal substrings share the same hash.
 */
int main() {
    string s = "abcabc";
    StringHash sh(s);
    cout << (sh.get(0, 2) == sh.get(3, 5)) << "\n";  // -> 1
    return 0;
}
