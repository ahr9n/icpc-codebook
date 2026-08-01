// Compare the logarithmic fan search with a linear half-plane scan on random
// strictly-convex integer polygons, including vertices and points on edges.
int stress() {
    mt19937 rng(712);
    long long checks = 0;

    const long long wide = 4000000000000000000LL;
    vector<Point> wide_triangle = {{-wide, -wide}, {wide, -wide}, {0, wide}};
    if (point_in_convex_polygon(wide_triangle, {0, 0}) != 1 or
        point_in_convex_polygon(wide_triangle, {0, wide}) != 0 or
        point_in_convex_polygon(wide_triangle, {wide, wide}) != -1) {
        printf("point-in-convex FAIL wide\n");
        return 1;
    }
    checks += 3;

    auto classify_linear = [](const vector<Point>& poly, Point query) {
        bool boundary = false;
        for (int i = 0; i < (int)poly.size(); i++) {
            Point a = poly[i];
            Point b = poly[(i + 1) % poly.size()];
            __int128 side = cross(b - a, query - a);
            if (side < 0) {
                return -1;
            }
            if (side == 0 and on_segment(a, b, query)) {
                boundary = true;
            }
        }
        return boundary ? 0 : 1;
    };

    for (int rep = 0; rep < 5000; rep++) {
        int half_width = 1 + (int)(rng() % 20);
        int half_height = 1 + (int)(rng() % 20);
        int cut = (int)(rng() % min(half_width, half_height));
        vector<Point> poly = {{-half_width + cut, -half_height}, {half_width - cut, -half_height},
                              {half_width, -half_height + cut},  {half_width, half_height - cut},
                              {half_width - cut, half_height},   {-half_width + cut, half_height},
                              {-half_width, half_height - cut},  {-half_width, -half_height + cut}};

        vector<Point> strict;
        for (Point p: poly) {
            if (strict.empty() or p.x != strict.back().x or p.y != strict.back().y) {
                strict.push_back(p);
            }
        }
        poly.clear();
        for (int i = 0; i < (int)strict.size(); i++) {
            Point prev = strict[(i + strict.size() - 1) % strict.size()];
            Point cur = strict[i];
            Point next = strict[(i + 1) % strict.size()];
            if (cross(cur - prev, next - cur) != 0) {
                poly.push_back(cur);
            }
        }

        for (int trial = 0; trial < 80; trial++) {
            Point query = {(long long)(int)(rng() % 61) - 30, (long long)(int)(rng() % 61) - 30};
            int expected = classify_linear(poly, query);
            int actual = point_in_convex_polygon(poly, query);
            if (actual != expected) {
                printf("point-in-convex FAIL random rep=%d trial=%d\n", rep, trial);
                return 1;
            }
            checks++;
        }

        for (int i = 0; i < (int)poly.size(); i++) {
            Point a = poly[i];
            Point b = poly[(i + 1) % poly.size()];
            if (point_in_convex_polygon(poly, a) != 0) {
                printf("point-in-convex FAIL vertex rep=%d i=%d\n", rep, i);
                return 1;
            }
            checks++;
            if ((a.x + b.x) % 2 == 0 and (a.y + b.y) % 2 == 0) {
                Point mid = {(a.x + b.x) / 2, (a.y + b.y) / 2};
                if (point_in_convex_polygon(poly, mid) != 0) {
                    printf("point-in-convex FAIL edge rep=%d i=%d\n", rep, i);
                    return 1;
                }
                checks++;
            }
        }
    }

    printf("point-in-convex-polygon PASS %lld", checks);
    return 0;
}
