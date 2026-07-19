vector<int> coins;

/**
 * Game/Grundy win-lose DP: a position is winning iff some move leads to a losing one.
 * win[rem] over remaining state; moves = the allowed subtractions. O(states * moves)
 */
vector<int> win;

bool game_win(int rem) {
    if (rem == 0) return false;
    if (win[rem] != -1) return win[rem];
    win[rem] = 0;
    for (int move: coins)
        if (rem >= move and not game_win(rem - move)) win[rem] = 1;
    return win[rem];
}
