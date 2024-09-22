#include "PerfectMatching.hpp"

vector<vector<Segment>> PerfectMatching::getAllMatchings(const vector<Point2D> &points) {//get all possible perfect matchings without repetitions.
    if (points.size() % 2 != 0) throw std::invalid_argument("Number of points must be even");
    vector<vector<Segment>> matchings;
    improveMatching(matchings, points, {});

    for (unsigned int i = 0; i < matchings.size(); i++) { // sort all the segments in order to compare them.
        std::sort(matchings[i].begin(), matchings[i].end());
    }

    for (unsigned int i = 0; i < matchings.size(); i++) {
        for (unsigned int j = i+1; j < matchings.size(); j++) {
            if (matchings[i] == matchings[j]) { //check if the matching is isomorphic to any other matching.
                matchings.erase(matchings.begin() + j);
                j--;
            }
        }
    }

    return matchings;
}


void PerfectMatching::buildAdjacencyMatrix(const vector<vector<Segment>>& matchings, vector<vector<bool>>& adjMatrix) const {
    int matchCount = matchings.size();
    for (int i = 0; i < matchCount; ++i) {
        for (int j = i + 1; j < matchCount; ++j) {
            if (areAdjacent(matchings[i], matchings[j])) {
                adjMatrix[i][j] = adjMatrix[j][i] = true;
            }
        }
    }
}


bool PerfectMatching::intersects(const vector<Segment> &matching, const Segment &s) {
    return std::any_of(matching.begin(), matching.end(), [&](const Segment &m) {//check if the segment intersects with any other segment in the matching.
        return Geometry::doIntersect(m, s);
    });
}

void PerfectMatching::improveMatching(vector<vector<Segment>> &matchings, const vector<Point2D> &points,
                                      const vector<Segment> &matching) {//recursive function to find all possible perfect matchings.
    for (unsigned int i = 0; i < points.size()-1; i++) {
        for (unsigned int j = i+1; j < points.size(); j++) {
            Segment s(points[i], points[j]);
            if (intersects(matching, s)) continue;

            vector<Segment> newMatching = matching;
            newMatching.push_back(s);
            if (newMatching.size() == points.size() / 2) {
                matchings.push_back(newMatching);
            } else {
                improveMatching(matchings, points, newMatching);
            }
        }
    }

}

bool PerfectMatching::isomorphic(const vector<Segment> &matching, const vector<Segment> &other) {
    vector<Segment> copy1 = matching;
    vector<Segment> copy2 = other;
    std::sort(copy1.begin(), copy1.end());
    std::sort(copy2.begin(), copy2.end());

    for (unsigned int i = 0; i < copy1.size(); i++)
        if (copy1[i] != copy2[i]) return false;

    return true;
}

bool PerfectMatching::isFlip(const vector<Segment> &matching1, const vector<Segment> &matching2) {
    int diffCount = 0;
    vector<Point2D> vertices;

    for (const auto& seg1 : matching1) {
        bool found = false;
        for (const auto& seg2 : matching2) {
            if (seg1 == seg2) {
                found = true;
                break;
            }
        }
        if (!found) {
            diffCount++;
            vertices.push_back(seg1.p1());
            vertices.push_back(seg1.p2());
        }
    }

    for (const auto& seg2 : matching2) {
        bool found = false;
        for (const auto& seg1 : matching1) {
            if (seg1 == seg2) {
                found = true;
                break;
            }
        }
        if (!found) {
            diffCount++;
            vertices.push_back(seg2.p1());
            vertices.push_back(seg2.p2());
        }
    }

    if (diffCount != 4) return false;

    std::sort(vertices.begin(), vertices.end());
    vertices.erase(std::unique(vertices.begin(), vertices.end()), vertices.end());

    return vertices.size() == 4;
}

vector<vector<bool>> PerfectMatching::getAdjacencyMatrix(const vector<vector<Segment>> &matchings) {
    vector<vector<bool>> adjMatrix(matchings.size(), vector<bool>(matchings.size(), false));

    for (unsigned int i = 0; i < matchings.size(); i++) {
        for (unsigned int j = i + 1; j < matchings.size(); j++) {
            if (isFlip(matchings[i], matchings[j])) {
                adjMatrix[i][j] = true;
                adjMatrix[j][i] = true;
            }
            if(i==j) adjMatrix[i][j] = false;
        }
    }

    return adjMatrix;
}
