struct Point {
    double x, y;
};

/**
 * Point in polygon by ray casting: true if p is inside (boundary undefined). O(n)
 */
bool point_in_polygon(vector<Point>& poly, Point p) {
    int m = poly.size();
    bool inside = false;
    for (int i = 0, j = m - 1; i < m; j = i++) {
        Point a = poly[i], b = poly[j];
        if ((a.y > p.y) != (b.y > p.y) and p.x < (b.x - a.x) * (p.y - a.y) / (b.y - a.y) + a.x)
            inside = not inside;
    }
    return inside;
}
