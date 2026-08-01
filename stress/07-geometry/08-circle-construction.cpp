static Circle brute_mec(vector<Point> pts) {
    vector<Circle> candidates;
    for (auto p: pts) candidates.push_back({p, 0});
    for (int i = 0; i < (int)pts.size(); i++)
        for (int j = i + 1; j < (int)pts.size(); j++)
            candidates.push_back(circle_from_2(pts[i], pts[j]));
    for (int i = 0; i < (int)pts.size(); i++)
        for (int j = i + 1; j < (int)pts.size(); j++)
            for (int k = j + 1; k < (int)pts.size(); k++) {
                Circle candidate{};
                if (circumcircle(pts[i], pts[j], pts[k], candidate))
                    candidates.push_back(candidate);
            }

    Circle best = {{0, 0}, 1e100};
    for (auto candidate: candidates) {
        bool covers = true;
        for (auto p: pts) covers = covers and in_circle(candidate, p);
        if (covers and candidate.r < best.r) best = candidate;
    }
    return best;
}

int stress() {
    mt19937 rng(708);
    long long checks = 0;
    for (int rep = 0; rep < 8000; rep++) {
        int n = 1 + rng() % 8;
        vector<Point> pts(n);
        for (auto& p: pts) p = {(double)((int)(rng() % 17) - 8), (double)((int)(rng() % 17) - 8)};
        Circle got = min_enclosing_circle(pts);
        Circle expected = brute_mec(pts);
        if (fabs(got.r - expected.r) > 1e-7) {
            printf("FAIL radius got=%.12f expected=%.12f n=%d\n", got.r, expected.r, n);
            return 1;
        }
        for (auto p: pts)
            if (not in_circle(got, p)) {
                printf("FAIL uncovered point\n");
                return 1;
            }
        checks++;
    }

    for (int rep = 0; rep < 2000; rep++) {
        int n = 1 + rng() % 12;
        vector<Point> pts;
        for (int i = 0; i < n; i++) {
            double t = (int)(rng() % 31) - 15;
            pts.push_back({2 * t + 3, -3 * t + 1});
        }
        Circle got = min_enclosing_circle(pts);
        Circle expected = brute_mec(pts);
        if (fabs(got.r - expected.r) > 1e-7) {
            printf("FAIL collinear radius got=%.12f expected=%.12f\n", got.r, expected.r);
            return 1;
        }
        checks++;
    }

    Circle collinear = circle_from_3({-5, 0}, {0, 0}, {7, 0});
    if (fabs(collinear.center.x - 1) > 1e-9 or fabs(collinear.r - 6) > 1e-9) return 1;
    printf("circle-construction PASS %lld", checks + 1);
    return 0;
}
