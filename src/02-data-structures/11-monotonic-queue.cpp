/**
 * Monotonic deque for the sliding-window minimum: the min of every width-k window
 * in O(1) amortized each. The deque stores indices whose values increase front to
 * back, so its front is always the window's min. A new value evicts larger tails
 * (they can never be a future min), and the front is dropped once it slides out of
 * the window. For a sliding maximum, flip the tail comparison. Total O(n).
 */
vector<long long> sliding_min(vector<long long>& a, int k) {
    deque<int> dq;
    vector<long long> res;
    for (int i = 0; i < (int)a.size(); i++) {
        while (not dq.empty() and a[dq.back()] >= a[i]) dq.pop_back();
        dq.push_back(i);
        if (dq.front() <= i - k) dq.pop_front();
        if (i >= k - 1) res.push_back(a[dq.front()]);
    }
    return res;
}

/**
 * Example: a = {1, 3, -1, -3, 5, 3, 6, 7}, k = 3 -- window minima.
 */
int main() {
    vector<long long> a = {1, 3, -1, -3, 5, 3, 6, 7};
    for (long long m: sliding_min(a, 3)) cout << m << " ";
    cout << "\n";  // -> -1 -3 -3 -3 3 3
    return 0;
}
