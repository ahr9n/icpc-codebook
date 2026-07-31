/**
 * 2-SAT: satisfy a conjunction of clauses (a OR b) over boolean variables and
 * recover one satisfying assignment when it exists. Each variable i owns two
 * literal-nodes, 2i for "i is true" and 2i+1 for "i is false"; a clause (a OR b)
 * adds the implications (not a -> b) and (not b -> a). A formula is satisfiable
 * iff no variable has both literals in one SCC. Tarjan numbers SCCs in reverse
 * topological order, so the literal with the smaller component id is forced
 * true. Build with add_or(...), then call solve(). O(V + clauses).
 */
struct TwoSat {
    int n_vars;
    vector<vector<int>> g;
    vector<int> assignment;

    TwoSat(int n_vars) : n_vars(n_vars), g(2 * n_vars) {}

    // literal-node for "variable v equals value is_true"
    int node(int v, bool is_true) {
        return 2 * v + (is_true ? 0 : 1);
    }

    // clause (var_a == val_a) OR (var_b == val_b)
    void add_or(int var_a, bool val_a, int var_b, bool val_b) {
        int a = node(var_a, val_a), b = node(var_b, val_b);
        g[a ^ 1].push_back(b);
        g[b ^ 1].push_back(a);
    }

    // force (var == val) to hold
    void add_fact(int var, bool val) {
        add_or(var, val, var, val);
    }

    vector<int> comp, low, num, stk;
    vector<bool> on_stk;
    int timer, scc_cnt;

    void scc_dfs(int u) {
        low[u] = num[u] = timer++;
        stk.push_back(u);
        on_stk[u] = true;
        for (int v: g[u]) {
            if (num[v] == -1) {
                scc_dfs(v);
                low[u] = min(low[u], low[v]);
            } else if (on_stk[v])
                low[u] = min(low[u], num[v]);
        }
        if (low[u] == num[u]) {
            while (true) {
                int v = stk.back();
                stk.pop_back();
                on_stk[v] = false;
                comp[v] = scc_cnt;
                if (v == u) break;
            }
            scc_cnt++;
        }
    }

    bool solve() {
        int m = 2 * n_vars;
        comp.assign(m, -1);
        low.assign(m, 0);
        num.assign(m, -1);
        on_stk.assign(m, false);
        stk.clear();
        timer = scc_cnt = 0;
        for (int u = 0; u < m; u++)
            if (num[u] == -1) scc_dfs(u);

        assignment.assign(n_vars, 0);
        for (int v = 0; v < n_vars; v++) {
            if (comp[2 * v] == comp[2 * v + 1]) return false;
            assignment[v] = comp[2 * v] < comp[2 * v + 1];
        }
        return true;
    }
};

/**
 * Example: (x0 or x1) and (not x0 or x2) and (not x1 or not x2).
 */
int main() {
    TwoSat sat(3);
    sat.add_or(0, true, 1, true);
    sat.add_or(0, false, 2, true);
    sat.add_or(1, false, 2, false);
    if (sat.solve()) {
        cout << "SAT";
        for (int v = 0; v < 3; v++) cout << ' ' << sat.assignment[v];
        cout << '\n';
    } else
        cout << "UNSAT\n";
    return 0;
}
// -> SAT 1 0 1
