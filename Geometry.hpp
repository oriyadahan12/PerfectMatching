#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <iostream>
#include <algorithm>

class Point2D {
private:
    double _x, _y;
    int number;

public:
    Point2D(double x, double y) : _x(x), _y(y) {}
    Point2D(double x, double y, int num) : _x(x), _y(y), number(num){};

    double x() const { return _x; }
    double y() const { return _y; }

    bool operator==(const Point2D &p) const { return _x == p.x() && _y == p.y(); }
    bool operator<(const Point2D &p) const { return (_x < p.x()) || (_x == p.x() && _y < p.y()); } // lexicographical order
    friend std::ostream &operator<<(std::ostream &os, const Point2D &p);

    Point2D operator+(const Point2D &p) const { return Point2D(_x + p.x(), _y + p.y()); }
    Point2D operator-(const Point2D &p) const { return Point2D(_x - p.x(), _y - p.y()); }
    Point2D operator*(double c) const { return Point2D(_x * c, _y * c); }
    Point2D operator/(double c) const { return Point2D(_x / c, _y / c); }
    int getNumber() const {
        return this->number;
    }

};

class Segment {
private:
    Point2D _p1, _p2;

public:
    Segment(double x1, double y1, double x2, double y2) : _p1(x1, y1), _p2(x2, y2) {}
    Segment(Point2D p, Point2D q) : _p1(p), _p2(q) {}

    Point2D p1() const { return _p1; }
    Point2D p2() const { return _p2; }

    bool operator==(const Segment &s) const { return _p1 == s.p1() && _p2 == s.p2(); }
    bool operator!=(const Segment &s) const { return !(*this == s); }
    bool operator<(const Segment &s) const { return _p1 < s.p1() || (_p1 == s.p1() && _p2 < s.p2()); } // lexicographical order

    friend std::ostream &operator<<(std::ostream &os, const Segment &s);

};

enum Orientation {
    COLLINEAR, CLOCKWISE, COUNTERCLOCKWISE//clockwise or counterclockwise, check the orientation of the points
};

class Geometry {
public:
    // return the orientation of the p1->p2->p3
    static Orientation getOrientation(const Point2D &p1, const Point2D &p2, const Point2D &p3);
 
    // check if p is on the segment p1-p2 (or s)
    static bool onSegment(const Point2D &p, const Point2D &s1, const Point2D &s2);
    static bool onSegment(const Point2D &p, const Segment &s) { return onSegment(p,s.p1(),s.p2()); }

    // check if two segments intersect (p1-q1 and p2-q2)
    static bool doIntersect(const Point2D &p1, const Point2D &q1, const Point2D &p2, const Point2D &q2); 
    static bool doIntersect(const Segment& s1, const Segment& s2) { return doIntersect(s1.p1(),s1.p2(),s2.p1(),s2.p2()); }

};


#endif
