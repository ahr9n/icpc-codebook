struct Point {
    long long x, y;
};

Point operator-(Point a, Point b) {
    return {a.x - b.x, a.y - b.y};
}

__int128 cross(Point a, Point b) {
    return (__int128)a.x * b.y - (__int128)a.y * b.x;
}

bool on_segment(Point a, Point b, Point p) {
    if (cross(b - a, p - a) != 0) {
        return false;
    }
    return min(a.x, b.x) <= p.x and p.x <= max(a.x, b.x) and min(a.y, b.y) <= p.y and
           p.y <= max(a.y, b.y);
}

/**
 * Classifies a point against a convex polygon in O(log n): 1 inside, 0 on its
 * boundary, and -1 outside. The polygon must have at least three vertices in
 * counter-clockwise order, with no repeated first vertex and no three
 * consecutive collinear vertices. Coordinates may use the full long long
 * range as long as coordinate differences fit in long long and their cross
 * products fit in __int128.
 */
int point_in_convex_polygon(const vector<Point>& poly, Point query) {
    int n = poly.size();
    Point origin = poly[0];
    __int128 left_side = cross(poly[1] - origin, query - origin);
    __int128 right_side = cross(poly[n - 1] - origin, query - origin);

    if (left_side < 0 or right_side > 0) {
        return -1;
    }
    if (left_side == 0) {
        return on_segment(origin, poly[1], query) ? 0 : -1;
    }
    if (right_side == 0) {
        return on_segment(origin, poly[n - 1], query) ? 0 : -1;
    }

    int low = 1;
    int high = n - 1;
    while (high - low > 1) {
        int mid = (low + high) / 2;
        if (cross(poly[mid] - origin, query - origin) >= 0) {
            low = mid;
        } else {
            high = mid;
        }
    }

    __int128 side = cross(poly[high] - poly[low], query - poly[low]);
    if (side < 0) {
        return -1;
    }
    if (side == 0) {
        return 0;
    }
    return 1;
}

/** Example: classify an interior point, an edge point, and an exterior point. */
int main() {
    vector<Point> square = {{0, 0}, {6, 0}, {6, 4}, {0, 4}};
    cout << point_in_convex_polygon(square, {2, 3}) << "\n";
    cout << point_in_convex_polygon(square, {6, 1}) << "\n";
    cout << point_in_convex_polygon(square, {7, 1}) << "\n";
    return 0;
}
// -> 1
// -> 0
// -> -1
