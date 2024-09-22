#include "Geometry.hpp"

std::ostream &operator<<(std::ostream &os, const Point2D &p) {
    os << "(" << p.x() << ", " << p.y() << ")";
    return os;
}

std::ostream &operator<<(std::ostream &os, const Segment &s) {
    os << s.p1() << ">-<" << s.p2();
    return os;
}

Orientation Geometry::getOrientation(const Point2D &p1, const Point2D &p2, const Point2D &p3) {
    double val = (p2.y() - p1.y()) * (p3.x() - p2.x()) - (p2.x() - p1.x()) * (p3.y() - p2.y());
    if (val == 0) return COLLINEAR;
    return (val > 0) ? CLOCKWISE : COUNTERCLOCKWISE;
}

bool Geometry::onSegment(const Point2D &p, const Point2D &s1, const Point2D &s2) {
    return (p.x() <= std::max(s1.x(), s2.x()) && p.x() >= std::min(s1.x(), s2.x()) &&
            p.y() <= std::max(s1.y(), s2.y()) && p.y() >= std::min(s1.y(), s2.y()));
}

bool Geometry::doIntersect(const Point2D &p1, const Point2D &q1, const Point2D &p2, const Point2D &q2) {

    Orientation o1 = getOrientation(p1, q1, p2);
    Orientation o2 = getOrientation(p1, q1, q2);
    Orientation o3 = getOrientation(p2, q2, p1);
    Orientation o4 = getOrientation(p2, q2, q1);

    if (o1 != o2 && o3 != o4) return true;

    if (o1 == COLLINEAR && onSegment(p1, p2, q1)) return true;
    if (o2 == COLLINEAR && onSegment(p1, q2, q1)) return true;
    if (o3 == COLLINEAR && onSegment(p2, p1, q2)) return true;
    if (o4 == COLLINEAR && onSegment(p2, q1, q2)) return true;

    return false;
}
