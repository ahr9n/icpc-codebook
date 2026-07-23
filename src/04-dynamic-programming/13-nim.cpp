/**
 * Nim: first player wins iff the XOR of all pile sizes is nonzero. O(n)
 */
bool nim_first_wins(vector<int>& piles) {
    int x = 0;
    for (int p: piles) x ^= p;
    return x != 0;
}

/**
 * Example: Nim piles {3,4,5}; XOR = 2 != 0, so the first player wins.
 */
int main() {
    vector<int> piles = {3, 4, 5};
    cout << nim_first_wins(piles) << "\n";  // -> 1
    return 0;
}
