/**
 * Stable marriage (Gale-Shapley): free men propose down their preference list;
 * a woman tentatively holds her best proposer so far and rejects the rest.
 * men_pref[m] / women_pref[w] list the other side by DESCENDING preference (each
 * a permutation of 0..n-1). Returns husband_of[w] = man matched to woman w.
 * O(n^2). The result is man-optimal and woman-pessimal: every man gets the best
 * partner he could have in ANY stable matching, and every woman the worst.
 */
vector<int> stable_marriage(const vector<vector<int>>& men_pref,
                            const vector<vector<int>>& women_pref) {
    int n = men_pref.size();
    vector<vector<int>> woman_rank(n, vector<int>(n));
    for (int w = 0; w < n; w++)
        for (int rank = 0; rank < n; rank++) woman_rank[w][women_pref[w][rank]] = rank;

    vector<int> next_proposal(n, 0);
    vector<int> husband_of(n, -1);
    vector<int> free_men(n);
    for (int m = 0; m < n; m++) free_men[m] = m;

    while (not free_men.empty()) {
        int m = free_men.back();
        free_men.pop_back();
        int w = men_pref[m][next_proposal[m]];
        next_proposal[m]++;

        if (husband_of[w] == -1) {
            husband_of[w] = m;
        } else if (woman_rank[w][m] < woman_rank[w][husband_of[w]]) {
            free_men.push_back(husband_of[w]);
            husband_of[w] = m;
        } else {
            free_men.push_back(m);
        }
    }
    return husband_of;
}

/**
 * Example: n = 3. Print each woman and the man she ends up matched with.
 */
int main() {
    vector<vector<int>> men_pref = {{0, 1, 2}, {1, 0, 2}, {0, 1, 2}};
    vector<vector<int>> women_pref = {{1, 0, 2}, {0, 1, 2}, {0, 1, 2}};
    vector<int> husband_of = stable_marriage(men_pref, women_pref);
    for (int w = 0; w < (int)husband_of.size(); w++)
        cout << "woman " << w << " -> man " << husband_of[w] << "\n";
    // -> woman 0 -> man 1
    // -> woman 1 -> man 0
    // -> woman 2 -> man 2
    return 0;
}
