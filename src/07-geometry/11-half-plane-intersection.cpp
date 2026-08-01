const double EPS = 1e-9;
struct Point {
    double x, y;
};
Point operator+(Point a, Point b) {
    return {a.x + b.x, a.y + b.y};
}
Point operator-(Point a, Point b) {
    return {a.x - b.x, a.y - b.y};
}
Point operator*(Point a, double t) {
    return {a.x * t, a.y * t};
}
double cross(Point a, Point b) {
    return a.x * b.y - a.y * b.x;
}
double dot(Point a, Point b) {
    return a.x * b.x + a.y * b.y;
}
double norm(Point a) {
    return sqrt(dot(a, a));
}

/**
 * Angular-sort/deque intersection of half-planes lying left of p -> p + dir.
 * Returns the bounded intersection polygon in ccw order, or empty for no
 * positive-area region. Add a bounding box for an otherwise unbounded result.
 * Directions are unit length. O(n log n).
 */
struct Halfplane {
    Point p, dir;
    double angle;
};
Halfplane make_halfplane(Point a, Point b) {
    Point dir = b - a;
    dir = dir * (1.0 / norm(dir));
    double angle = atan2(dir.y, dir.x);
    if (dir.x < 0 and dir.y == 0) {
        angle = acos(-1.0);
    }
    return {a, dir, angle};
}
bool on_outer_side(Halfplane h, Point r) {
    return cross(h.dir, r - h.p) < -EPS;
}
Point line_intersection(Halfplane s, Halfplane t) {
    double along = cross(t.p - s.p, t.dir) / cross(s.dir, t.dir);
    return s.p + s.dir * along;
}

vector<Point> halfplane_intersection(vector<Halfplane> planes) {
    sort(planes.begin(), planes.end(),
         [](const Halfplane& a, const Halfplane& b) { return a.angle < b.angle; });

    vector<Halfplane> filtered;
    for (auto h: planes) {
        if (filtered.empty() or fabs(cross(h.dir, filtered.back().dir)) >= EPS) {
            filtered.push_back(h);
        } else if (on_outer_side(h, filtered.back().p)) {
            filtered.back() = h;
        }
    }

    int plane_count = filtered.size();
    vector<double> extended_angles(2 * plane_count);
    const double pi = acos(-1.0);
    for (int i = 0; i < 2 * plane_count; i++) {
        extended_angles[i] = filtered[i % plane_count].angle + (i >= plane_count ? 2 * pi : 0);
    }
    for (int i = 0; i < plane_count; i++) {
        double opposite_angle = extended_angles[i] + pi;
        auto first = lower_bound(extended_angles.begin() + i + 1,
                                 extended_angles.begin() + i + plane_count, opposite_angle - EPS);
        for (auto it = first;
             it != extended_angles.begin() + i + plane_count and *it <= opposite_angle + EPS;
             ++it) {
            Halfplane opposite = filtered[(it - extended_angles.begin()) % plane_count];
            if (fabs(cross(filtered[i].dir, opposite.dir)) < EPS and
                dot(filtered[i].dir, opposite.dir) < 0 and on_outer_side(filtered[i], opposite.p)) {
                return {};
            }
        }
    }

    deque<Halfplane> dq;
    for (auto h: filtered) {
        while (dq.size() > 1 and
               on_outer_side(h, line_intersection(dq[dq.size() - 1], dq[dq.size() - 2]))) {
            dq.pop_back();
        }
        while (dq.size() > 1 and on_outer_side(h, line_intersection(dq[0], dq[1]))) {
            dq.pop_front();
        }

        // Opposite parallel planes can become adjacent only when the region is
        // empty; for equal directions only the tighter plane can contribute.
        if (not dq.empty() and fabs(cross(h.dir, dq.back().dir)) < EPS) {
            if (dot(h.dir, dq.back().dir) < 0) {
                return {};
            }
            if (on_outer_side(h, dq.back().p)) {
                dq.pop_back();
            } else {
                continue;
            }
        }
        dq.push_back(h);
    }

    while (dq.size() > 2 and
           on_outer_side(dq[0], line_intersection(dq[dq.size() - 1], dq[dq.size() - 2]))) {
        dq.pop_back();
    }
    while (dq.size() > 2 and on_outer_side(dq.back(), line_intersection(dq[0], dq[1]))) {
        dq.pop_front();
    }
    if (dq.size() < 3) {
        return {};
    }

    int m = dq.size();
    vector<Point> poly(m);
    for (int i = 0; i < m; i++) {
        poly[i] = line_intersection(dq[i], dq[(i + 1) % m]);
    }
    double twice_area = 0;
    for (int i = 1; i + 1 < m; i++) {
        twice_area += cross(poly[i] - poly[0], poly[i + 1] - poly[0]);
    }
    if (twice_area <= EPS) {
        return {};
    }
    return poly;
}

/**
 * Example: four half-planes cutting out the square [-1, 1] x [-1, 1]; report the
 * vertex count and the enclosed area.
 */
int main() {
    vector<Halfplane> square = {
        make_halfplane({-1, -1}, {1, -1}),  // feasible side is y >= -1
        make_halfplane({1, -1}, {1, 1}),    // feasible side is x <= 1
        make_halfplane({1, 1}, {-1, 1}),    // feasible side is y <= 1
        make_halfplane({-1, 1}, {-1, -1}),  // feasible side is x >= -1
    };
    vector<Point> region = halfplane_intersection(square);

    double area = 0;
    int m = region.size();
    for (int i = 0; i < m; i++) {
        area += cross(region[i], region[(i + 1) % m]);
    }
    area = fabs(area) / 2;

    cout << fixed << setprecision(1) << m << " " << area << "\n";  // -> 4 4.0
    return 0;
}
