/**
 * Fast Fourier Transform over complex doubles for polynomial multiplication.
 * Evaluate both polynomials at the 2^k-th roots of unity, multiply point-wise,
 * then interpolate back with the inverse transform -- turning an O(n^2)
 * convolution into O(n log n). Results are floating point, so round to the
 * nearest integer for integer coefficients; magnitudes past ~10^15 lose
 * precision (use NTT below for exact modular convolution). In-place, iterative.
 */
using Complex = complex<long double>;

void fft(vector<Complex>& a, bool inverse) {
    int n = a.size();
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }
    for (int len = 2; len <= n; len <<= 1) {
        long double angle = 2 * acosl(-1.0L) / len * (inverse ? -1 : 1);
        Complex step(cosl(angle), sinl(angle));
        for (int i = 0; i < n; i += len) {
            Complex w(1, 0);
            for (int k = 0; k < len / 2; k++) {
                Complex u = a[i + k], v = a[i + k + len / 2] * w;
                a[i + k] = u + v;
                a[i + k + len / 2] = u - v;
                w *= step;
            }
        }
    }
    if (inverse)
        for (Complex& x: a) x /= n;
}

vector<long long> multiply(vector<long long>& p, vector<long long>& q) {
    vector<Complex> fp(p.begin(), p.end()), fq(q.begin(), q.end());
    int need = p.size() + q.size() - 1, n = 1;
    while (n < need) n <<= 1;
    fp.resize(n);
    fq.resize(n);
    fft(fp, false);
    fft(fq, false);
    for (int i = 0; i < n; i++) fp[i] *= fq[i];
    fft(fp, true);
    vector<long long> result(need);
    for (int i = 0; i < need; i++) result[i] = llroundl(fp[i].real());
    return result;
}

/**
 * Example: (1 + 2x + 3x^2) * (4 + 5x) = 4 + 13x + 22x^2 + 15x^3.
 */
int main() {
    vector<long long> p = {1, 2, 3}, q = {4, 5};
    for (long long c: multiply(p, q)) cout << c << " ";
    cout << "\n";  // -> 4 13 22 15
    return 0;
}
