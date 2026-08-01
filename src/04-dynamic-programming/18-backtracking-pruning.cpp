int board_size;
long long used_cols;
long long used_diag;
long long used_anti;

/**
 * Backtracking with pruning, on N-Queens placement counting.
 * Place one queen per row; count full placements with no two attacking.
 * O(N!) worst case, with O(N) recursion depth. Contract: 0 <= N <= 32 so the
 * 2*N-1 diagonal masks fit in a signed 64-bit integer; the returned count must
 * fit in long long.
 *
 * Occupied-column and diagonal masks reject an invalid partial placement in
 * O(1), avoiding its entire subtree. Diagonal indices are shifted to remain
 * non-negative.
 */
long long count_queens(int row) {
    if (row == board_size) {
        return 1;
    }

    long long solutions = 0;
    for (int col = 0; col < board_size; col++) {
        int diag = row + col;
        int anti = row - col + board_size - 1;
        if (used_cols & (1LL << col)) {
            continue;
        }
        if (used_diag & (1LL << diag)) {
            continue;
        }
        if (used_anti & (1LL << anti)) {
            continue;
        }

        used_cols ^= 1LL << col;
        used_diag ^= 1LL << diag;
        used_anti ^= 1LL << anti;
        solutions += count_queens(row + 1);
        used_cols ^= 1LL << col;
        used_diag ^= 1LL << diag;
        used_anti ^= 1LL << anti;
    }
    return solutions;
}

long long n_queens(int size) {
    board_size = size;
    used_cols = 0;
    used_diag = 0;
    used_anti = 0;
    return count_queens(0);
}

/** Example: the 6- and 8-queens boards have 4 and 92 placements. */
int main() {
    cout << n_queens(6) << "\n";  // -> 4
    cout << n_queens(8) << "\n";  // -> 92
    return 0;
}
