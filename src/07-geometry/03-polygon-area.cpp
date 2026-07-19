struct Point {
    double x, y;
};
double cross(Point a, Point b) {
    return a.x * b.y - a.y * b.x;
}

/**
 * Signed polygon area via the shoelace formula (positive if ccw). O(n)
 * Take fabs for the actual area.
 */
double polygon_area(vector<Point>& poly) {
    double area = 0;
    int m = poly.size();
    for (int i = 0; i < m; i++) area += cross(poly[i], poly[(i + 1) % m]);
    return area / 2;
}
