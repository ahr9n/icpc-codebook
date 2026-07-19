/**
 * Binary search on the answer (integer): smallest x in [lo, hi] with feasible(x) true.
 * feasible must be monotonic (false...false, true...true). O(log range * check)
 */
long long bin_search_answer(long long lo, long long hi, function<bool(long long)> feasible) {
    while (lo < hi) {
        long long mid = lo + (hi - lo) / 2;
        if (feasible(mid))
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}
