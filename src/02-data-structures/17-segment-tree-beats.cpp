/**
 * Segment Tree Beats: range chmin (a[i] = min(a[i], x) on [l, r]) with range-sum
 * and range-max queries, in amortized O(n log^2 n) over any sequence of updates.
 *
 * Each node tracks its maximum, its strict second maximum, how many entries equal
 * that maximum, and the range sum. A chmin(x) only touches entries that exceed x,
 * so it stops early at a node whenever x >= max (nothing changes) and resolves in
 * O(1) whenever second_max < x < max (only the max-valued entries drop to x, so
 * sum shrinks by (max - x) * count_max and second_max is untouched). It recurses
 * further only when x lies at or below second_max -- the rare case that splits the
 * top level into two distinct values. The amortized bound follows because each
 * such deep recursion permanently collapses two distinct level-maxima into one,
 * and the total number of distinct values that can ever be destroyed across the
 * whole tree is O(n log n); paying one extra log for the tree height gives the
 * O(n log^2 n) total. The pushed-down tag is just a max-ceiling: applying it to a
 * child always meets the O(1) case, since a parent's max is >= each child's
 * second_max by construction.
 *
 * chmax (a[i] = max(a[i], x)) and range-add extend the same idea but need the
 * symmetric (min, second_min, count_min) trio and add-lazy, so more state per node.
 */
const long long BEATS_INF = LINF;

struct SegTreeBeats {
    int n;
    vector<long long> sum, max_val, second_max, count_max;

    SegTreeBeats(const vector<long long>& values) : n(values.size()) {
        sum.assign(4 * n, 0);
        max_val.assign(4 * n, 0);
        second_max.assign(4 * n, 0);
        count_max.assign(4 * n, 0);
        build(1, 0, n - 1, values);
    }

    void pull(int i) {
        int left = 2 * i;
        int right = 2 * i + 1;
        sum[i] = sum[left] + sum[right];
        max_val[i] = max(max_val[left], max_val[right]);

        if (max_val[left] == max_val[right]) {
            second_max[i] = max(second_max[left], second_max[right]);
            count_max[i] = count_max[left] + count_max[right];
            return;
        }

        int larger = max_val[left] > max_val[right] ? left : right;
        int smaller = max_val[left] > max_val[right] ? right : left;
        second_max[i] = max(second_max[larger], max_val[smaller]);
        count_max[i] = count_max[larger];
    }

    void build(int i, int lo, int hi, const vector<long long>& values) {
        if (lo == hi) {
            sum[i] = values[lo];
            max_val[i] = values[lo];
            second_max[i] = -BEATS_INF;
            count_max[i] = 1;
            return;
        }
        int mid = (lo + hi) / 2;
        build(2 * i, lo, mid, values);
        build(2 * i + 1, mid + 1, hi, values);
        pull(i);
    }

    void apply_chmin(int i, long long x) {
        if (x >= max_val[i]) return;
        sum[i] -= (max_val[i] - x) * count_max[i];
        max_val[i] = x;
    }

    void push(int i) {
        apply_chmin(2 * i, max_val[i]);
        apply_chmin(2 * i + 1, max_val[i]);
    }

    void update(int i, int lo, int hi, int l, int r, long long x) {
        if (r < lo or hi < l or max_val[i] <= x) return;
        if (l <= lo and hi <= r and second_max[i] < x) {
            apply_chmin(i, x);
            return;
        }
        int mid = (lo + hi) / 2;
        push(i);
        update(2 * i, lo, mid, l, r, x);
        update(2 * i + 1, mid + 1, hi, l, r, x);
        pull(i);
    }

    long long query_sum(int i, int lo, int hi, int l, int r) {
        if (r < lo or hi < l) return 0;
        if (l <= lo and hi <= r) return sum[i];
        int mid = (lo + hi) / 2;
        push(i);
        return query_sum(2 * i, lo, mid, l, r) + query_sum(2 * i + 1, mid + 1, hi, l, r);
    }

    long long query_max(int i, int lo, int hi, int l, int r) {
        if (r < lo or hi < l) return -BEATS_INF;
        if (l <= lo and hi <= r) return max_val[i];
        int mid = (lo + hi) / 2;
        push(i);
        return max(query_max(2 * i, lo, mid, l, r), query_max(2 * i + 1, mid + 1, hi, l, r));
    }

    void chmin(int l, int r, long long x) {
        update(1, 0, n - 1, l, r, x);
    }

    long long range_sum(int l, int r) {
        return query_sum(1, 0, n - 1, l, r);
    }

    long long range_max(int l, int r) {
        return query_max(1, 0, n - 1, l, r);
    }
};

/**
 * Example: chmin updates interleaved with sum and max queries.
 */
int main() {
    vector<long long> values = {5, 4, 7, 2, 9, 1};
    SegTreeBeats st(values);

    cout << st.range_sum(0, 5) << "\n";  // -> 28
    cout << st.range_max(0, 5) << "\n";  // -> 9

    st.chmin(1, 4, 5);                   // clamp indices 1..4 at 5: {5, 4, 5, 2, 5, 1}
    cout << st.range_sum(0, 5) << "\n";  // -> 22
    cout << st.range_max(0, 4) << "\n";  // -> 5

    st.chmin(0, 5, 3);                   // clamp everything at 3: {3, 3, 3, 2, 3, 1}
    cout << st.range_sum(0, 5) << "\n";  // -> 15
    cout << st.range_max(2, 5) << "\n";  // -> 3

    st.chmin(0, 5, 100);                 // no-op: all values already below 100
    cout << st.range_sum(0, 5) << "\n";  // -> 15
    return 0;
}
