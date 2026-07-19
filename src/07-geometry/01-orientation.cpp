const double EPS = 1e-9;
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
 * Orientation of ordered triple (a, b, c): +1 ccw, -1 cw, 0 collinear.
 */
int orient(Point a, Point b, Point c) {
    double v = cross(b - a, c - a);
    if (v > EPS) return 1;
    if (v < -EPS) return -1;
    return 0;
}
