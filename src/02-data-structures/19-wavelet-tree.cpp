/**
 * Wavelet tree over a static integer array. After an O(n log C) build (C is the
 * width of the value range, so log C = number of levels) each query runs in
 * O(log C):
 *   kth(l, r, k)       k-th smallest value in index range [l, r], k is 1-based
 *   count_leq(l, r, x) how many elements in [l, r] are <= x
 * Index range [l, r] is 0-based and inclusive; values may be negative.
 * Contract: the input array is non-empty and queries satisfy l <= r and
 * 1 <= k <= r - l + 1.
 *
 * A node owns a value band [lo, hi] and holds its slice of the array. Elements
 * with value <= mid recurse left, the rest recurse right (a stable partition, so
 * relative order is kept). left_count[i] = how many of the node's first i
 * elements went left; that prefix routes a query position to the correct child
 * and remaps it in O(1), which is what makes each query one walk down the tree.
 * Positions inside a node are 1-based so left_count[l - 1] is the count strictly
 * before l.
 */
struct WaveletNode {
    int lo, hi;
    WaveletNode *left = nullptr, *right = nullptr;
    vector<int> left_count;

    WaveletNode(vector<int>::iterator from, vector<int>::iterator to, int lo, int hi)
        : lo(lo), hi(hi) {
        if (lo == hi or from == to) {
            return;
        }
        int mid = lo + ((long long)hi - lo) / 2;
        left_count.reserve((to - from) + 1);
        left_count.push_back(0);
        for (auto it = from; it != to; ++it) {
            int goes_left = *it <= mid ? 1 : 0;
            left_count.push_back(left_count.back() + goes_left);
        }
        auto split = stable_partition(from, to, [mid](int x) { return x <= mid; });
        left = new WaveletNode(from, split, lo, mid);
        right = new WaveletNode(split, to, mid + 1, hi);
    }

    int kth(int l, int r, int k) {
        if (lo == hi) {
            return lo;
        }
        int in_left = left_count[r] - left_count[l - 1];
        if (k <= in_left) {
            return left->kth(left_count[l - 1] + 1, left_count[r], k);
        }
        int right_l = l - left_count[l - 1];
        int right_r = r - left_count[r];
        return right->kth(right_l, right_r, k - in_left);
    }

    int count_leq(int l, int r, int x) {
        if (l > r or x < lo) {
            return 0;
        }
        if (hi <= x) {
            return r - l + 1;
        }
        int left_l = left_count[l - 1] + 1;
        int left_r = left_count[r];
        int right_l = l - left_count[l - 1];
        int right_r = r - left_count[r];
        return left->count_leq(left_l, left_r, x) + right->count_leq(right_l, right_r, x);
    }
};

struct WaveletTree {
    WaveletNode* root;

    WaveletTree(vector<int> values) {
        int lo = *min_element(values.begin(), values.end());
        int hi = *max_element(values.begin(), values.end());
        root = new WaveletNode(values.begin(), values.end(), lo, hi);
    }

    int kth(int l, int r, int k) {
        return root->kth(l + 1, r + 1, k);
    }

    int count_leq(int l, int r, int x) {
        return root->count_leq(l + 1, r + 1, x);
    }
};

/** Example: k-th-smallest and threshold-count queries with duplicates. */
int main() {
    vector<int> values = {3, -1, 4, 1, 5, -1, 2, 3};

    WaveletTree wt(values);

    cout << wt.kth(0, 7, 1) << "\n";
    cout << wt.kth(0, 7, 4) << "\n";
    cout << wt.kth(2, 5, 2) << "\n";
    cout << wt.count_leq(0, 7, 3) << "\n";
    cout << wt.count_leq(2, 5, 1) << "\n";
    return 0;
}
// -> -1
// -> 2
// -> 1
// -> 6
// -> 2
