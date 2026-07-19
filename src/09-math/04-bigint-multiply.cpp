/**
 * Big integer multiply: schoolbook multiplication of two decimal strings. O(len^2)
 */
string big_mul(string a, string b) {
    int p = a.size(), q = b.size();
    vector<int> d(p + q, 0);
    for (int i = p - 1; i >= 0; i--)
        for (int j = q - 1; j >= 0; j--) d[i + j + 1] += (a[i] - '0') * (b[j] - '0');
    for (int i = p + q - 1; i > 0; i--) {
        d[i - 1] += d[i] / 10;
        d[i] %= 10;
    }
    string res;
    int start = 0;
    while (start < p + q - 1 and d[start] == 0) start++;
    for (int i = start; i < p + q; i++) res += char('0' + d[i]);
    return res;
}
