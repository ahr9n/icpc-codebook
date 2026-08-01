int board_size;
int used_cols;
int used_diag;
int used_anti;

/**
 * Backtracking with pruning, on N-Queens placement counting.
 * Place one queen per row; count full placements with no two attacking.
 * O(N!) worst case, but the feasibility prune collapses most of the tree.
 *
 * General recipe:
 *   1. order the decision variables (here: fill rows top to bottom),
 *   2. bound/feasibility prune: skip any partial that already can't extend,
 *   3. symmetry breaking: fix a choice to avoid recounting mirror images.
 *
 * The prune here is why this beats brute force: a column and the two diagonals
 * through a square are each attacked by at most one queen, so we mask them out
 * in used_cols / used_diag / used_anti and never even descend into a placement
 * that a later row could not survive. Each bitmask lookup rejects a whole
 * subtree of doomed completions in O(1). Diagonal index row+col is constant
 * along a "\" diagonal; anti-diagonal index row-col+board_size-1 is constant
 * along a "/" diagonal (shifted to stay non-negative).
 */
int count_queens(int row) {
    if (row == board_size) {
        return 1;
    }

    int solutions = 0;
    for (int col = 0; col < board_size; col++) {
        int diag = row + col;
        int anti = row - col + board_size - 1;
        if (used_cols & (1 << col)) {
            continue;
        }
        if (used_diag & (1 << diag)) {
            continue;
        }
        if (used_anti & (1 << anti)) {
            continue;
        }

        used_cols ^= (1 << col);
        used_diag ^= (1 << diag);
        used_anti ^= (1 << anti);
        solutions += count_queens(row + 1);
        used_cols ^= (1 << col);
        used_diag ^= (1 << diag);
        used_anti ^= (1 << anti);
    }
    return solutions;
}

int n_queens(int size) {
    board_size = size;
    used_cols = 0;
    used_diag = 0;
    used_anti = 0;
    return count_queens(0);
}

/**
 * Example: distinct N-Queens placements for n = 6 and n = 8.
 */
int main() {
    cout << n_queens(6) << "\n";  // -> 4
    cout << n_queens(8) << "\n";  // -> 92
    return 0;
}
