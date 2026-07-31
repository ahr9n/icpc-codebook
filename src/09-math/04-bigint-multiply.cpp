/**
 * Big integer multiply: schoolbook multiplication of two decimal strings. O(len^2)
 */
string big_mul(string a, string b) {
    int p = a.size(), q = b.size();
    vector<int> digits(p + q, 0);
    for (int i = p - 1; i >= 0; i--)
        for (int j = q - 1; j >= 0; j--) digits[i + j + 1] += (a[i] - '0') * (b[j] - '0');
    for (int i = p + q - 1; i > 0; i--) {
        digits[i - 1] += digits[i] / 10;
        digits[i] %= 10;
    }
    string res;
    int start = 0;
    while (start < p + q - 1 and digits[start] == 0) start++;
    for (int i = start; i < p + q; i++) res += char('0' + digits[i]);
    return res;
}
