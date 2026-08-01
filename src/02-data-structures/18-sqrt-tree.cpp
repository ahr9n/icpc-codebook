/**
 * Sqrt tree: O(1) static range query for any associative op (here min; sum, gcd,
 * matrix product all work). Build O(n log log n), memory O(n log log n). No updates.
 *
 * Idea: recursively split into sqrt-sized blocks. A layer whose segment spans
 * 2^s indices cuts each segment into 2^(s/2) blocks of 2^((s+1)/2). For that
 * layer store, per position, the op over its block as a prefix and as a suffix,
 * plus "between[i..j]" = op over whole blocks i..j of a segment. A query on
 * [l, r] is served by the shallowest layer where l and r sit in the same segment
 * but different blocks (found from the top differing bit of l xor r), combining
 * suffix[l] + between(inner blocks) + prefix[r] in O(1). Segment size shrinks by
 * a square root each layer, so there are only O(log log n) layers.
 */
struct SqrtTree {
    int n, lg;
    vector<long long> v;
    vector<int> seg_log;   // seg_log[layer] = log2 of that layer's segment length
    vector<int> on_layer;  // on_layer[bit] = layer to use when l xor r tops out at 'bit'
    vector<vector<long long>> prefix, suffix, between;

    long long op(long long a, long long b) {
        return min(a, b);
    }

    SqrtTree(const vector<long long>& a) : v(a) {
        n = v.size();
        lg = 0;
        while ((1 << lg) < n) lg++;  // smallest lg with 2^lg >= n; top segment covers all

        on_layer.assign(lg + 1, 0);
        for (int len = lg; len > 1; len = (len + 1) / 2) {
            int layer = seg_log.size();
            seg_log.push_back(len);
            int block_log = (len + 1) / 2;
            for (int bit = block_log; bit < len; bit++) on_layer[bit] = layer;
        }

        int layers = seg_log.size();
        int cap = 1 << lg;
        prefix.assign(layers, vector<long long>(cap));
        suffix.assign(layers, vector<long long>(cap));
        between.assign(layers, vector<long long>(cap));
        for (int layer = 0; layer < layers; layer++) build_layer(layer);
    }

    void build_layer(int layer) {
        int len = seg_log[layer];
        int block_log = (len + 1) / 2;
        int block_size = 1 << block_log;
        int block_cnt_log = len / 2;  // 2^block_cnt_log blocks per segment

        for (int start = 0; start < n; start += block_size) {
            int stop = min(start + block_size, n);
            prefix[layer][start] = v[start];
            for (int i = start + 1; i < stop; i++)
                prefix[layer][i] = op(prefix[layer][i - 1], v[i]);
            suffix[layer][stop - 1] = v[stop - 1];
            for (int i = stop - 2; i >= start; i--)
                suffix[layer][i] = op(v[i], suffix[layer][i + 1]);
        }

        int seg_size = 1 << len;
        for (int seg = 0; seg < n; seg += seg_size) {
            int seg_end = min(seg + seg_size, n);  // blocks past here belong to later segments
            for (int i = 0; seg + (i << block_log) < seg_end; i++) {
                long long acc = 0;
                for (int j = i; seg + (j << block_log) < seg_end; j++) {
                    long long block_val = suffix[layer][seg + (j << block_log)];
                    acc = (i == j) ? block_val : op(acc, block_val);
                    between[layer][seg + (i << block_cnt_log) + j] = acc;
                }
            }
        }
    }

    long long query(int l, int r) {
        if (l == r) return v[l];
        if (l + 1 == r) return op(v[l], v[r]);

        int layer = on_layer[__lg(l ^ r)];
        int len = seg_log[layer];
        int block_log = (len + 1) / 2;
        int block_cnt_log = len / 2;
        int seg = (l >> len) << len;
        int block_l = (l - seg) >> block_log;
        int block_r = (r - seg) >> block_log;

        long long ans = suffix[layer][l];
        if (block_r - block_l > 1) {
            int lo = block_l + 1, hi = block_r - 1;
            ans = op(ans, between[layer][seg + (lo << block_cnt_log) + hi]);
        }
        return op(ans, prefix[layer][r]);
    }
};

/**
 * Example: build over an array, then answer four O(1) inclusive range-min queries.
 */
int main() {
    vector<long long> a = {5, 2, 8, 1, 9, 3, 7, 4};
    SqrtTree st(a);
    cout << st.query(0, 2) << "\n";  // -> 2
    cout << st.query(2, 5) << "\n";  // -> 1
    cout << st.query(4, 7) << "\n";  // -> 3
    cout << st.query(6, 7) << "\n";  // -> 4
    return 0;
}
