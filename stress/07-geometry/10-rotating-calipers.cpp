static long long brute_diameter_sq(vector<Point> pts) {
    long long best = 0;
    for (int i = 0; i < (int)pts.size(); i++)
        for (int j = i + 1; j < (int)pts.size(); j++) best = max(best, dist_sq(pts[i], pts[j]));
    return best;
}

static double brute_min_width(vector<Point> pts) {
    if (pts.size() < 3) {
        return 0;
    }
    double best = 1e100;
    bool found_direction = false;
    for (int i = 0; i < (int)pts.size(); i++)
        for (int j = i + 1; j < (int)pts.size(); j++) {
            Point edge = pts[j] - pts[i];
            double length = sqrt(edge.x * edge.x + edge.y * edge.y);
            if (length == 0) {
                continue;
            }
            found_direction = true;
            double low = 1e100, high = -1e100;
            for (auto point: pts) {
                double projection = cross(edge, point - pts[i]) / length;
                low = min(low, projection);
                high = max(high, projection);
            }
            best = min(best, high - low);
        }
    return found_direction ? best : 0;
}

int stress() {
    mt19937 rng(710);
    long long checks = 0;
    for (int rep = 0; rep < 12000; rep++) {
        int n = rng() % 31;
        vector<Point> pts(n);
        for (auto& p: pts)
            p = {(double)((int)(rng() % 101) - 50), (double)((int)(rng() % 101) - 50)};
        long long got = convex_diameter_sq(pts);
        long long expected = brute_diameter_sq(pts);
        if (got != expected) {
            printf("FAIL got=%lld expected=%lld n=%d\n", got, expected, n);
            return 1;
        }
        double got_width = convex_min_width(pts);
        double expected_width = brute_min_width(pts);
        if (fabs(got_width - expected_width) > 1e-8) {
            printf("FAIL width got=%.12f expected=%.12f n=%d\n", got_width, expected_width, n);
            return 1;
        }
        checks++;
    }

    for (int rep = 0; rep < 2000; rep++) {
        int n = 1 + rng() % 30;
        vector<Point> pts;
        for (int i = 0; i < n; i++) {
            double t = (int)(rng() % 101) - 50;
            pts.push_back({t, 3 * t + 2});
        }
        if (convex_diameter_sq(pts) != brute_diameter_sq(pts) or
            fabs(convex_min_width(pts) - brute_min_width(pts)) > 1e-8)
            return 1;
        checks++;
    }
    printf("rotating-calipers PASS %lld", checks);
    return 0;
}
