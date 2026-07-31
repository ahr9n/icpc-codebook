/**
 * Number-Theoretic Transform: the FFT above done in the prime field mod
 * 998244353, whose 2^23 | (mod - 1) supplies exact roots of unity from the
 * primitive root 3. Same butterfly, but every operation is modular, so integer
 * convolutions come out exact -- no floating-point rounding, at the cost of
 * working modulo a fixed prime. Use it when coefficients or their sums exceed
 * FFT's safe precision. O(n log n), in-place, iterative.
 */
const long long NTT_MOD = 998244353;
const long long NTT_ROOT = 3;

long long ntt_pow(long long base, long long exp) {
    long long res = 1;
    base %= NTT_MOD;
    while (exp) {
        if (exp & 1) res = res * base % NTT_MOD;
        base = base * base % NTT_MOD;
        exp >>= 1;
    }
    return res;
}

void ntt(vector<long long>& a, bool inverse) {
    int n = a.size();
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }
    for (int len = 2; len <= n; len <<= 1) {
        long long step = ntt_pow(NTT_ROOT, (NTT_MOD - 1) / len);
        if (inverse) step = ntt_pow(step, NTT_MOD - 2);
        for (int i = 0; i < n; i += len) {
            long long w = 1;
            for (int k = 0; k < len / 2; k++) {
                long long u = a[i + k], v = a[i + k + len / 2] * w % NTT_MOD;
                a[i + k] = (u + v) % NTT_MOD;
                a[i + k + len / 2] = (u - v + NTT_MOD) % NTT_MOD;
                w = w * step % NTT_MOD;
            }
        }
    }
    if (inverse) {
        long long inv_n = ntt_pow(n, NTT_MOD - 2);
        for (long long& x: a) x = x * inv_n % NTT_MOD;
    }
}

vector<long long> multiply(vector<long long>& p, vector<long long>& q) {
    vector<long long> fp(p.begin(), p.end()), fq(q.begin(), q.end());
    int need = p.size() + q.size() - 1, n = 1;
    while (n < need) n <<= 1;
    fp.resize(n);
    fq.resize(n);
    ntt(fp, false);
    ntt(fq, false);
    for (int i = 0; i < n; i++) fp[i] = fp[i] * fq[i] % NTT_MOD;
    ntt(fp, true);
    fp.resize(need);
    return fp;
}

/**
 * Example: (1 + 2x + 3x^2) * (4 + 5x) = 4 + 13x + 22x^2 + 15x^3 (mod 998244353).
 */
int main() {
    vector<long long> p = {1, 2, 3}, q = {4, 5};
    for (long long c: multiply(p, q)) cout << c << " ";
    cout << "\n";  // -> 4 13 22 15
    return 0;
}
