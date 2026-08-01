/**
 * Longest common subsequence in linear memory.
 *
 * lcs_length returns only the length in O(n*m) time and O(min(n,m)) memory.
 * hirschberg reconstructs one LCS in O(n*m) time and O(n+m) peak memory. It
 * finds where an optimal solution crosses the middle row using forward and
 * backward linear-space DP passes, so reconstruction needs no full DP table.
 */
vector<long long> lcs_row(const string& a, int a_left, int a_right, const string& b, int b_left,
                          int b_right, bool reversed) {
    int n = a_right - a_left, m = b_right - b_left;
    vector<long long> prev(m + 1, 0), cur(m + 1, 0);
    for (int i = 1; i <= n; i++) {
        char from_a = reversed ? a[a_right - i] : a[a_left + i - 1];
        for (int j = 1; j <= m; j++) {
            char from_b = reversed ? b[b_right - j] : b[b_left + j - 1];
            if (from_a == from_b) {
                cur[j] = prev[j - 1] + 1;
            } else {
                cur[j] = max(prev[j], cur[j - 1]);
            }
        }
        swap(prev, cur);
    }
    return prev;
}

long long lcs_length(const string& a, const string& b) {
    if (a.size() < b.size()) {
        return lcs_length(b, a);
    }
    return lcs_row(a, 0, a.size(), b, 0, b.size(), false).back();
}

void build_lcs(const string& a, int a_left, int a_right, const string& b, int b_left, int b_right,
               string& result) {
    if (a_left == a_right or b_left == b_right) {
        return;
    }
    if (a_right - a_left == 1) {
        for (int i = b_left; i < b_right; i++) {
            if (b[i] == a[a_left]) {
                result.push_back(a[a_left]);
                return;
            }
        }
        return;
    }

    int a_mid = (a_left + a_right) / 2;
    int b_size = b_right - b_left;
    int b_mid = b_left;
    {
        vector<long long> forward = lcs_row(a, a_left, a_mid, b, b_left, b_right, false);
        vector<long long> backward = lcs_row(a, a_mid, a_right, b, b_left, b_right, true);
        long long best = -1;
        for (int prefix = 0; prefix <= b_size; prefix++) {
            long long length = forward[prefix] + backward[b_size - prefix];
            if (length > best) {
                best = length;
                b_mid = b_left + prefix;
            }
        }
    }

    build_lcs(a, a_left, a_mid, b, b_left, b_mid, result);
    build_lcs(a, a_mid, a_right, b, b_mid, b_right, result);
}

string hirschberg(const string& a, const string& b) {
    string result;
    result.reserve(min(a.size(), b.size()));
    build_lcs(a, 0, a.size(), b, 0, b.size(), result);
    return result;
}

/** Example: one LCS of AGGTAB and GXTXAYB is GTAB. */
int main() {
    string a = "AGGTAB", b = "GXTXAYB";
    cout << lcs_length(a, b) << "\n";  // -> 4
    cout << hirschberg(a, b) << "\n";  // -> GTAB
    return 0;
}
