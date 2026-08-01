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
    if (m < 3) {
        return pts;
    }

    vector<Point> hull(2 * m);
    int k = 0;
    for (int i = 0; i < m; i++) {
        while (k >= 2 and cross(hull[k - 1] - hull[k - 2], pts[i] - hull[k - 2]) <= 0) {
            k--;
        }
        hull[k++] = pts[i];
    }
    int lower = k + 1;
    for (int i = m - 2; i >= 0; i--) {
        while (k >= lower and cross(hull[k - 1] - hull[k - 2], pts[i] - hull[k - 2]) <= 0) {
            k--;
        }
        hull[k++] = pts[i];
    }
    hull.resize(k - 1);
    return hull;
}

/**
 * Rotating calipers diameter and minimum width. Walk one antipodal pointer as
 * each hull edge rotates; it never resets, so each sweep is O(hull) after the
 * O(n log n) hull. Diameter checks antipodal vertex pairs; width minimizes the
 * farthest vertex's perpendicular distance from each hull edge.
 *
 * Returns the max squared Euclidean distance as an exact long long. Contract:
 * integer coordinates whose squared distance fits long long. Width is zero for
 * fewer than three hull vertices.
 */
long long convex_diameter_sq(vector<Point> pts) {
    vector<Point> hull = convex_hull(pts);
    int m = hull.size();
    if (m < 2) {
        return 0;
    }
    if (m == 2) {
        return dist_sq(hull[0], hull[1]);
    }

    long long best = 0;
    int far = 1;
    for (int i = 0; i < m; i++) {
        Point edge = hull[(i + 1) % m] - hull[i];
        while (cross(edge, hull[(far + 1) % m] - hull[i]) > cross(edge, hull[far] - hull[i])) {
            far = (far + 1) % m;
        }
        best = max(best, dist_sq(hull[i], hull[far]));
        best = max(best, dist_sq(hull[(i + 1) % m], hull[far]));
    }
    return best;
}

double convex_min_width(vector<Point> pts) {
    vector<Point> hull = convex_hull(pts);
    int m = hull.size();
    if (m < 3) {
        return 0;
    }

    double best = 1e100;
    int far = 1;
    for (int i = 0; i < m; i++) {
        Point edge = hull[(i + 1) % m] - hull[i];
        while (cross(edge, hull[(far + 1) % m] - hull[i]) > cross(edge, hull[far] - hull[i])) {
            far = (far + 1) % m;
        }
        best = min(best, cross(edge, hull[far] - hull[i]) / hypot(edge.x, edge.y));
    }
    return best;
}

/**
 * Example: report the squared diameter and minimum width of a convex pentagon.
 */
int main() {
    vector<Point> pts = {{0, 0}, {3, 0}, {4, 3}, {1, 4}, {0, 2}};
    cout << convex_diameter_sq(pts) << '\n';
    cout << fixed << setprecision(3) << convex_min_width(pts) << '\n';
    // -> 25
    //    3.479
    return 0;
}
