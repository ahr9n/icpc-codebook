static long long brute_intersections(vector<HorizontalSegment> horizontal,
                                     vector<VerticalSegment> vertical) {
    long long total = 0;
    for (auto h: horizontal) {
        if (h.left > h.right) swap(h.left, h.right);
        for (auto v: vertical) {
            if (v.low > v.high) swap(v.low, v.high);
            total += h.left <= v.x and v.x <= h.right and v.low <= h.y and h.y <= v.high;
        }
    }
    return total;
}

static int brute_semicircle(vector<Point> points, Point pivot) {
    const double pi = acos(-1.0);
    vector<double> angles;
    for (auto point: points) angles.push_back(atan2(point.y - pivot.y, point.x - pivot.x));
    int best = 0;
    for (auto start: angles) {
        int count = 0;
        for (auto angle: angles) {
            double delta = angle - start;
            while (delta < 0) delta += 2 * pi;
            while (delta >= 2 * pi) delta -= 2 * pi;
            count += delta <= pi + EPS;
        }
        best = max(best, count);
    }
    return best;
}

int stress() {
    mt19937 rng(715);
    long long checks = 0;
    for (int rep = 0; rep < 10000; rep++) {
        int h_count = rng() % 20;
        int v_count = rng() % 20;
        vector<HorizontalSegment> horizontal(h_count);
        vector<VerticalSegment> vertical(v_count);
        for (auto& h: horizontal)
            h = {(int)(rng() % 31) - 15, (int)(rng() % 31) - 15, (int)(rng() % 31) - 15};
        for (auto& v: vertical)
            v = {(int)(rng() % 31) - 15, (int)(rng() % 31) - 15, (int)(rng() % 31) - 15};
        if (count_axis_aligned_intersections(horizontal, vertical) !=
            brute_intersections(horizontal, vertical))
            return 1;
        checks++;
    }

    Point pivot = {0, 0};
    for (int rep = 0; rep < 10000; rep++) {
        int n = rng() % 30;
        vector<Point> points;
        while ((int)points.size() < n) {
            Point point = {(double)((int)(rng() % 31) - 15), (double)((int)(rng() % 31) - 15)};
            if (point.x != 0 or point.y != 0) points.push_back(point);
        }
        if (max_points_in_semicircle(points, pivot) != brute_semicircle(points, pivot)) return 1;
        checks++;
    }
    printf("line-radial-sweep PASS %lld", checks);
    return 0;
}
