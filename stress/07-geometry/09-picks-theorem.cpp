// Stress: Pick's-theorem formulas vs a brute-force lattice scan of the bounding
// box, over many random axis-aligned rectangles and lattice triangles (both
// families are simple polygons whose interior / boundary we can classify point
// by point). Also checks the Pick identity area == I + B/2 - 1 and thin
// degenerate shapes.

// Twice the signed area of triangle (a, b, c); sign tells the halfplane side.
static long long cross2(Point a, Point b, Point c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

static bool on_segment(Point a, Point b, Point p) {
    if (cross2(a, b, p) != 0) return false;
    return min(a.x, b.x) <= p.x and p.x <= max(a.x, b.x) and min(a.y, b.y) <= p.y and
           p.y <= max(a.y, b.y);
}

static bool on_boundary(vector<Point>& poly, Point p) {
    int n = poly.size();
    for (int i = 0; i < n; i++)
        if (on_segment(poly[i], poly[(i + 1) % n], p)) return true;
    return false;
}

// Ray casting: strictly-inside test (points on the boundary handled separately).
static bool strictly_inside(vector<Point>& poly, Point p) {
    int n = poly.size();
    bool inside = false;
    for (int i = 0, j = n - 1; i < n; j = i++) {
        Point a = poly[i];
        Point b = poly[j];
        bool straddles = (a.y > p.y) != (b.y > p.y);
        if (not straddles) continue;
        double x_cross = (double)(b.x - a.x) * (p.y - a.y) / (double)(b.y - a.y) + a.x;
        if (p.x < x_cross) inside = not inside;
    }
    return inside;
}

static bool brute(vector<Point>& poly, long long& interior, long long& boundary) {
    long long min_x = poly[0].x, max_x = poly[0].x;
    long long min_y = poly[0].y, max_y = poly[0].y;
    for (auto p: poly) {
        min_x = min(min_x, p.x);
        max_x = max(max_x, p.x);
        min_y = min(min_y, p.y);
        max_y = max(max_y, p.y);
    }
    interior = 0;
    boundary = 0;
    for (long long x = min_x; x <= max_x; x++)
        for (long long y = min_y; y <= max_y; y++) {
            Point p = {x, y};
            if (on_boundary(poly, p))
                boundary++;
            else if (strictly_inside(poly, p))
                interior++;
        }
    return true;
}

int stress() {
    mt19937 rng(7);
    long long checks = 0;

    auto verify = [&](vector<Point> poly) -> bool {
        long long bi = 0, bb = 0;
        brute(poly, bi, bb);
        long long area2 = twice_area(poly);
        long long b = boundary_points(poly);
        long long interior = interior_points(poly);
        if (b != bb or interior != bi) {
            printf("FAIL B=%lld(exp %lld) I=%lld(exp %lld)\n", b, bb, interior, bi);
            return false;
        }
        // Pick's identity: 2A == 2I + B - 2.
        if (area2 != 2 * interior + b - 2) {
            printf("FAIL identity 2A=%lld I=%lld B=%lld\n", area2, interior, b);
            return false;
        }
        checks++;
        return true;
    };

    auto rand_coord = [&]() { return (long long)(rng() % 21) - 10; };

    for (int rep = 0; rep < 3000; rep++) {
        long long x1 = rand_coord(), x2 = rand_coord();
        long long y1 = rand_coord(), y2 = rand_coord();
        if (x1 == x2 or y1 == y2) continue;
        if (x1 > x2) swap(x1, x2);
        if (y1 > y2) swap(y1, y2);
        vector<Point> rect = {{x1, y1}, {x2, y1}, {x2, y2}, {x1, y2}};
        if (not verify(rect)) return 1;
    }

    for (int rep = 0; rep < 5000; rep++) {
        Point a = {rand_coord(), rand_coord()};
        Point b = {rand_coord(), rand_coord()};
        Point c = {rand_coord(), rand_coord()};
        if (cross2(a, b, c) == 0) continue;
        if (cross2(a, b, c) < 0) swap(b, c);
        vector<Point> tri = {a, b, c};
        if (not verify(tri)) return 1;
    }

    vector<Point> thin = {{0, 0}, {6, 0}, {6, 1}, {0, 1}};
    if (not verify(thin)) return 1;
    vector<Point> tri = {{0, 0}, {4, 0}, {0, 4}};
    if (not verify(tri)) return 1;
    vector<Point> rect = {{-2, -3}, {5, -3}, {5, 2}, {-2, 2}};
    if (not verify(rect)) return 1;

    printf("picks PASS %lld", checks);
    return 0;
}
