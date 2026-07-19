/**
 * Big integer add: schoolbook addition of two decimal strings. O(len)
 */
string big_add(string a, string b) {
    string res;
    int i = a.size() - 1, j = b.size() - 1, carry = 0;
    while (i >= 0 or j >= 0 or carry) {
        int sum = carry;
        if (i >= 0) sum += a[i--] - '0';
        if (j >= 0) sum += b[j--] - '0';
        res += char('0' + sum % 10);
        carry = sum / 10;
    }
    reverse(res.begin(), res.end());
    return res;
}
