#ifndef PERFECTMATCHING_HPP
#define PERFECTMATCHING_HPP

#include <vector>
#include <stdexcept>
#include "Geometry.hpp"

using std::vector;

class PerfectMatching {
private:

    static void improveMatching(vector<vector<Segment>> &matchings, const vector<Point2D> &points, const vector<Segment> &matching);

    static bool intersects(const vector<Segment> &matching, const Segment &s);

    static bool isomorphic(const vector<Segment> &matching, const vector<Segment> &other);

    static bool isFlip(const vector<Segment> &matching1, const vector<Segment> &matching2);

    

public:

    static vector<vector<Segment>> getAllMatchings(const vector<Point2D> &points);

    static vector<vector<bool>> getAdjacencyMatrix(const vector<vector<Segment>> &matchings);

    void buildAdjacencyMatrix(const vector<vector<Segment>>& matchings, vector<vector<bool>>& adjMatrix) const;

    bool areAdjacent(const vector<Segment>& matching1, const vector<Segment>& matching2) const {
    int commonSegments = 0;
    for (const auto& seg1 : matching1) {
        for (const auto& seg2 : matching2) {
            if (seg1 == seg2) {
                commonSegments++;
            }
        }
    }
    return commonSegments >= 1;  // Example threshold}
    }
};

#endif
