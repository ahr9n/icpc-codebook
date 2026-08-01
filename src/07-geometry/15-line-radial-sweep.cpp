const double EPS = 1e-12;
struct Point {
    double x, y;
};
struct HorizontalSegment {
    long long left, right, y;
};
struct VerticalSegment {
    long long x, low, high;
};

struct Fenwick {
    vector<int> tree;
    Fenwick(int n) : tree(n + 1) {}
    void add(int at, int delta) {
        for (at++; at < (int)tree.size(); at += at & -at) {
            tree[at] += delta;
        }
    }
    int prefix_sum(int at) {
        int total = 0;
        for (; at > 0; at -= at & -at) {
            total += tree[at];
        }
        return total;
    }
};

/**
 * Two reusable sweep patterns.
 *
 * count_axis_aligned_intersections is an offline x-sweep: horizontal segments
 * enter an active Fenwick tree at their left endpoint and leave after their
 * right endpoint; a vertical segment queries active y-coordinates in its range.
 * Add, query, remove event ordering counts shared endpoints. O((n + q) log n).
 *
 * max_points_in_semicircle is a radial sweep around pivot. Sorting polar angles,
 * duplicating them by +2pi, and advancing one pointer finds the most points in
 * any closed half-plane through pivot. Points must differ from pivot. O(n log n).
 */
long long count_axis_aligned_intersections(vector<HorizontalSegment> horizontal,
                                           vector<VerticalSegment> vertical) {
    struct Event {
        long long x;
        int type;
        int index;
    };
    vector<long long> ys;
    vector<Event> events;
    for (int i = 0; i < (int)horizontal.size(); i++) {
        if (horizontal[i].left > horizontal[i].right) {
            swap(horizontal[i].left, horizontal[i].right);
        }
        ys.push_back(horizontal[i].y);
        events.push_back({horizontal[i].left, 0, i});
        events.push_back({horizontal[i].right, 2, i});
    }
    for (int i = 0; i < (int)vertical.size(); i++) {
        if (vertical[i].low > vertical[i].high) {
            swap(vertical[i].low, vertical[i].high);
        }
        events.push_back({vertical[i].x, 1, i});
    }
    sort(ys.begin(), ys.end());
    ys.erase(unique(ys.begin(), ys.end()), ys.end());
    sort(events.begin(), events.end(),
         [](Event a, Event b) { return a.x < b.x or (a.x == b.x and a.type < b.type); });

    Fenwick active(ys.size());
    long long intersections = 0;
    for (auto event: events) {
        if (event.type == 1) {
            VerticalSegment segment = vertical[event.index];
            int below_low = lower_bound(ys.begin(), ys.end(), segment.low) - ys.begin();
            int at_most_high = upper_bound(ys.begin(), ys.end(), segment.high) - ys.begin();
            intersections += active.prefix_sum(at_most_high) - active.prefix_sum(below_low);
        } else {
            HorizontalSegment segment = horizontal[event.index];
            int y_index = lower_bound(ys.begin(), ys.end(), segment.y) - ys.begin();
            active.add(y_index, event.type == 0 ? 1 : -1);
        }
    }
    return intersections;
}

int max_points_in_semicircle(vector<Point> points, Point pivot) {
    int n = points.size();
    if (n == 0) {
        return 0;
    }
    const double pi = acos(-1.0);
    vector<double> angles;
    for (auto point: points) {
        angles.push_back(atan2(point.y - pivot.y, point.x - pivot.x));
    }
    sort(angles.begin(), angles.end());
    for (int i = 0; i < n; i++) {
        angles.push_back(angles[i] + 2 * pi);
    }

    int best = 0;
    int right = 0;
    for (int left = 0; left < n; left++) {
        right = max(right, left);
        while (right < left + n and angles[right] - angles[left] <= pi + EPS) {
            right++;
        }
        best = max(best, right - left);
    }
    return best;
}

/** Example: endpoint-inclusive segment crossings and a closed radial semicircle. */
int main() {
    vector<HorizontalSegment> horizontal = {{0, 4, 1}, {2, 6, 3}};
    vector<VerticalSegment> vertical = {{2, 0, 4}, {5, 0, 4}};
    cout << count_axis_aligned_intersections(horizontal, vertical) << "\n";

    vector<Point> points = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}, {2, 1}};
    cout << max_points_in_semicircle(points, {0, 0}) << "\n";
    // -> 3
    //    4
    return 0;
}
