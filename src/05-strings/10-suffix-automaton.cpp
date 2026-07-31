struct SamState {
    long long len;
    int link;
    map<char, int> next;
};
vector<SamState> sam;
int last;

void sam_init() {
    sam.assign(1, {0, -1, {}});
    last = 0;
}

/**
 * Suffix automaton: the minimal DFA accepting every suffix of a string, built
 * online one character at a time. Each state owns an endpos-equivalence class of
 * substrings whose lengths fill (len[link] .. len]; the suffix link points to the
 * class of the longest proper suffix that occurs strictly more often. Extending
 * may clone a state to keep transitions minimal. At most 2n-1 states and 3n-4
 * transitions, so O(n log |alphabet|) total with a map per state.
 */
void sam_extend(char c) {
    int cur = sam.size();
    sam.push_back({sam[last].len + 1, -1, {}});
    int p = last;
    while (p != -1 and not sam[p].next.count(c)) {
        sam[p].next[c] = cur;
        p = sam[p].link;
    }
    if (p == -1) {
        sam[cur].link = 0;
    } else {
        int q = sam[p].next[c];
        if (sam[p].len + 1 == sam[q].len) {
            sam[cur].link = q;
        } else {
            int clone = sam.size();
            sam.push_back({sam[p].len + 1, sam[q].link, sam[q].next});
            while (p != -1) {
                auto it = sam[p].next.find(c);
                if (it == sam[p].next.end() or it->second != q) break;
                it->second = clone;
                p = sam[p].link;
            }
            sam[q].link = clone;
            sam[cur].link = clone;
        }
    }
    last = cur;
}

/**
 * Number of distinct non-empty substrings: every substring maps to exactly one
 * state, contributing len[v] - len[link[v]] new lengths there. O(states).
 */
long long count_distinct_substrings() {
    long long total = 0;
    for (int v = 1; v < (int)sam.size(); v++) total += sam[v].len - sam[sam[v].link].len;
    return total;
}

/**
 * Longest common substring of a and b: build a's automaton, then walk b, growing
 * the match while a transition exists and following suffix links when it breaks.
 * O(|a| + |b| log |alphabet|).
 */
long long longest_common_substring(string a, string b) {
    sam_init();
    for (char c: a) sam_extend(c);
    long long best = 0, cur_len = 0;
    int v = 0;
    for (char c: b) {
        while (v != 0 and not sam[v].next.count(c)) {
            v = sam[v].link;
            cur_len = sam[v].len;
        }
        auto it = sam[v].next.find(c);
        if (it != sam[v].next.end()) {
            v = it->second;
            cur_len++;
        } else {
            v = 0;
            cur_len = 0;
        }
        best = max(best, cur_len);
    }
    return best;
}

/**
 * Example: "abcbc" has 12 distinct substrings; the longest common substring of
 * "abcbc" and "dbcbd" is "bcb", length 3.
 */
int main() {
    sam_init();
    for (char c: string("abcbc")) sam_extend(c);
    cout << count_distinct_substrings() << "\n";                 // -> 12
    cout << longest_common_substring("abcbc", "dbcbd") << "\n";  // -> 3
    return 0;
}
