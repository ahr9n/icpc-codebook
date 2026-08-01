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
 * Circle-line and circle-circle intersection, projection based for numerical
 * stability. Both return 0, 1, or 2 points.
 *
 * circle_line: drop the center onto the line to get the foot of the
 * perpendicular; the chord is symmetric about that foot, so the two hits are
 * foot +/- half_chord along the line's unit direction. gap = center-to-line
 * distance decides 2 (secant) / 1 (tangent) / 0 (miss).
 *
 * circle_circle: the intersection points lie on the radical line, perpendicular
 * to the center-to-center segment. Solving r1^2 - a^2 = r2^2 - (d - a)^2 for the
 * signed distance a from c1 to the chord's midpoint reduces this to the same
 * foot +/- half_chord shape. Concentric centers are degenerate: equal radii mean
 * infinitely many points (returned as empty, test concentric_infinite first),
 * different radii mean none.
 *
 * O(1) each.
 */
vector<Point> circle_line_intersection(Point center, double radius, Point a, Point b) {
    Point dir = b - a;
    double len = norm(dir);
    Point unit = dir * (1.0 / len);
    double foot_t = dot(center - a, unit);
    Point foot = a + unit * foot_t;
    double gap = norm(center - foot);

    vector<Point> hits;
    if (gap > radius + EPS) return hits;

    // Classify on gap vs radius, not on half_chord: at a tangent the sqrt below
    // amplifies gap's rounding error, so a half_chord test would split one point
    // into two coincident ones.
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
    return norm(c2 - c1) < EPS and fabs(r1 - r2) < EPS;
}

vector<Point> circle_circle_intersection(Point c1, double r1, Point c2, double r2) {
    Point between = c2 - c1;
    double d = norm(between);
    vector<Point> hits;

    // Coincident centers: either the same circle (handled by concentric_infinite)
    // or nested with no shared point.
    if (d < EPS) return hits;
    if (d > r1 + r2 + EPS) return hits;
    if (d < fabs(r1 - r2) - EPS) return hits;

    Point unit = between * (1.0 / d);
    double a = (d * d + r1 * r1 - r2 * r2) / (2 * d);
    Point mid = c1 + unit * a;

    // Externally or internally tangent: the chord collapses to its midpoint.
    // Decide on the distance boundary rather than the error-amplified half_chord.
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
    for (auto p: secant) cout << " (" << p.x << "," << p.y << ")";
    cout << "\n";  // -> 2 (-1.000,0.000) (1.000,0.000)

    vector<Point> tangent = circle_line_intersection({0, 0}, 1, {-2, 1}, {2, 1});
    cout << tangent.size();
    for (auto p: tangent) cout << " (" << p.x << "," << p.y << ")";
    cout << "\n";  // -> 1 (0.000,1.000)

    vector<Point> pair = circle_circle_intersection({0, 0}, 2, {3, 0}, 2);
    cout << pair.size();
    for (auto p: pair) cout << " (" << p.x << "," << p.y << ")";
    cout << "\n";  // -> 2 (1.500,1.323) (1.500,-1.323)
    return 0;
}
