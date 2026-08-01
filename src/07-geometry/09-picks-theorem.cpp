struct Point {
    long long x, y;
};

/**
 * Pick's theorem toolkit for a simple polygon on the integer lattice, given its
 * vertices in order (cw or ccw). All three quantities are exact integers.
 *   twice_area: |shoelace|, so the true area is this / 2.  O(n)
 *   boundary_points: lattice points ON the edges = sum of gcd(|dx|, |dy|) per
 *     edge, since an edge crosses exactly gcd interior-of-segment steps.  O(n)
 *   interior_points: strictly-inside lattice points, from Pick's identity
 *     A = I + B/2 - 1  =>  I = (2A - B + 2) / 2.  O(n)
 * We keep twice the area to stay in integers: 2A - B + 2 is always even.
 */
long long twice_area(vector<Point>& poly) {
    long long total = 0;
    int n = poly.size();
    for (int i = 0; i < n; i++) {
        Point a = poly[i];
        Point b = poly[(i + 1) % n];
        total += a.x * b.y - a.y * b.x;
    }
    return llabs(total);
}

long long boundary_points(vector<Point>& poly) {
    long long total = 0;
    int n = poly.size();
    for (int i = 0; i < n; i++) {
        Point a = poly[i];
        Point b = poly[(i + 1) % n];
        total += gcd(llabs(b.x - a.x), llabs(b.y - a.y));
    }
    return total;
}

long long interior_points(vector<Point>& poly) {
    return (twice_area(poly) - boundary_points(poly) + 2) / 2;
}

/**
 * Example: the triangle (0,0),(4,0),(0,4) has area 8, 12 boundary points, and
 * by Pick's theorem 3 interior points.
 */
int main() {
    vector<Point> tri = {{0, 0}, {4, 0}, {0, 4}};
    cout << twice_area(tri) / 2 << " " << boundary_points(tri) << " " << interior_points(tri)
         << "\n";  // -> 8 12 3
    return 0;
}
