/**
 * Z-function: z[i] = length of the longest substring starting at i matching a prefix of s.
 * Pattern matching / periodicity. O(n)
 */
vector<int> z_function(string& s) {
    int len = s.size();
    vector<int> z(len, 0);
    z[0] = len;
    for (int i = 1, l = 0, r = 0; i < len; i++) {
        if (i < r) z[i] = min(r - i, z[i - l]);
        while (i + z[i] < len and s[z[i]] == s[i + z[i]]) z[i]++;
        if (i + z[i] > r) l = i, r = i + z[i];
    }
    return z;
}
