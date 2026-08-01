static bool near(double a, double b, double tolerance = 1e-8) {
    return fabs(a - b) <= tolerance * max({1.0, fabs(a), fabs(b)});
}

int stress() {
    mt19937 rng(716);
    long long checks = 0;
    auto coordinate = [&]() { return (double)((int)(rng() % 41) - 20); };
    for (int rep = 0; rep < 12000; rep++) {
        Point3 a = {coordinate(), coordinate(), coordinate()};
        Point3 b = {coordinate(), coordinate(), coordinate()};
        Point3 c = {coordinate(), coordinate(), coordinate()};
        Plane plane{};
        if (not plane_from_points(a, b, c, plane)) continue;

        if (not near(norm(plane.normal), 1) or not near(signed_distance(plane, a), 0) or
            not near(signed_distance(plane, b), 0) or not near(signed_distance(plane, c), 0))
            return 1;
        Point3 point = {coordinate(), coordinate(), coordinate()};
        Point3 projection = project_to_plane(plane, point);
        if (not near(signed_distance(plane, projection), 0) or
            not near(norm(point - projection), fabs(signed_distance(plane, point))))
            return 1;

        Point3 direction = {coordinate(), coordinate(), coordinate()};
        Point3 intersection{};
        bool hit = line_plane_intersection(point, direction, plane, intersection);
        bool expected = fabs(dot(plane.normal, direction)) >= EPS;
        if (hit != expected or (hit and not near(signed_distance(plane, intersection), 0)))
            return 1;
        checks++;
    }

    Point3 a = {0, 0, 0}, b = {1, 0, 0}, c = {0, 1, 0}, d = {0, 0, 1};
    if (not near(tetrahedron_volume(a, b, c, d), 1.0 / 6) or
        not in_tetrahedron({0.1, 0.2, 0.3}, a, b, c, d) or in_tetrahedron({1, 1, 1}, a, b, c, d))
        return 1;

    for (int rep = 0; rep < 2500; rep++) {
        int n = 4 + rng() % 10;
        vector<Point3> points;
        while ((int)points.size() < n) {
            Point3 point = {coordinate(), coordinate(), coordinate()};
            bool duplicate = false;
            for (auto other: points) {
                duplicate = duplicate or norm(point - other) < EPS;
            }
            if (not duplicate) {
                points.push_back(point);
            }
        }

        bool general_position = true;
        for (int i = 0; i < n; i++)
            for (int j = i + 1; j < n; j++)
                for (int k = j + 1; k < n; k++)
                    for (int l = k + 1; l < n; l++)
                        if (fabs(tetrahedron_signed_volume6(points[i], points[j], points[k],
                                                            points[l])) < EPS)
                            general_position = false;
        if (not general_position) {
            rep--;
            continue;
        }

        vector<Face3> faces = convex_hull_3d(points);
        set<array<int, 3>> got;
        map<pair<int, int>, int> edge_count;
        for (auto face: faces) {
            Point3 normal = cross(points[face.b] - points[face.a], points[face.c] - points[face.a]);
            for (auto point: points)
                if (dot(normal, point - points[face.a]) > EPS) return 1;
            array<int, 3> key = {face.a, face.b, face.c};
            sort(key.begin(), key.end());
            got.insert(key);
            edge_count[minmax(face.a, face.b)]++;
            edge_count[minmax(face.b, face.c)]++;
            edge_count[minmax(face.c, face.a)]++;
        }
        for (auto [edge, count]: edge_count)
            if (count != 2) return 1;

        set<array<int, 3>> expected;
        for (int i = 0; i < n; i++)
            for (int j = i + 1; j < n; j++)
                for (int k = j + 1; k < n; k++) {
                    Point3 normal = cross(points[j] - points[i], points[k] - points[i]);
                    bool nonnegative = true, nonpositive = true;
                    for (auto point: points) {
                        double side = dot(normal, point - points[i]);
                        nonnegative = nonnegative and side >= -EPS;
                        nonpositive = nonpositive and side <= EPS;
                    }
                    if (nonnegative or nonpositive) expected.insert({i, j, k});
                }
        if (got != expected) {
            printf("FAIL hull faces got=%zu expected=%zu n=%d\n", got.size(), expected.size(), n);
            return 1;
        }
        checks++;
    }
    printf("geometry-3d PASS %lld", checks + 1);
    return 0;
}
