static bool near(double a, double b, double tolerance = 1e-7) {
    return fabs(a - b) <= tolerance;
}

static bool on_line(Point p, Point a, Point b) {
    Point dir = b - a;
    return fabs(dir.x * (p.y - a.y) - dir.y * (p.x - a.x)) <= 1e-7 * max(1.0, norm(dir));
}

int stress() {
    mt19937 rng(701);
    long long checks = 0;

    auto random_coord = [&]() { return (double)((int)(rng() % 41) - 20); };
    for (int rep = 0; rep < 10000; rep++) {
        Point center = {random_coord(), random_coord()};
        double radius = rng() % 21;
        Point a = {random_coord(), random_coord()};
        Point b = {random_coord(), random_coord()};
        if (near(norm(b - a), 0)) {
            rep--;
            continue;
        }

        vector<Point> hits = circle_line_intersection(center, radius, a, b);
        Point dir = b - a;
        long double qa = (long double)dot(dir, dir);
        long double qb =
            2 * ((long double)(a.x - center.x) * dir.x + (long double)(a.y - center.y) * dir.y);
        long double qc = (long double)(a.x - center.x) * (a.x - center.x) +
                         (long double)(a.y - center.y) * (a.y - center.y) - radius * radius;
        long double discriminant = qb * qb - 4 * qa * qc;
        int expected = discriminant < -1e-12 ? 0 : (fabsl(discriminant) <= 1e-12 ? 1 : 2);
        if ((int)hits.size() != expected) {
            printf("FAIL line count got=%zu expected=%d\n", hits.size(), expected);
            return 1;
        }
        for (auto p: hits)
            if (not on_line(p, a, b) or not near(norm(p - center), radius, 2e-7)) {
                printf("FAIL line residual\n");
                return 1;
            }
        checks++;
    }

    for (int rep = 0; rep < 10000; rep++) {
        Point c1 = {random_coord(), random_coord()};
        Point c2 = {random_coord(), random_coord()};
        double r1 = rng() % 21;
        double r2 = rng() % 21;
        double d = norm(c2 - c1);
        vector<Point> hits = circle_circle_intersection(c1, r1, c2, r2);

        int expected;
        if (d == 0 and r1 == r2)
            expected = r1 == 0 ? 1 : -1;
        else if (d > r1 + r2 or d < fabs(r1 - r2) or d == 0)
            expected = 0;
        else if (near(d, r1 + r2) or near(d, fabs(r1 - r2)))
            expected = 1;
        else
            expected = 2;

        if (expected == -1) {
            if (not concentric_infinite(c1, r1, c2, r2) or not hits.empty()) {
                printf("FAIL coincident circles\n");
                return 1;
            }
        } else if ((int)hits.size() != expected) {
            printf("FAIL circle count got=%zu expected=%d\n", hits.size(), expected);
            return 1;
        }
        for (auto p: hits)
            if (not near(norm(p - c1), r1, 3e-7) or not near(norm(p - c2), r2, 3e-7)) {
                printf("FAIL circle residual\n");
                return 1;
            }
        checks++;
    }

    vector<Point> point_hit = circle_circle_intersection({3, -4}, 0, {3, -4}, 0);
    if (point_hit.size() != 1 or not near(point_hit[0].x, 3) or not near(point_hit[0].y, -4))
        return 1;
    printf("line-circle-intersection PASS %lld", checks + 1);
    return 0;
}
