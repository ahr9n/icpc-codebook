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
