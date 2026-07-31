struct Point {
    long long x, y;
};

double dist(const Point& a, const Point& b) {
    double dx = a.x - b.x, dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
}

/**
 * Closest pair of points by a left-to-right line sweep. Sort by x and keep an
 * active set ordered by y holding only the points within the current best
 * distance in x. For each new point, drop the ones now too far left, then scan
 * just the y-band [y - best, y + best] -- geometry caps that at O(1) candidates
 * -- and shrink best. A multiset keeps coincident points from colliding.
 * O(n log n).
 */
double closest_pair(vector<Point> pts) {
    sort(pts.begin(), pts.end(), [](const Point& a, const Point& b) { return a.x < b.x; });
    multiset<pair<long long, long long>> active;
    double best = 1e18;
    int left = 0;
    for (int i = 0; i < (int)pts.size(); i++) {
        while (left < i and pts[i].x - pts[left].x > best) {
            active.erase(active.find({pts[left].y, pts[left].x}));
            left++;
        }
        long long band = (long long)best + 1;
        auto lo = active.lower_bound({pts[i].y - band, LLONG_MIN});
        auto hi = active.upper_bound({pts[i].y + band, LLONG_MAX});
        for (auto it = lo; it != hi; ++it) best = min(best, dist(pts[i], {it->second, it->first}));
        active.insert({pts[i].y, pts[i].x});
    }
    return best;
}

/**
 * Example: among (0,0), (3,4), (7,7), (1,1) the closest pair is (0,0)-(1,1),
 * distance sqrt(2) ~ 1.41.
 */
int main() {
    vector<Point> pts = {{0, 0}, {3, 4}, {7, 7}, {1, 1}};
    cout << fixed << setprecision(2) << closest_pair(pts) << "\n";  // -> 1.41
    return 0;
}
