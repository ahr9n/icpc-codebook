/**
 * Aho-Corasick: match many patterns at once. Trie + BFS fail links folded into a goto
 * automaton so text scanning is O(|text|). out[u] = patterns ending at u or a suffix of it.
 */
const int ALPHA = 26;

struct AhoCorasick {
    vector<array<int, ALPHA>> go;
    vector<int> fail, out;

    AhoCorasick() {
        new_node();
    }

    int new_node() {
        go.push_back({});
        go.back().fill(-1);
        fail.push_back(0);
        out.push_back(0);
        return go.size() - 1;
    }

    void insert(string& w) {
        int u = 0;
        for (char ch: w) {
            int c = ch - 'a';
            if (go[u][c] == -1) go[u][c] = new_node();
            u = go[u][c];
        }
        out[u]++;
    }

    void build() {
        queue<int> q;
        for (int c = 0; c < ALPHA; c++) {
            if (go[0][c] == -1)
                go[0][c] = 0;
            else
                fail[go[0][c]] = 0, q.push(go[0][c]);
        }
        while (not q.empty()) {
            int u = q.front();
            q.pop();
            out[u] += out[fail[u]];
            for (int c = 0; c < ALPHA; c++) {
                int v = go[u][c];
                if (v == -1)
                    go[u][c] = go[fail[u]][c];
                else
                    fail[v] = go[fail[u]][c], q.push(v);
            }
        }
    }

    long long count_matches(string& text) {
        long long total = 0;
        int u = 0;
        for (char ch: text) {
            u = go[u][ch - 'a'];
            total += out[u];
        }
        return total;
    }
};

/**
 * Example: total occurrences of {he, she, his, hers} inside "ahishers".
 */
int main() {
    AhoCorasick ac;
    vector<string> patterns = {"he", "she", "his", "hers"};
    for (string& p: patterns) ac.insert(p);
    ac.build();

    string text = "ahishers";
    cout << ac.count_matches(text) << "\n";  // -> 4
    return 0;
}
