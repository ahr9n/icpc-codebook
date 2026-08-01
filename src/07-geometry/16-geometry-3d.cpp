const double EPS = 1e-9;
struct Point3 {
    double x, y, z;
};
Point3 operator+(Point3 a, Point3 b) {
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}
Point3 operator-(Point3 a, Point3 b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}
Point3 operator*(Point3 a, double scale) {
    return {a.x * scale, a.y * scale, a.z * scale};
}
double dot(Point3 a, Point3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
Point3 cross(Point3 a, Point3 b) {
    return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}
double norm(Point3 a) {
    return sqrt(dot(a, a));
}

struct Plane {
    Point3 point, normal;
};
struct Face3 {
    int a, b, c;
};

/**
 * Core 3D vector and plane operations. A plane stores one point and a unit
 * normal; signed_distance chooses the normal's positive side, projection drops
 * a perpendicular, and line_plane_intersection returns false for a parallel
 * line. Tetrahedron volume and containment are O(1).
 *
 * Contract: plane_from_points needs three non-collinear points. Tetrahedron
 * containment needs four non-coplanar vertices and includes the boundary.
 */
bool plane_from_points(Point3 a, Point3 b, Point3 c, Plane& plane) {
    Point3 normal = cross(b - a, c - a);
    double length = norm(normal);
    if (length < EPS) {
        return false;
    }
    plane = {a, normal * (1.0 / length)};
    return true;
}

double signed_distance(Plane plane, Point3 point) {
    return dot(plane.normal, point - plane.point);
}

Point3 project_to_plane(Plane plane, Point3 point) {
    return point - plane.normal * signed_distance(plane, point);
}

bool line_plane_intersection(Point3 line_point, Point3 line_direction, Plane plane,
                             Point3& intersection) {
    double denominator = dot(plane.normal, line_direction);
    if (fabs(denominator) < EPS) {
        return false;
    }
    double along = dot(plane.normal, plane.point - line_point) / denominator;
    intersection = line_point + line_direction * along;
    return true;
}

double tetrahedron_signed_volume6(Point3 a, Point3 b, Point3 c, Point3 d) {
    return dot(b - a, cross(c - a, d - a));
}

double tetrahedron_volume(Point3 a, Point3 b, Point3 c, Point3 d) {
    return fabs(tetrahedron_signed_volume6(a, b, c, d)) / 6;
}

bool in_tetrahedron(Point3 point, Point3 a, Point3 b, Point3 c, Point3 d) {
    double whole = fabs(tetrahedron_signed_volume6(a, b, c, d));
    double parts = fabs(tetrahedron_signed_volume6(point, b, c, d)) +
                   fabs(tetrahedron_signed_volume6(a, point, c, d)) +
                   fabs(tetrahedron_signed_volume6(a, b, point, d)) +
                   fabs(tetrahedron_signed_volume6(a, b, c, point));
    return fabs(parts - whole) <= EPS * max(1.0, whole);
}

/**
 * Incremental 3D convex hull. Faces are outward-oriented triangles indexing the
 * original points. A visible patch is removed and its horizon is joined to the
 * new point; an interior point changes nothing. O(n^2 log n), including the
 * map-based horizon maintenance.
 *
 * Contract: at least four unique points in general position (no four coplanar),
 * with coordinate scales for which EPS is meaningful. Returns empty for a
 * coplanar input. Coplanar facets need a separate policy and are not silently
 * triangulated here.
 */
vector<Face3> convex_hull_3d(const vector<Point3>& points) {
    int n = points.size();
    if (n < 4) {
        return {};
    }

    int second = -1, third = -1, fourth = -1;
    for (int i = 1; i < n and second == -1; i++) {
        if (norm(points[i] - points[0]) >= EPS) {
            second = i;
        }
    }
    for (int i = 1; i < n and third == -1; i++) {
        if (i != second and norm(cross(points[second] - points[0], points[i] - points[0])) >= EPS) {
            third = i;
        }
    }
    for (int i = 1; i < n and fourth == -1; i++) {
        if (i != second and i != third and
            fabs(tetrahedron_signed_volume6(points[0], points[second], points[third], points[i])) >=
                EPS) {
            fourth = i;
        }
    }
    if (fourth == -1) {
        return {};
    }

    vector<int> order = {0, second, third, fourth};
    for (int i = 0; i < n; i++) {
        if (i != 0 and i != second and i != third and i != fourth) {
            order.push_back(i);
        }
    }
    Point3 inside = (points[0] + points[second] + points[third] + points[fourth]) * 0.25;
    auto outward_face = [&](int a, int b, int c) {
        if (dot(cross(points[b] - points[a], points[c] - points[a]), inside - points[a]) > 0) {
            swap(b, c);
        }
        return Face3{a, b, c};
    };

    vector<Face3> faces = {outward_face(0, second, third), outward_face(0, fourth, second),
                           outward_face(0, third, fourth), outward_face(second, fourth, third)};
    for (int at = 4; at < n; at++) {
        int point_index = order[at];
        vector<bool> visible(faces.size());
        bool has_visible_face = false;
        for (int i = 0; i < (int)faces.size(); i++) {
            Face3 face = faces[i];
            visible[i] =
                dot(cross(points[face.b] - points[face.a], points[face.c] - points[face.a]),
                    points[point_index] - points[face.a]) > EPS;
            has_visible_face = has_visible_face or visible[i];
        }
        if (not has_visible_face) {
            continue;
        }

        map<pair<int, int>, pair<int, int>> horizon;
        auto add_edge = [&](int a, int b) {
            pair<int, int> key = minmax(a, b);
            auto it = horizon.find(key);
            if (it == horizon.end()) {
                horizon[key] = {a, b};
            } else {
                horizon.erase(it);
            }
        };
        vector<Face3> kept;
        for (int i = 0; i < (int)faces.size(); i++) {
            if (visible[i]) {
                add_edge(faces[i].a, faces[i].b);
                add_edge(faces[i].b, faces[i].c);
                add_edge(faces[i].c, faces[i].a);
            } else {
                kept.push_back(faces[i]);
            }
        }
        faces = kept;
        for (auto [key, edge]: horizon) {
            faces.push_back(outward_face(edge.first, edge.second, point_index));
        }
    }
    return faces;
}

/** Example: plane projection, unit-tetrahedron volume, and tetrahedron hull faces. */
int main() {
    Plane plane{};
    plane_from_points({0, 0, 0}, {1, 0, 0}, {0, 1, 0}, plane);
    Point3 projection = project_to_plane(plane, {2, 3, 4});
    cout << fixed << setprecision(1) << projection.x << " " << projection.y << " " << projection.z
         << "\n";
    cout << tetrahedron_volume({0, 0, 0}, {1, 0, 0}, {0, 1, 0}, {0, 0, 1}) << "\n";
    vector<Point3> points = {{0, 0, 0}, {1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
    cout << convex_hull_3d(points).size() << "\n";
    // -> 2.0 3.0 0.0
    //    0.2
    //    4
    return 0;
}
