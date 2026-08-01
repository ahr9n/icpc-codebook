/**
 * Li Chao tree: maintain a set of lines y = m*x + b, answer min over all lines
 * at a query x. add_line and query are both O(log C) where C is the domain size.
 * Domain is the fixed integer range [x_lo, x_hi] fixed at construction; every
 * query x must satisfy x_lo <= x <= x_hi. Memory is O(C) (a 4*C node segment
 * tree over the x-coordinates), so this fits a small/compressed x-domain.
 * This is the MIN version: flip both `<` comparisons to `>` and seed the empty
 * line with b = -LINF to answer maxima instead. It also extends to segments
 * (a line active only on [xl, xr]): descend to the O(log C) canonical nodes of
 * [xl, xr] and run this same add_line at each, giving O(log^2 C) per segment.
 */
struct Line {
    long long slope, intercept;

    long long eval(long long x) const {
        return slope * x + intercept;
    }
};

struct LiChao {
    long long x_lo, x_hi;
    vector<Line> lines;

    LiChao(long long x_lo, long long x_hi) : x_lo(x_lo), x_hi(x_hi) {
        long long size = x_hi - x_lo + 1;
        lines.assign(4 * size, Line{0, LINF});
    }

    void add_line(Line candidate) {
        add_line(1, x_lo, x_hi, candidate);
    }

    long long query(long long x) {
        return query(1, x_lo, x_hi, x);
    }

    void add_line(int node, long long lo, long long hi, Line candidate) {
        long long mid = lo + (hi - lo) / 2;
        bool better_at_lo = candidate.eval(lo) < lines[node].eval(lo);
        bool better_at_mid = candidate.eval(mid) < lines[node].eval(mid);

        if (better_at_mid) swap(lines[node], candidate);

        if (lo == hi) return;

        if (better_at_lo != better_at_mid)
            add_line(2 * node, lo, mid, candidate);
        else
            add_line(2 * node + 1, mid + 1, hi, candidate);
    }

    long long query(int node, long long lo, long long hi, long long x) {
        long long here = lines[node].eval(x);
        if (lo == hi) return here;

        long long mid = lo + (hi - lo) / 2;
        if (x <= mid) return min(here, query(2 * node, lo, mid, x));
        return min(here, query(2 * node + 1, mid + 1, hi, x));
    }
};

/**
 * Example: insert four lines over the domain [-10, 10] and query the lower
 * envelope at a few points.
 */
int main() {
    LiChao tree(-10, 10);
    tree.add_line(Line{2, 3});    // y = 2x + 3
    tree.add_line(Line{-1, 10});  // y = -x + 10
    tree.add_line(Line{0, 1});    // y = 1
    tree.add_line(Line{1, -4});   // y = x - 4

    cout << tree.query(-5) << "\n";  // -> -9
    cout << tree.query(0) << "\n";   // -> -4
    cout << tree.query(3) << "\n";   // -> -1
    cout << tree.query(7) << "\n";   // -> 1
    return 0;
}
