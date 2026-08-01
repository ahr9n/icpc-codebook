/**
 * Lyndon decomposition via Duval's algorithm: split s into a non-increasing
 * sequence of Lyndon words (a Lyndon word is strictly smaller than all its
 * proper suffixes). The factorization is unique. duval runs in O(n); the same
 * Duval scan over s+s yields min_rotation, the start index of a
 * lexicographically smallest rotation, also in O(n). For an empty string,
 * duval returns no factors and min_rotation returns 0.
 */
vector<string> duval(const string& s) {
    int len = s.size();
    vector<string> factors;
    int start = 0;
    while (start < len) {
        int candidate = start, lookahead = start + 1;
        while (lookahead < len and s[candidate] <= s[lookahead]) {
            if (s[candidate] < s[lookahead])
                candidate = start;
            else
                candidate++;
            lookahead++;
        }
        int period = lookahead - candidate;
        while (start <= candidate) {
            factors.push_back(s.substr(start, period));
            start += period;
        }
    }
    return factors;
}

int min_rotation(const string& s) {
    int len = s.size();
    string doubled = s + s;
    int best = 0, start = 0;
    while (start < len) {
        best = start;
        int candidate = start, lookahead = start + 1;
        while (lookahead < 2 * len and doubled[candidate] <= doubled[lookahead]) {
            if (doubled[candidate] < doubled[lookahead])
                candidate = start;
            else
                candidate++;
            lookahead++;
        }
        int period = lookahead - candidate;
        while (start <= candidate) {
            start += period;
        }
    }
    return best;
}

/** Example: factor bananas and rotate bbaaccaab to its minimum form. */
int main() {
    string factored;
    for (const string& factor: duval("bananas")) {
        if (not factored.empty()) {
            factored += '|';
        }
        factored += factor;
    }
    cout << factored << '\n';  // -> b|ananas

    string cyclic = "bbaaccaab";
    int start = min_rotation(cyclic);
    cout << start << ' ' << cyclic.substr(start) + cyclic.substr(0, start) << '\n';
    // -> 6 aabbbaacc
    return 0;
}
