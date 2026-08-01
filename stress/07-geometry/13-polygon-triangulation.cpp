static double area_twice(const vector<Point>& polygon) {
    double area = 0;
    for (int i = 0; i < (int)polygon.size(); i++)
        area += cross({0, 0}, polygon[i], polygon[(i + 1) % polygon.size()]);
    return fabs(area);
}

static bool verify_triangulation(const vector<Point>& polygon) {
    vector<array<int, 3>> triangles = triangulate_polygon(polygon);
    if (triangles.size() + 2 != polygon.size()) return false;
    double triangle_area_twice = 0;
    for (auto [a, b, c]: triangles) {
        if (a < 0 or b < 0 or c < 0 or a >= (int)polygon.size() or b >= (int)polygon.size() or
            c >= (int)polygon.size())
            return false;
        double area = cross(polygon[a], polygon[b], polygon[c]);
        if (area <= EPS) return false;
        triangle_area_twice += area;
    }
    return fabs(triangle_area_twice - area_twice(polygon)) <= 1e-7 * max(1.0, area_twice(polygon));
}

int stress() {
    mt19937 rng(713);
    long long checks = 0;
    const double pi = acos(-1.0);
    for (int rep = 0; rep < 7000; rep++) {
        int n = 3 + rng() % 15;
        vector<Point> polygon;
        double phase = (rng() % 10000) / 10000.0;
        for (int i = 0; i < n; i++) {
            double angle = phase + 2 * pi * i / n;
            double radius = 10 + rng() % 41;
            polygon.push_back({radius * cos(angle), radius * sin(angle)});
        }
        if (rng() % 2) reverse(polygon.begin(), polygon.end());
        if (not verify_triangulation(polygon)) {
            printf("FAIL n=%d\n", n);
            return 1;
        }
        checks++;
    }
    printf("polygon-triangulation PASS %lld", checks);
    return 0;
}
