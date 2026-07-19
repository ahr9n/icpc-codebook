/**
 * Trie over lowercase letters: insert words, then count/prefix-match. O(|word|) per op.
 * child[node][c] = next node or -1; cnt = words ending here.
 */
const int ALPHA = 26;

struct Trie {
    vector<array<int, ALPHA>> child;
    vector<int> cnt;

    Trie() {
        new_node();
    }

    int new_node() {
        child.push_back({});
        child.back().fill(-1);
        cnt.push_back(0);
        return child.size() - 1;
    }

    void insert(string& w) {
        int u = 0;
        for (char ch: w) {
            int c = ch - 'a';
            if (child[u][c] == -1) child[u][c] = new_node();
            u = child[u][c];
        }
        cnt[u]++;
    }

    bool contains(string& w) {
        int u = 0;
        for (char ch: w) {
            int c = ch - 'a';
            if (child[u][c] == -1) return false;
            u = child[u][c];
        }
        return cnt[u] > 0;
    }
};
