const double EPS = 1e-9;
struct Point {
    double x, y;
};

/**
 * Segment intersection test: do segments ab and cd share any point? O(1)
 * Straddle test plus collinear-overlap fallback.
 */
bool on_segment(Point a, Point b, Point p) {
    return orient(a, b, p) == 0 and min(a.x, b.x) - EPS <= p.x and p.x <= max(a.x, b.x) + EPS and
           min(a.y, b.y) - EPS <= p.y and p.y <= max(a.y, b.y) + EPS;
}

bool segments_intersect(Point a, Point b, Point c, Point d) {
    int o1 = orient(a, b, c), o2 = orient(a, b, d);
    int o3 = orient(c, d, a), o4 = orient(c, d, b);
    if (o1 != o2 and o3 != o4) return true;
    return on_segment(a, b, c) or on_segment(a, b, d) or on_segment(c, d, a) or on_segment(c, d, b);
}
