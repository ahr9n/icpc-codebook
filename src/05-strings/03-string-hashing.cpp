/**
 * Polynomial string hashing (Mersenne mod 2^61-1, random base). Prefix hashes + powers give
 * the hash of s[l..r] in O(1) as one 61-bit scalar you compare with ==. A single 2^61-1 mod
 * is as collision-safe as a double 1e9 mod (~4e-19 per pair) with half the code; the random
 * base makes it anti-hash proof, since a fixed base+mod gets hacked on Codeforces. char + 1
 * keeps a 0 byte from colliding a shorter prefix. mul uses __int128 to dodge overflow. O(n)
 * build. Compare substrings only within one StringHash -- two objects draw different bases.
 */
mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count() ^ random_device{}());

struct StringHash {
    static const unsigned long long mod = (1ULL << 61) - 1;
    unsigned long long base;
    vector<unsigned long long> pre, pow;

    unsigned long long mul(unsigned long long a, unsigned long long b) {
        return (unsigned __int128)a * b % mod;
    }

    StringHash(const string& s) {
        base = rng() % (mod - 256) + 256;  // above the alphabet, below the mod
        int n = s.size();
        pre.assign(n + 1, 0);
        pow.assign(n + 1, 1);
        for (int i = 0; i < n; i++) {
            pre[i + 1] = (mul(pre[i], base) + s[i] + 1) % mod;
            pow[i + 1] = mul(pow[i], base);
        }
    }

    unsigned long long get(int l, int r) {
        return (pre[r + 1] + mod - mul(pre[l], pow[r - l + 1])) % mod;
    }
};

/**
 * Example: equal substrings share a hash, different ones do not.
 */
int main() {
    string s = "abcabc";
    StringHash hash(s);
    cout << (hash.get(0, 2) == hash.get(3, 5)) << "\n";  // -> 1
    cout << (hash.get(0, 2) == hash.get(1, 3)) << "\n";  // -> 0
    return 0;
}
