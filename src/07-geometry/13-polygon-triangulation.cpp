const double EPS = 1e-9;
struct Point {
    double x, y;
};
double cross(Point a, Point b, Point c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

/**
 * Ear-clipping triangulation of a simple polygon. Vertices may be cw or ccw;
 * returned triples are original vertex indices in ccw order and their interiors
 * are disjoint. An ear is a convex corner whose triangle contains no remaining
 * vertex, so removing it preserves the rest of the polygon.
 *
 * Contract: at least three vertices, no self-intersections, repeated vertices,
 * or three consecutive collinear vertices. Returns empty if these assumptions
 * prevent finding an ear. O(n^3) with the deliberately simple vertex scan.
 */
vector<array<int, 3>> triangulate_polygon(const vector<Point>& polygon) {
    int n = polygon.size();
    if (n < 3) {
        return {};
    }

    double signed_area_twice = 0;
    for (int i = 0; i < n; i++)
        signed_area_twice +=
            polygon[i].x * polygon[(i + 1) % n].y - polygon[i].y * polygon[(i + 1) % n].x;
    if (fabs(signed_area_twice) < EPS) {
        return {};
    }

    vector<int> remaining(n);
    iota(remaining.begin(), remaining.end(), 0);
    if (signed_area_twice < 0) {
        reverse(remaining.begin(), remaining.end());
    }

    vector<array<int, 3>> triangles;
    while (remaining.size() > 3) {
        bool clipped = false;
        int m = remaining.size();
        for (int i = 0; i < m and not clipped; i++) {
            int prev = remaining[(i + m - 1) % m];
            int cur = remaining[i];
            int next = remaining[(i + 1) % m];
            if (cross(polygon[prev], polygon[cur], polygon[next]) <= EPS) {
                continue;
            }

            bool contains_vertex = false;
            for (auto vertex: remaining) {
                if (vertex == prev or vertex == cur or vertex == next) {
                    continue;
                }
                double side1 = cross(polygon[prev], polygon[cur], polygon[vertex]);
                double side2 = cross(polygon[cur], polygon[next], polygon[vertex]);
                double side3 = cross(polygon[next], polygon[prev], polygon[vertex]);
                if (side1 >= -EPS and side2 >= -EPS and side3 >= -EPS) {
                    contains_vertex = true;
                    break;
                }
            }
            if (contains_vertex) {
                continue;
            }

            triangles.push_back({prev, cur, next});
            remaining.erase(remaining.begin() + i);
            clipped = true;
        }
        if (not clipped) {
            return {};
        }
    }
    triangles.push_back({remaining[0], remaining[1], remaining[2]});
    return triangles;
}

/** Example: triangulate a five-vertex concave polygon into three ccw ears. */
int main() {
    vector<Point> polygon = {{0, 0}, {4, 0}, {4, 4}, {2, 2}, {0, 4}};
    vector<array<int, 3>> triangles = triangulate_polygon(polygon);
    cout << triangles.size() << "\n";
    for (auto [a, b, c]: triangles) {
        cout << a << " " << b << " " << c << "\n";
    }
    // -> 3
    //    1 2 3
    //    0 1 3
    //    0 3 4
    return 0;
}
