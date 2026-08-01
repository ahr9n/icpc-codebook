const double EPS = 1e-10;
struct Point {
    double x, y;
};
struct Circle {
    Point center;
    double radius;
};
Point operator-(Point a, Point b) {
    return {a.x - b.x, a.y - b.y};
}
double dot(Point a, Point b) {
    return a.x * b.x + a.y * b.y;
}
double norm(Point a) {
    return sqrt(dot(a, a));
}

static vector<Circle> unique_circles(vector<Circle> circles) {
    vector<Circle> unique;
    for (auto circle: circles) {
        if (circle.radius <= EPS) {
            continue;
        }
        bool duplicate = false;
        for (auto other: unique) {
            if (norm(circle.center - other.center) <= EPS and
                fabs(circle.radius - other.radius) <= EPS) {
                duplicate = true;
            }
        }
        if (not duplicate) {
            unique.push_back(circle);
        }
    }
    return unique;
}

static void add_wrapped_interval(vector<pair<double, double>>& intervals, double from, double to) {
    const double two_pi = 2 * acos(-1.0);
    while (from < 0) {
        from += two_pi;
        to += two_pi;
    }
    while (from >= two_pi) {
        from -= two_pi;
        to -= two_pi;
    }
    if (to <= two_pi) {
        intervals.push_back({from, to});
    } else {
        intervals.push_back({from, two_pi});
        intervals.push_back({0, to - two_pi});
    }
}

static double arc_area(Circle circle, double from, double to) {
    Point a = {circle.center.x + circle.radius * cos(from),
               circle.center.y + circle.radius * sin(from)};
    Point b = {circle.center.x + circle.radius * cos(to),
               circle.center.y + circle.radius * sin(to)};
    double delta = to - from;
    double endpoints_cross = a.x * b.y - a.y * b.x;
    return (endpoints_cross + circle.radius * circle.radius * (delta - sin(delta))) / 2;
}

/**
 * Areas covered by at least one circle (union) or by every circle
 * (intersection). Each circle contributes only the ccw boundary arcs exposed to
 * the union, or lying inside all other circles for the intersection. Green's
 * theorem converts those arcs directly to signed area, avoiding polygonal
 * approximation. O(n^2 log n) time and O(n) extra space.
 *
 * Radii must be non-negative. Duplicate circles are ignored. Tangencies and
 * radius-zero circles have zero area; intersection_area requires a non-empty
 * input and returns zero when no positive-area common region exists.
 */
double circle_union_area(vector<Circle> circles) {
    circles = unique_circles(circles);
    const double two_pi = 2 * acos(-1.0);
    double area = 0;
    for (int i = 0; i < (int)circles.size(); i++) {
        vector<pair<double, double>> covered;
        bool hidden = false;
        for (int j = 0; j < (int)circles.size(); j++) {
            if (i == j) {
                continue;
            }
            Circle a = circles[i], b = circles[j];
            Point between = b.center - a.center;
            double distance = norm(between);
            if (distance + a.radius <= b.radius + EPS) {
                hidden = true;
                break;
            }
            if (distance >= a.radius + b.radius - EPS or distance + b.radius <= a.radius + EPS) {
                continue;
            }
            double direction = atan2(between.y, between.x);
            double cosine = (distance * distance + a.radius * a.radius - b.radius * b.radius) /
                            (2 * distance * a.radius);
            double half = acos(clamp(cosine, -1.0, 1.0));
            add_wrapped_interval(covered, direction - half, direction + half);
        }
        if (hidden) {
            continue;
        }

        sort(covered.begin(), covered.end());
        double exposed_from = 0;
        for (auto [left, right]: covered) {
            if (left > exposed_from + EPS) {
                area += arc_area(circles[i], exposed_from, left);
            }
            exposed_from = max(exposed_from, right);
        }
        if (exposed_from < two_pi - EPS) {
            area += arc_area(circles[i], exposed_from, two_pi);
        }
    }
    return max(0.0, area);
}

double circle_intersection_area(vector<Circle> circles) {
    circles = unique_circles(circles);
    if (circles.empty()) {
        return 0;
    }
    const double two_pi = 2 * acos(-1.0);
    double area = 0;
    for (int i = 0; i < (int)circles.size(); i++) {
        vector<pair<double, double>> allowed = {{0, two_pi}};
        for (int j = 0; j < (int)circles.size() and not allowed.empty(); j++) {
            if (i == j) {
                continue;
            }
            Circle a = circles[i], b = circles[j];
            Point between = b.center - a.center;
            double distance = norm(between);
            if (distance + a.radius <= b.radius + EPS) {
                continue;
            }
            if (distance >= a.radius + b.radius - EPS) {
                return 0;
            }
            if (distance + b.radius <= a.radius + EPS) {
                allowed.clear();
                break;
            }

            double direction = atan2(between.y, between.x);
            double cosine = (distance * distance + a.radius * a.radius - b.radius * b.radius) /
                            (2 * distance * a.radius);
            double half = acos(clamp(cosine, -1.0, 1.0));
            vector<pair<double, double>> inside;
            add_wrapped_interval(inside, direction - half, direction + half);
            vector<pair<double, double>> next;
            for (auto [left, right]: allowed) {
                for (auto [inside_left, inside_right]: inside) {
                    double overlap_left = max(left, inside_left);
                    double overlap_right = min(right, inside_right);
                    if (overlap_left < overlap_right + EPS) {
                        next.push_back({overlap_left, overlap_right});
                    }
                }
            }
            allowed = next;
        }
        for (auto [left, right]: allowed) {
            if (left < right - EPS) {
                area += arc_area(circles[i], left, right);
            }
        }
    }
    return max(0.0, area);
}

/** Example: union and common-lens area of two radius-two circles. */
int main() {
    vector<Circle> circles = {{{0, 0}, 2}, {{2, 0}, 2}};
    cout << fixed << setprecision(3) << circle_union_area(circles) << "\n";
    cout << circle_intersection_area(circles) << "\n";
    // -> 20.219
    //    4.913
    return 0;
}
