/**
 * Monotonic stack for the largest rectangle in a histogram. Hold bar indices in
 * a stack of non-decreasing heights; when a shorter bar arrives, pop each taller
 * bar and settle its maximal rectangle -- its height times the width now fixed on
 * both sides (the new bar bounds the right, the stack's next entry the left). A
 * trailing zero-height sentinel flushes whatever remains. O(n).
 */
long long largest_rectangle(vector<long long> heights) {
    int n = heights.size();
    heights.push_back(0);
    vector<int> stk;
    long long best = 0;
    for (int i = 0; i <= n; i++) {
        while (not stk.empty() and heights[stk.back()] >= heights[i]) {
            long long height = heights[stk.back()];
            stk.pop_back();
            int left = stk.empty() ? -1 : stk.back();
            long long width = i - left - 1;
            best = max(best, height * width);
        }
        stk.push_back(i);
    }
    return best;
}

/**
 * Example: heights {2, 1, 5, 6, 2, 3} -- the 5,6 pair gives 2 * 5 = 10.
 */
int main() {
    vector<long long> heights = {2, 1, 5, 6, 2, 3};
    cout << largest_rectangle(heights) << "\n";  // -> 10
    return 0;
}
