struct Point {
    double x, y;
};
Point operator-(Point a, Point b) {
    return {a.x - b.x, a.y - b.y};
}
double cross(Point a, Point b) {
    return a.x * b.y - a.y * b.x;
}

/**
 * Andrew's monotone chain convex hull: sort by (x, y), build lower + upper hull.
 * Returns hull vertices ccw. O(n log n)
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
 * Example: area and hull of a unit square (hull has 4 vertices, area 1).
 */
int main() {
    vector<Point> square = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    cout << polygon_area(square) << '\n';
    cout << convex_hull(square).size() << '\n';
    cout << segments_intersect({0, 0}, {1, 1}, {0, 1}, {1, 0}) << '\n';
    return 0;
}
