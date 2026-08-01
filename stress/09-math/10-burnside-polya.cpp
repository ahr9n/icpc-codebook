// Enumerate all short color strings and canonicalize them under rotations and
// reflections, independently counting the resulting symmetry orbits.
int stress() {
    long long classified = 0;

    for (int n = 1; n <= 8; n++) {
        for (int colors = 1; colors <= 4; colors++) {
            set<vector<int>> necklace_orbits;
            set<vector<int>> bracelet_orbits;
            long long total = 1;
            for (int i = 0; i < n; i++) {
                total *= colors;
            }

            for (long long mask = 0; mask < total; mask++) {
                long long encoded = mask;
                vector<int> coloring(n);
                for (int i = 0; i < n; i++) {
                    coloring[i] = encoded % colors;
                    encoded /= colors;
                }

                vector<int> best_rotation = coloring;
                vector<int> best_dihedral = coloring;
                for (int shift = 0; shift < n; shift++) {
                    vector<int> rotated(n);
                    vector<int> reflected(n);
                    for (int i = 0; i < n; i++) {
                        rotated[i] = coloring[(i + shift) % n];
                        reflected[i] = coloring[(shift - i + n) % n];
                    }
                    best_rotation = min(best_rotation, rotated);
                    best_dihedral = min(best_dihedral, min(rotated, reflected));
                }
                necklace_orbits.insert(best_rotation);
                bracelet_orbits.insert(best_dihedral);
                classified++;
            }

            long long necklaces = count_necklaces(n, colors);
            long long bracelets = count_bracelets(n, colors);
            if (necklaces != (long long)necklace_orbits.size() or
                bracelets != (long long)bracelet_orbits.size()) {
                printf("burnside FAIL n=%d colors=%d expected=%zu/%zu actual=%lld/%lld\n", n,
                       colors, necklace_orbits.size(), bracelet_orbits.size(), necklaces,
                       bracelets);
                return 1;
            }
        }
    }

    printf("burnside-polya PASS %lld", classified);
    return 0;
}
