const double EPS = 1e-9;
struct Point {
    double x, y;
};
struct Circle {
    Point center;
    double r;
};

double dist(Point a, Point b) {
    double dx = a.x - b.x, dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
}

/**
 * Circumcircle of three points: the unique circle passing through a, b, c,
 * found as the intersection of two perpendicular bisectors (solved in closed
 * form by a determinant). Returns false and leaves out untouched when the
 * points are collinear -- no finite circle exists. O(1).
 */
bool circumcircle(Point a, Point b, Point c, Circle& out) {
    double d = 2 * (a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y));
    if (fabs(d) < EPS) return false;
    double a2 = a.x * a.x + a.y * a.y;
    double b2 = b.x * b.x + b.y * b.y;
    double c2 = c.x * c.x + c.y * c.y;
    double ux = (a2 * (b.y - c.y) + b2 * (c.y - a.y) + c2 * (a.y - b.y)) / d;
    double uy = (a2 * (c.x - b.x) + b2 * (a.x - c.x) + c2 * (b.x - a.x)) / d;
    out.center = {ux, uy};
    out.r = dist(out.center, a);
    return true;
}

bool in_circle(Circle c, Point p) {
    return dist(c.center, p) <= c.r + EPS;
}

Circle circle_from_2(Point a, Point b) {
    Point center = {(a.x + b.x) / 2, (a.y + b.y) / 2};
    return {center, dist(a, b) / 2};
}

Circle circle_from_3(Point a, Point b, Point c) {
    Circle out{};
    circumcircle(a, b, c, out);
    return out;
}

/**
 * Minimum enclosing circle (smallest circle covering every point) via Welzl's
 * randomized incremental algorithm. Grow the answer point by point: whenever a
 * point falls outside the current circle it must lie on the boundary of the new
 * one, which pins down one, then two, then three boundary points -- the circle
 * is fixed by two points as a diameter or by three via the circumcircle. The
 * random shuffle makes the triple-nested rebuilds rare, giving expected O(n)
 * (worst case O(n^2)). Contract: pts non-empty; a single point yields r = 0.
 */
Circle min_enclosing_circle(vector<Point> pts) {
    const unsigned int shuffle_seed = 20240218u;  // fixed so the demo is reproducible
    mt19937 rng(shuffle_seed);
    shuffle(pts.begin(), pts.end(), rng);

    int n = pts.size();
    Circle c = {pts[0], 0};
    for (int i = 1; i < n; i++) {
        if (in_circle(c, pts[i])) continue;
        c = {pts[i], 0};
        for (int j = 0; j < i; j++) {
            if (in_circle(c, pts[j])) continue;
            c = circle_from_2(pts[i], pts[j]);
            for (int k = 0; k < j; k++) {
                if (in_circle(c, pts[k])) continue;
                c = circle_from_3(pts[i], pts[j], pts[k]);
            }
        }
    }
    return c;
}

/**
 * Example: the circumcircle of the right triangle (0,0),(4,0),(0,3) is centered
 * at the hypotenuse midpoint (2, 1.5) with radius 2.5. The minimum enclosing
 * circle of a 5-point set is pinned by its two farthest-apart extreme points.
 */
int main() {
    Circle cc{};
    circumcircle({0, 0}, {4, 0}, {0, 3}, cc);
    cout << fixed << setprecision(2);
    cout << cc.center.x << " " << cc.center.y << " " << cc.r << "\n";  // -> 2.00 1.50 2.50

    vector<Point> pts = {{0, 0}, {10, 0}, {5, 5}, {3, 2}, {7, 1}};
    Circle mec = min_enclosing_circle(pts);
    cout << mec.center.x << " " << mec.center.y << " " << mec.r << "\n";  // -> 5.00 0.00 5.00
    return 0;
}
