// Stress: pruned n_queens vs an independent unpruned per-row brute force (explicit
// pairwise conflict checks) and the known OEIS A000170 solution counts.
int brute_queens(int row, int size, vector<int>& cols) {
    if (row == size) return 1;

    int total = 0;
    for (int col = 0; col < size; col++) {
        bool ok = true;
        for (int r = 0; r < row; r++) {
            if (cols[r] == col or abs(cols[r] - col) == row - r) {
                ok = false;
                break;
            }
        }
        if (ok) {
            cols[row] = col;
            total += brute_queens(row + 1, size, cols);
        }
    }
    return total;
}

int stress() {
    long long checks = 0;
    int known[] = {1, 1, 0, 0, 2, 10, 4, 40, 92, 352};  // solutions for size 0..9

    for (int size = 0; size <= 9; size++) {
        int got = n_queens(size);

        vector<int> cols(size, 0);
        int brute = brute_queens(0, size, cols);

        if (got != brute or got != known[size]) {
            printf("FAIL size=%d got=%d brute=%d known=%d\n", size, got, brute, known[size]);
            return 1;
        }
        checks++;
    }

    printf("backtracking-pruning PASS %lld", checks);
    return 0;
}
