#ifndef PERFECTMATCHING_HPP
#define PERFECTMATCHING_HPP

#include <vector>
#include <stdexcept>
#include "Geometry.hpp"

using std::vector;

class Matching {
    private:
        vector<Segment> _segments;

    public:
        Matching() = default;
        Matching(const vector<Segment>& segments) : _segments(segments) {}

        const vector<Segment>& segments() const { return _segments; }

        inline void sortSegmenting() { std::sort(_segments.begin(), _segments.end()); }

        bool operator==(const Matching& other) const;

        bool intersects(const Segment& s) const;
        bool isFlip(const Matching& other) const;

        void addMatch(const Segment& s) { _segments.push_back(s); }
        void removeLastMatch() { _segments.pop_back(); }
        size_t size() const { return _segments.size(); }

        auto begin() const { return _segments.begin(); }
        auto end() const { return _segments.end(); }
        inline Segment getMatch(unsigned int i) const { return _segments[i]; }

        bool isRotate(Matching &m, int rotations);
};

class PerfectMatchingFinder {
private:

    static void improveMatching(vector<Matching> &matchings, const vector<Point2D> &points, Matching &matching);

public:

    // get all possible perfect matchings without repetitions or intersections
    static vector<Matching> getAllMatchings(const vector<Point2D> &points);

    // get the adjacency matrix of the matchings
    static vector<vector<bool>> getAdjacencyMatrix(const vector<Matching> &matchings);

    // void buildAdjacencyMatrix(const vector<Matching>& matchings, vector<vector<bool>>& adjMatrix) const;

    // bool areAdjacent(const Matching& matching1, const Matching& matching2) const {
    // int commonSegments = 0;
    // for (const auto& seg1 : matching1) {
    //     for (const auto& seg2 : matching2) {
    //         if (seg1 == seg2) {
    //             commonSegments++;
    //         }
    //     }
    // }
    // return commonSegments >= 1;  // Example threshold}
    // }
};

#endif
