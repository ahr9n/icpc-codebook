struct Point {
    double x, y;
};
Point operator-(Point a, Point b) {
    return {a.x - b.x, a.y - b.y};
}
double cross(Point a, Point b) {
    return a.x * b.y - a.y * b.x;
}
long long dist_sq(Point a, Point b) {
    long long dx = (long long)(a.x - b.x);
    long long dy = (long long)(a.y - b.y);
    return dx * dx + dy * dy;
}

/**
 * Andrew's monotone chain convex hull: sort by (x, y), build lower + upper hull.
 * Returns hull vertices ccw with no collinear points. O(n log n)
 */
vector<Point> convex_hull(vector<Point> pts) {
    sort(pts.begin(), pts.end(),
         [](Point a, Point b) { return a.x < b.x or (a.x == b.x and a.y < b.y); });
    int m = pts.size();
    if (m < 3) return pts;

    vector<Point> hull(2 * m);
    int k = 0;
    for (int i = 0; i < m; i++) {
        while (k >= 2 and cross(hull[k - 1] - hull[k - 2], pts[i] - hull[k - 2]) <= 0) k--;
        hull[k++] = pts[i];
    }
    int lower = k + 1;
    for (int i = m - 2; i >= 0; i--) {
        while (k >= lower and cross(hull[k - 1] - hull[k - 2], pts[i] - hull[k - 2]) <= 0) k--;
        hull[k++] = pts[i];
    }
    hull.resize(k - 1);
    return hull;
}

/**
 * Rotating calipers diameter: farthest pair of points = farthest pair of hull
 * vertices. Walk one antipodal pointer forward as each hull edge rotates; the
 * pointer never resets, so the whole sweep is O(hull) after the O(n log n) hull.
 *
 * Returns the max squared Euclidean distance as an exact long long. Contract:
 * integer coordinates (squared distance stays integral). Same antipodal walk
 * yields other extents — min width (min over edges of the farthest vertex's
 * distance to the edge line) and the full set of antipodal pairs.
 */
long long convex_diameter_sq(vector<Point> pts) {
    vector<Point> hull = convex_hull(pts);
    int m = hull.size();
    if (m < 2) return 0;
    if (m == 2) return dist_sq(hull[0], hull[1]);

    long long best = 0;
    int far = 1;
    for (int i = 0; i < m; i++) {
        Point edge = hull[(i + 1) % m] - hull[i];
        while (cross(edge, hull[(far + 1) % m] - hull[i]) > cross(edge, hull[far] - hull[i]))
            far = (far + 1) % m;
        best = max(best, dist_sq(hull[i], hull[far]));
        best = max(best, dist_sq(hull[(i + 1) % m], hull[far]));
    }
    return best;
}

/**
 * Example: the farthest pair in this set is (0,0)-(4,3), squared distance 25.
 */
int main() {
    vector<Point> pts = {{0, 0}, {3, 0}, {4, 3}, {1, 4}, {0, 2}};
    cout << convex_diameter_sq(pts) << '\n';
    return 0;
}
