/**
 * Randomness with mt19937_64. Seed once, globally, with chrono ^ random_device so it survives
 * a weak entropy source (random_device is deterministic on old MinGW) and still varies per
 * run to defeat precomputed anti-tests. Draw ranges with uniform_int_distribution, never
 * rng() % n -- modulo is biased and anti-hackable. Reuse rng for hash bases, treap
 * priorities, random pivots, and shuffle-before-sort to dodge anti-quicksort TLE cases.
 */
mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count() ^ random_device{}());

long long rand_int(long long lo, long long hi) {  // uniform in [lo, hi]
    return uniform_int_distribution<long long>(lo, hi)(rng);
}

/**
 * Example: draws stay in range and shuffle is a permutation (invariants, so output is stable
 * despite the random seed).
 */
int main() {
    bool in_range = true;
    for (int i = 0; i < 1000; i++) {
        long long x = rand_int(1, 6);
        in_range = in_range and 1 <= x and x <= 6;
    }
    cout << in_range << "\n";  // -> 1

    vector<int> v = {1, 2, 3, 4, 5}, shuffled = v;
    shuffle(shuffled.begin(), shuffled.end(), rng);
    sort(shuffled.begin(), shuffled.end());
    cout << (v == shuffled) << "\n";  // -> 1
    return 0;
}
