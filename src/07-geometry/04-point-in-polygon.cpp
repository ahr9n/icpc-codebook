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

/**
 * Example: test one point inside and one outside the square.
 */
int main() {
    vector<Point> square = {{0, 0}, {4, 0}, {4, 4}, {0, 4}};
    Point inside = {2, 2}, outside = {5, 5};
    cout << point_in_polygon(square, inside) << point_in_polygon(square, outside) << "\n";  // -> 10
    return 0;
}
