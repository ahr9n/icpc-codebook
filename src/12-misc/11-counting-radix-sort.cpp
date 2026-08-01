/**
 * Integer-key sorts that beat comparison sort when keys are small or fixed width.
 *
 * counting_sort(a, max_val): stable, O(n + max_val). Requires 0 <= a[i] <= max_val.
 *   Wins over std::sort for large n with a small key range (e.g. n = 10^6 keys in
 *   [0, 10^5]). Stability contract: elements with equal keys keep their input order,
 *   which is exactly what lets radix chain digit passes.
 *
 * radix_sort(a): O(n * bytes) with base-256 LSD passes over the 4 machine bytes of a
 *   32-bit key; here bytes = 4, so 4 stable counting passes. Handles negatives by
 *   flipping the sign bit, which maps two's-complement order onto unsigned order.
 *   Wins over std::sort for large n of fixed-width integers regardless of value range.
 */
vector<int> counting_sort(const vector<int>& a, int max_val) {
    vector<int> cnt(max_val + 2, 0);
    for (int x: a) cnt[x + 1]++;
    for (int i = 1; i < (int)cnt.size(); i++) cnt[i] += cnt[i - 1];
    vector<int> out(a.size());
    for (int x: a) out[cnt[x]++] = x;
    return out;
}

void radix_sort(vector<int>& a) {
    int n = a.size();
    if (n == 0) return;
    const int radix = 256, byte_bits = 8, byte_count = 4;
    const unsigned mask = radix - 1, sign_bit = 0x80000000u;
    vector<unsigned> key(n), buf(n);
    for (int i = 0; i < n; i++) key[i] = (unsigned)a[i] ^ sign_bit;
    for (int pass = 0; pass < byte_count; pass++) {
        int shift = pass * byte_bits;
        vector<int> cnt(radix + 1, 0);
        for (int i = 0; i < n; i++) cnt[((key[i] >> shift) & mask) + 1]++;
        for (int b = 1; b <= radix; b++) cnt[b] += cnt[b - 1];
        for (int i = 0; i < n; i++) {
            unsigned bucket = (key[i] >> shift) & mask;
            buf[cnt[bucket]++] = key[i];
        }
        swap(key, buf);
    }
    for (int i = 0; i < n; i++) a[i] = (int)(key[i] ^ sign_bit);
}

/**
 * Example: counting sort of small non-negative keys, radix sort of mixed signs.
 */
int main() {
    vector<int> a = {4, 1, 3, 4, 0, 2, 1};
    vector<int> sorted_small = counting_sort(a, 4);
    for (int x: sorted_small) cout << x << " ";  // -> 0 1 1 2 3 4 4
    cout << "\n";
    vector<int> b = {5, -3, 42, 0, -1, 7, -100, 5};
    radix_sort(b);
    for (int x: b) cout << x << " ";  // -> -100 -3 -1 0 5 5 7 42
    cout << "\n";
    return 0;
}
