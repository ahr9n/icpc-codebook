static double pair_intersection(Circle a, Circle b) {
    double distance = norm(b.center - a.center);
    if (distance >= a.radius + b.radius) return 0;
    if (distance <= fabs(a.radius - b.radius)) {
        double radius = min(a.radius, b.radius);
        return acos(-1.0) * radius * radius;
    }
    double alpha = acos((distance * distance + a.radius * a.radius - b.radius * b.radius) /
                        (2 * distance * a.radius));
    double beta = acos((distance * distance + b.radius * b.radius - a.radius * a.radius) /
                       (2 * distance * b.radius));
    double triangle_twice =
        sqrt((-distance + a.radius + b.radius) * (distance + a.radius - b.radius) *
             (distance - a.radius + b.radius) * (distance + a.radius + b.radius));
    return a.radius * a.radius * alpha + b.radius * b.radius * beta - triangle_twice / 2;
}

static pair<double, double> grid_areas(vector<Circle> circles) {
    double min_x = 1e100, max_x = -1e100, min_y = 1e100, max_y = -1e100;
    for (auto circle: circles) {
        min_x = min(min_x, circle.center.x - circle.radius);
        max_x = max(max_x, circle.center.x + circle.radius);
        min_y = min(min_y, circle.center.y - circle.radius);
        max_y = max(max_y, circle.center.y + circle.radius);
    }
    const int steps = 220;
    double dx = (max_x - min_x) / steps;
    double dy = (max_y - min_y) / steps;
    long long union_cells = 0, intersection_cells = 0;
    for (int ix = 0; ix < steps; ix++)
        for (int iy = 0; iy < steps; iy++) {
            Point point = {min_x + (ix + 0.5) * dx, min_y + (iy + 0.5) * dy};
            int inside = 0;
            for (auto circle: circles) inside += norm(point - circle.center) <= circle.radius;
            union_cells += inside > 0;
            intersection_cells += inside == (int)circles.size();
        }
    return {union_cells * dx * dy, intersection_cells * dx * dy};
}

int stress() {
    mt19937 rng(714);
    long long checks = 0;
    const double pi = acos(-1.0);
    for (int rep = 0; rep < 10000; rep++) {
        Circle a = {{(double)((int)(rng() % 41) - 20), (double)((int)(rng() % 41) - 20)},
                    (double)(1 + rng() % 15)};
        Circle b = {{(double)((int)(rng() % 41) - 20), (double)((int)(rng() % 41) - 20)},
                    (double)(1 + rng() % 15)};
        vector<Circle> circles = {a, b};
        double intersection = pair_intersection(a, b);
        double expected_union = pi * (a.radius * a.radius + b.radius * b.radius) - intersection;
        if (fabs(circle_intersection_area(circles) - intersection) > 2e-7 or
            fabs(circle_union_area(circles) - expected_union) > 2e-7) {
            printf("FAIL pair\n");
            return 1;
        }
        checks++;
    }

    for (int rep = 0; rep < 250; rep++) {
        int n = 1 + rng() % 5;
        vector<Circle> circles;
        for (int i = 0; i < n; i++)
            circles.push_back({{(double)((int)(rng() % 21) - 10), (double)((int)(rng() % 21) - 10)},
                               (double)(1 + rng() % 10)});
        auto [grid_union, grid_intersection] = grid_areas(circles);
        double box_scale = 1;
        for (auto circle: circles) box_scale = max(box_scale, pi * circle.radius * circle.radius);
        if (fabs(circle_union_area(circles) - grid_union) > 0.015 * box_scale or
            fabs(circle_intersection_area(circles) - grid_intersection) > 0.015 * box_scale) {
            printf("FAIL grid n=%d union=%.6f/%.6f intersection=%.6f/%.6f\n", n,
                   circle_union_area(circles), grid_union, circle_intersection_area(circles),
                   grid_intersection);
            return 1;
        }
        checks++;
    }

    vector<Circle> duplicate = {{{3, -2}, 5}, {{3, -2}, 5}};
    if (fabs(circle_union_area(duplicate) - 25 * pi) > 1e-8 or
        fabs(circle_intersection_area(duplicate) - 25 * pi) > 1e-8)
        return 1;
    printf("circle-union-intersection-area PASS %lld", checks + 1);
    return 0;
}
