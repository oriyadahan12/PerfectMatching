#include "PerfectMatching.hpp"

bool Matching::operator==(const Matching& other) const {
    if (_segments.size() != other.segments().size()) return false;
    for (unsigned int i = 0; i < _segments.size(); i++) {
        if (_segments[i] != other.segments()[i]) return false;
    }
    return true;
}

bool Matching::intersects(const Segment& s) const {
    for (const auto& seg : _segments) {
        if (Geometry::doIntersect(seg.p1(), seg.p2(), s.p1(), s.p2())) return true;
    }
    return false;
}

bool Matching::isFlip(const Matching &other) const {
    int diffCount = 0;
    vector<Point2D> vertices;

    // Check if the two matchings have exactly 2 different segments
    for (const auto& seg1 : *this) {
        bool found = false;
        for (const auto& seg2 : other) {
            if (seg1 == seg2) {
                found = true; // found a matching segment
                break;
            }
        }
        if (!found) {
            diffCount++;
            vertices.push_back(seg1.p1());
            vertices.push_back(seg1.p2());
        }
    }
    // If the two matchings have other than 2 different segments, they can't be flips
    if (diffCount != 2) return false;

    return vertices.size() == 4;
}

vector<Matching> PerfectMatchingFinder::getAllMatchings(const vector<Point2D> &points) {
    if (points.size() % 2 != 0) throw std::invalid_argument("Number of points must be even");
    vector<Matching> matchings;
    Matching emptyMatching;
    improveMatching(matchings, points, emptyMatching);

    for (unsigned int i = 0; i < matchings.size(); i++) { // sort all the segments in order to compare them.
        matchings[i].sortSegmenting();
    }

    for (unsigned int i = 0; i < matchings.size(); i++) {
        for (unsigned int j = i+1; j < matchings.size(); j++) {
            if (matchings[i] == matchings[j]) { // check if the matching is isomorphic to any other matching.
                matchings[j] = matchings.back();
                matchings.pop_back();
                j--; // decrement j to check the new matching at the same index.
            }
        }
    }

    return matchings;
}


// void PerfectMatchingFinder::buildAdjacencyMatrix(const vector<Matching>& matchings, vector<vector<bool>>& adjMatrix) const {
//     int matchCount = matchings.size();
//     for (int i = 0; i < matchCount; ++i) {
//         for (int j = i + 1; j < matchCount; ++j) {
//             if (areAdjacent(matchings[i], matchings[j])) {
//                 adjMatrix[i][j] = adjMatrix[j][i] = true;
//             }
//         }
//     }
// }

void PerfectMatchingFinder::improveMatching(vector<Matching> &matchings, const vector<Point2D> &points,
                                    Matching &matching)  { // recursive function to find all possible perfect matchings.
    for (unsigned int i = 0; i < points.size()-1; i++) {
        for (unsigned int j = i+1; j < points.size(); j++) {
            Segment s(points[i], points[j]);
            if (matching.intersects(s)) continue;

            matching.addMatch(s);
            if (matching.size() == points.size() / 2) {
                matchings.push_back(matching);
            } else {
                improveMatching(matchings, points, matching);
            }
            matching.removeLastMatch();
        }
    }
}

vector<vector<bool>> PerfectMatchingFinder::getAdjacencyMatrix(const vector<Matching> &matchings) {
    vector<vector<bool>> adjMatrix(matchings.size(), vector<bool>(matchings.size(), false));

    for (unsigned int i = 0; i < matchings.size(); i++) {
        for (unsigned int j = i + 1; j < matchings.size(); j++) {
            if (matchings[i].isFlip(matchings[j])) {
                adjMatrix[i][j] = true;
                adjMatrix[j][i] = true;
            }
            if(i==j) adjMatrix[i][j] = false;
        }
    }

    return adjMatrix;
}
