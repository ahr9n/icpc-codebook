/**
 * XOR (linear) basis over GF(2): a set of numbers viewed as vectors of bits,
 * kept in reduced form so each basis element owns a distinct highest bit.
 * insert() folds a value in, discarding it if already spanned; max_xor()
 * greedily takes each basis element that increases the running value, giving the
 * largest XOR of any subset. The span has 2^rank distinct XOR values. Insert and
 * query are O(BITS); a fresh instance starts empty.
 */
struct XorBasis {
    static const int BITS = 60;
    long long basis[BITS] = {};

    void insert(long long x) {
        for (int b = BITS - 1; b >= 0; b--) {
            if (not((x >> b) & 1)) continue;
            if (not basis[b]) {
                basis[b] = x;
                return;
            }
            x ^= basis[b];
        }
    }

    long long max_xor() {
        long long res = 0;
        for (int b = BITS - 1; b >= 0; b--) res = max(res, res ^ basis[b]);
        return res;
    }
};

/**
 * Example: {1, 2, 3, 8} -- the largest achievable XOR is 8 ^ 3 = 11.
 */
int main() {
    XorBasis xb;
    for (long long v: {1LL, 2LL, 3LL, 8LL}) xb.insert(v);
    cout << xb.max_xor() << "\n";  // -> 11
    return 0;
}
