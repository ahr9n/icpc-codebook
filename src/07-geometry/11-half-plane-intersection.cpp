const double EPS = 1e-9;
struct Point {
    double x, y;
};
Point operator+(Point a, Point b) {
    return {a.x + b.x, a.y + b.y};
}
Point operator-(Point a, Point b) {
    return {a.x - b.x, a.y - b.y};
}
Point operator*(Point a, double t) {
    return {a.x * t, a.y * t};
}
double cross(Point a, Point b) {
    return a.x * b.y - a.y * b.x;
}
double dot(Point a, Point b) {
    return a.x * b.x + a.y * b.y;
}
double norm(Point a) {
    return sqrt(dot(a, a));
}

/**
 * Half-plane intersection via angular sort + deque. A half-plane is the region on
 * the LEFT of the directed ray p -> p + dir; the intersection of a set of them is
 * a convex region, returned as its polygon in ccw order. O(n log n), dominated by
 * the angle sort: with the deque kept in increasing angle, each plane is pushed
 * and popped at most once, and a plane made redundant by its neighbours' meeting
 * point falls off an end.
 *
 * Directions are stored as unit vectors, so cross/dot against them read as a
 * signed distance / an angle cosine and one EPS threshold serves everywhere.
 *
 * Contract: the caller must supply half-planes that bound the region (e.g. a
 * large axis-aligned box); an unbounded feasible region yields a meaningless
 * polygon. An empty feasible region returns an empty vector.
 */
struct Halfplane {
    Point p, dir;
    double angle;
};
Halfplane make_halfplane(Point a, Point b) {
    Point dir = b - a;
    dir = dir * (1.0 / norm(dir));
    return {a, dir, atan2(dir.y, dir.x)};
}
bool on_outer_side(Halfplane h, Point r) {
    return cross(h.dir, r - h.p) < -EPS;
}
Point line_intersection(Halfplane s, Halfplane t) {
    double along = cross(t.p - s.p, t.dir) / cross(s.dir, t.dir);
    return s.p + s.dir * along;
}

vector<Point> halfplane_intersection(vector<Halfplane> planes) {
    sort(planes.begin(), planes.end(),
         [](const Halfplane& a, const Halfplane& b) { return a.angle < b.angle; });

    deque<Halfplane> dq;
    for (auto h: planes) {
        while (dq.size() > 1 and
               on_outer_side(h, line_intersection(dq[dq.size() - 1], dq[dq.size() - 2])))
            dq.pop_back();
        while (dq.size() > 1 and on_outer_side(h, line_intersection(dq[0], dq[1])))
            dq.pop_front();

        // Same-angle planes are adjacent after the sort: keep only the tighter of
        // two parallels, and bail out when they face each other (empty region).
        if (not dq.empty() and fabs(cross(h.dir, dq.back().dir)) < EPS) {
            if (dot(h.dir, dq.back().dir) < 0) return {};
            if (on_outer_side(h, dq.back().p))
                dq.pop_back();
            else
                continue;
        }
        dq.push_back(h);
    }

    while (dq.size() > 2 and
           on_outer_side(dq[0], line_intersection(dq[dq.size() - 1], dq[dq.size() - 2])))
        dq.pop_back();
    while (dq.size() > 2 and on_outer_side(dq.back(), line_intersection(dq[0], dq[1])))
        dq.pop_front();
    if (dq.size() < 3) return {};

    int m = dq.size();
    vector<Point> poly(m);
    for (int i = 0; i < m; i++) poly[i] = line_intersection(dq[i], dq[(i + 1) % m]);
    return poly;
}

/**
 * Example: four half-planes cutting out the square [-1, 1] x [-1, 1]; report the
 * vertex count and the enclosed area.
 */
int main() {
    vector<Halfplane> square = {
        make_halfplane({-1, -1}, {1, -1}),  // feasible side is y >= -1
        make_halfplane({1, -1}, {1, 1}),    // feasible side is x <= 1
        make_halfplane({1, 1}, {-1, 1}),    // feasible side is y <= 1
        make_halfplane({-1, 1}, {-1, -1}),  // feasible side is x >= -1
    };
    vector<Point> region = halfplane_intersection(square);

    double area = 0;
    int m = region.size();
    for (int i = 0; i < m; i++) area += cross(region[i], region[(i + 1) % m]);
    area = fabs(area) / 2;

    cout << fixed << setprecision(1) << m << " " << area << "\n";  // -> 4 4.0
    return 0;
}
