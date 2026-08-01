static vector<Point> clip_polygon(vector<Point> poly, Halfplane h) {
    vector<Point> clipped;
    for (int i = 0; i < (int)poly.size(); i++) {
        Point a = poly[i];
        Point b = poly[(i + 1) % poly.size()];
        bool a_inside = not on_outer_side(h, a);
        bool b_inside = not on_outer_side(h, b);
        if (a_inside) clipped.push_back(a);
        if (a_inside != b_inside) {
            Point edge = b - a;
            double t = cross(h.dir, h.p - a) / cross(h.dir, edge);
            clipped.push_back(a + edge * t);
        }
    }
    return clipped;
}

static double polygon_area(vector<Point> poly) {
    double area = 0;
    for (int i = 0; i < (int)poly.size(); i++) area += cross(poly[i], poly[(i + 1) % poly.size()]);
    return fabs(area) / 2;
}

static bool verify_hpi(vector<Halfplane> planes) {
    vector<Point> clipped = {{-100, -100}, {100, -100}, {100, 100}, {-100, 100}};
    for (auto h: planes) {
        if (clipped.empty()) break;
        clipped = clip_polygon(clipped, h);
    }
    vector<Point> got = halfplane_intersection(planes);
    double expected_area = polygon_area(clipped);
    double got_area = polygon_area(got);
    if (fabs(got_area - expected_area) > 1e-6 * max(1.0, expected_area)) {
        printf("FAIL area got=%.12f expected=%.12f vertices=%zu\n", got_area, expected_area,
               got.size());
        for (auto q: planes)
            printf("  {p=(%.3f,%.3f), dir=(%.6f,%.6f), angle=%.6f}\n", q.p.x, q.p.y, q.dir.x,
                   q.dir.y, q.angle);
        return false;
    }
    for (auto p: got)
        for (auto h: planes)
            if (on_outer_side(h, p)) {
                printf("FAIL infeasible output vertex p=(%.12f,%.12f) side=%.12g planes=%zu\n", p.x,
                       p.y, cross(h.dir, p - h.p), planes.size());
                for (auto q: planes)
                    printf("  {p=(%.3f,%.3f), dir=(%.6f,%.6f), angle=%.6f}\n", q.p.x, q.p.y,
                           q.dir.x, q.dir.y, q.angle);
                return false;
            }
    return true;
}

int stress() {
    mt19937 rng(711);
    long long checks = 0;
    vector<Halfplane> box = {
        make_halfplane({-100, -100}, {100, -100}),
        make_halfplane({100, -100}, {100, 100}),
        make_halfplane({100, 100}, {-100, 100}),
        make_halfplane({-100, 100}, {-100, -100}),
    };

    vector<Halfplane> contradiction = box;
    contradiction.push_back(make_halfplane({0, -3}, {1, -3}));
    contradiction.push_back(make_halfplane({1, -44}, {0, -44}));
    if (not halfplane_intersection(contradiction).empty()) {
        printf("FAIL parallel contradiction\n");
        return 1;
    }
    checks++;

    for (int rep = 0; rep < 8000; rep++) {
        vector<Halfplane> planes = box;
        int extra = rng() % 13;
        for (int i = 0; i < extra; i++) {
            Point a = {(double)((int)(rng() % 121) - 60), (double)((int)(rng() % 121) - 60)};
            Point dir = {(double)((int)(rng() % 15) - 7), (double)((int)(rng() % 15) - 7)};
            if (norm(dir) == 0) {
                i--;
                continue;
            }
            planes.push_back(make_halfplane(a, a + dir));
            if (rng() % 5 == 0) planes.push_back(planes.back());
        }
        shuffle(planes.begin(), planes.end(), rng);
        if (not verify_hpi(planes)) return 1;
        checks++;
    }

    vector<Halfplane> parallel = box;
    parallel.push_back(make_halfplane({-5, 0}, {5, 0}));
    parallel.push_back(make_halfplane({-5, 3}, {5, 3}));
    parallel.push_back(make_halfplane({-5, 3}, {5, 3}));
    if (not verify_hpi(parallel)) return 1;
    checks++;

    vector<Halfplane> empty = box;
    empty.push_back(make_halfplane({0, 2}, {0, 1}));
    empty.push_back(make_halfplane({1, 0}, {2, 0}));
    if (not verify_hpi(empty)) return 1;
    checks++;

    vector<Halfplane> wrapped = box;
    wrapped.push_back(make_halfplane({0, 1}, {-1, 1}));
    wrapped.push_back(make_halfplane({0, 2}, {-1, 2 + 0.0}));
    wrapped.push_back(make_halfplane({0, 2}, {-1, 2 - 0.0}));
    if (not verify_hpi(wrapped)) return 1;
    checks++;

    printf("half-plane-intersection PASS %lld", checks);
    return 0;
}
