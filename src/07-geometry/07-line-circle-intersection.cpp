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
double dot(Point a, Point b) {
    return a.x * b.x + a.y * b.y;
}
double norm(Point a) {
    return sqrt(dot(a, a));
}

/**
 * Projection-based circle-line and circle-circle intersections, returning zero,
 * one, or two points. Equal positive-radius concentric circles have infinitely
 * many intersections: test concentric_infinite, while the point list stays
 * empty. Radii must be non-negative and a line needs distinct endpoints. A
 * radius-zero circle is one point. O(1) each.
 */
vector<Point> circle_line_intersection(Point center, double radius, Point a, Point b) {
    Point dir = b - a;
    double len = norm(dir);
    Point unit = dir * (1.0 / len);
    double foot_t = dot(center - a, unit);
    Point foot = a + unit * foot_t;
    double gap = norm(center - foot);

    vector<Point> hits;
    if (gap > radius + EPS) {
        return hits;
    }

    if (gap > radius - EPS) {
        hits.push_back(foot);
        return hits;
    }
    double half_chord = sqrt(radius * radius - gap * gap);
    hits.push_back(foot - unit * half_chord);
    hits.push_back(foot + unit * half_chord);
    return hits;
}

bool concentric_infinite(Point c1, double r1, Point c2, double r2) {
    return norm(c2 - c1) < EPS and fabs(r1 - r2) < EPS and max(r1, r2) >= EPS;
}

vector<Point> circle_circle_intersection(Point c1, double r1, Point c2, double r2) {
    Point between = c2 - c1;
    double d = norm(between);
    vector<Point> hits;

    if (d < EPS) {
        if (r1 < EPS and r2 < EPS) {
            hits.push_back(c1);
        }
        return hits;
    }
    if (d > r1 + r2 + EPS) {
        return hits;
    }
    if (d < fabs(r1 - r2) - EPS) {
        return hits;
    }

    Point unit = between * (1.0 / d);
    double a = (d * d + r1 * r1 - r2 * r2) / (2 * d);
    Point mid = c1 + unit * a;

    if (d > r1 + r2 - EPS or d < fabs(r1 - r2) + EPS) {
        hits.push_back(mid);
        return hits;
    }
    double half_chord = sqrt(r1 * r1 - a * a);
    Point perp = {-unit.y, unit.x};
    hits.push_back(mid + perp * half_chord);
    hits.push_back(mid - perp * half_chord);
    return hits;
}

/**
 * Example: a secant line through the unit circle, a tangent line, and a pair of
 * overlapping circles.
 */
int main() {
    cout << fixed << setprecision(3);

    vector<Point> secant = circle_line_intersection({0, 0}, 1, {-2, 0}, {2, 0});
    cout << secant.size();
    for (auto p: secant) {
        cout << " (" << p.x << "," << p.y << ")";
    }
    cout << "\n";  // -> 2 (-1.000,0.000) (1.000,0.000)

    vector<Point> tangent = circle_line_intersection({0, 0}, 1, {-2, 1}, {2, 1});
    cout << tangent.size();
    for (auto p: tangent) {
        cout << " (" << p.x << "," << p.y << ")";
    }
    cout << "\n";  // -> 1 (0.000,1.000)

    vector<Point> pair = circle_circle_intersection({0, 0}, 2, {3, 0}, 2);
    cout << pair.size();
    for (auto p: pair) {
        cout << " (" << p.x << "," << p.y << ")";
    }
    cout << "\n";  // -> 2 (1.500,1.323) (1.500,-1.323)
    return 0;
}
