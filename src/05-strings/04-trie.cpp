/**
 * Trie over lowercase letters: insert words, then whole-word / prefix match. O(|word|).
 * t[u].next[c] = child node or -1; t[u].words_cnt = words ending at u; t[0] = root.
 */
const int ALPHA = 26;

struct Node {
    array<int, ALPHA> next;
    int words_cnt = 0;

    Node() {
        next.fill(-1);
    }
};

struct Trie {
    vector<Node> t;

    Trie() {
        new_node();
    }

    int new_node() {
        t.emplace_back();
        return t.size() - 1;
    }

    void insert(string& w) {
        int u = 0;
        for (char ch: w) {
            int c = ch - 'a';
            if (t[u].next[c] == -1) t[u].next[c] = new_node();
            u = t[u].next[c];
        }
        t[u].words_cnt++;
    }

    bool contains_word(string& w) {
        int u = 0;
        for (char ch: w) {
            int c = ch - 'a';
            if (t[u].next[c] == -1) return false;
            u = t[u].next[c];
        }
        return t[u].words_cnt > 0;
    }

    bool contains_prefix(string& w) {
        int u = 0;
        for (char ch: w) {
            int c = ch - 'a';
            if (t[u].next[c] == -1) return false;
            u = t[u].next[c];
        }
        return true;
    }
};

/**
 * Example: insert a word, then query whole-word and prefix membership.
 */
int main() {
    Trie tr;
    string apple = "apple", app = "app", banana = "banana";
    tr.insert(apple);
    cout << tr.contains_word(apple) << tr.contains_prefix(app) << tr.contains_word(banana)
         << "\n";  // -> 110
    return 0;
}
