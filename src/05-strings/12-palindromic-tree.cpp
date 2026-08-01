/**
 * Palindromic tree (eertree): stores every distinct palindromic substring as one
 * node, built online in O(n log |alphabet|) time and O(n) nodes. Two roots
 * represent lengths -1 and 0. Every other node is one distinct palindrome, and
 * suffix links connect palindromes to their longest proper palindromic suffix.
 * Therefore node_count - 2 is the distinct count, while the suffix-link depth
 * of last is the number of palindromes ending at the newest character.
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
        while (i - node[v].len - 1 < 0 or s[i - node[v].len - 1] != s[i]) {
            v = node[v].link;
        }
        return v;
    }

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

    int palindromes_ending_here() {
        return depth[last];
    }
};

/** Example: aabaa contains five distinct palindromic substrings. */
int main() {
    Eertree tree;
    for (char c: string("aabaa")) {
        tree.add(c);
    }
    cout << tree.count_distinct() << "\n";  // -> 5
    return 0;
}
