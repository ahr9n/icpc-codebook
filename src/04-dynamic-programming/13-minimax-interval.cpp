int n;
vector<int> a;
vector<vector<long long>> dp;

/**
 * Nim: first player wins iff the XOR of all pile sizes is nonzero. O(n)
 */
bool nim_first_wins(vector<int>& piles) {
    int x = 0;
    for (int p: piles) x ^= p;
    return x != 0;
}
