// Stress: Gale-Shapley output is a perfect stable matching and is man-optimal
// among every stable matching, enumerated exhaustively for small random instances.
int stress() {
    mt19937 rng(31);
    long long checks = 0;

    for (int rep = 0; rep < 3000; rep++) {
        int n = 1 + rng() % 7;
        vector<vector<int>> men_pref(n, vector<int>(n));
        vector<vector<int>> women_pref(n, vector<int>(n));
        for (int i = 0; i < n; i++) {
            iota(men_pref[i].begin(), men_pref[i].end(), 0);
            iota(women_pref[i].begin(), women_pref[i].end(), 0);
            shuffle(men_pref[i].begin(), men_pref[i].end(), rng);
            shuffle(women_pref[i].begin(), women_pref[i].end(), rng);
        }

        vector<vector<int>> man_rank(n, vector<int>(n));
        vector<vector<int>> woman_rank(n, vector<int>(n));
        for (int m = 0; m < n; m++) {
            for (int rank = 0; rank < n; rank++) {
                man_rank[m][men_pref[m][rank]] = rank;
            }
        }
        for (int w = 0; w < n; w++) {
            for (int rank = 0; rank < n; rank++) {
                woman_rank[w][women_pref[w][rank]] = rank;
            }
        }

        auto is_stable = [&](const vector<int>& husband_of) {
            vector<int> wife_of(n);
            for (int w = 0; w < n; w++) {
                wife_of[husband_of[w]] = w;
            }
            for (int m = 0; m < n; m++) {
                for (int w = 0; w < n; w++) {
                    if (man_rank[m][w] < man_rank[m][wife_of[m]] and
                        woman_rank[w][m] < woman_rank[w][husband_of[w]]) {
                        return false;
                    }
                }
            }
            return true;
        };

        vector<int> got = stable_marriage(men_pref, women_pref);
        vector<int> sorted_got = got;
        sort(sorted_got.begin(), sorted_got.end());
        for (int i = 0; i < n; i++) {
            if (sorted_got[i] != i) {
                printf("stable-marriage FAIL(perfect) rep=%d\n", rep);
                return 1;
            }
        }
        if (not is_stable(got)) {
            printf("stable-marriage FAIL(stable) rep=%d\n", rep);
            return 1;
        }
        checks++;

        vector<int> wife_of_got(n);
        for (int w = 0; w < n; w++) {
            wife_of_got[got[w]] = w;
        }

        vector<int> candidate(n);
        iota(candidate.begin(), candidate.end(), 0);
        do {
            if (not is_stable(candidate)) {
                continue;
            }
            vector<int> candidate_wife(n);
            for (int w = 0; w < n; w++) {
                candidate_wife[candidate[w]] = w;
            }
            for (int m = 0; m < n; m++) {
                if (man_rank[m][candidate_wife[m]] < man_rank[m][wife_of_got[m]]) {
                    printf("stable-marriage FAIL(man-optimal) rep=%d m=%d\n", rep, m);
                    return 1;
                }
                checks++;
            }
        } while (next_permutation(candidate.begin(), candidate.end()));
    }

    printf("stable-marriage PASS %lld", checks);
    return 0;
}
