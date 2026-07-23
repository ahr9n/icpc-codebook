vector<int> coins;

/**
 * Grundy number of a state: mex of the grundy values of reachable states.
 * Sum of independent games = XOR of their grundy values (Sprague-Grundy). O(states*moves)
 */
vector<int> grundy;

int grundy_val(int state) {
    if (state == 0) return 0;
    if (grundy[state] != -1) return grundy[state];
    set<int> reachable;
    for (int move: coins)
        if (state >= move) reachable.insert(grundy_val(state - move));
    int mex = 0;
    while (reachable.count(mex)) mex++;
    return grundy[state] = mex;
}

/**
 * Example: subtraction game moves {1,2}; Grundy value of state 4 is 1.
 */
int main() {
    coins = {1, 2};
    int state = 4;
    grundy.assign(state + 1, -1);
    cout << grundy_val(state) << "\n";  // -> 1
    return 0;
}
