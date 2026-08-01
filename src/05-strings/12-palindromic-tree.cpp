/**
 * Palindromic tree (eertree): stores every distinct palindromic substring as one
 * node, built online one character at a time in O(n log |alphabet|). There are two
 * roots -- an imaginary one of length -1 and the empty string of length 0 -- plus a
 * node per distinct palindrome, so the number of distinct palindromic substrings is
 * exactly node_count - 2. Each node's suffix link points to its longest proper
 * palindromic suffix; following that chain from `last` (the longest palindromic
 * suffix of the current prefix) enumerates every palindromic suffix of the prefix,
 * so its depth counts the palindromic substrings ending at the last added position.
 * The insertion trick relies on the imaginary root's length -1: it lets a lone
 * character c wrap the "palindrome before the string" into c itself.
 */
struct Eertree {
    struct Node {
        long long len;
        int link;
        map<char, int> next;
    };
    vector<Node> node;
    vector<int> depth;
    string s;
    int last;

    Eertree() {
        node.push_back({-1, 0, {}});
        node.push_back({0, 0, {}});
        depth = {0, 0};
        last = 1;
    }

    int walk_suffix(int v) {
        int i = (int)s.size() - 1;
        while (i - node[v].len - 1 < 0 or s[i - node[v].len - 1] != s[i]) v = node[v].link;
        return v;
    }

    // Returns the length of the longest palindromic suffix after appending c.
    long long add(char c) {
        s.push_back(c);
        int cur = walk_suffix(last);
        auto it = node[cur].next.find(c);
        if (it != node[cur].next.end()) {
            last = it->second;
            return node[last].len;
        }
        int v = node.size();
        node.push_back({node[cur].len + 2, 0, {}});
        if (node[v].len == 1) {
            node[v].link = 1;
        } else {
            node[v].link = node[walk_suffix(node[cur].link)].next[c];
        }
        node[cur].next[c] = v;
        depth.push_back(depth[node[v].link] + 1);
        last = v;
        return node[v].len;
    }

    long long count_distinct() {
        return (long long)node.size() - 2;
    }

    // Palindromic substrings ending at the last added position (link-tree depth).
    int palindromes_ending_here() {
        return depth[last];
    }
};

/**
 * Example: "aabaa" has 5 distinct palindromic substrings -- a, aa, b, aba, aabaa.
 */
int main() {
    Eertree tree;
    for (char c: string("aabaa")) tree.add(c);
    cout << tree.count_distinct() << "\n";  // -> 5
    return 0;
}
