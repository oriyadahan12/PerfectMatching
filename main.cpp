#include <iostream>
#include <cmath>
#include <vector>
#include <X11/Xlib.h>
#undef None
#include "PerfectMatching.hpp"
#include "GUILogic.hpp"
#include "AutomorphismFinder.hpp"

using std::vector;
using std::cout;
using std::endl;

vector<Point2D> getPoints(unsigned int n);

int main() {
    if (!XInitThreads()) {
        std::cerr << "Failed to initialize Xlib threading." << std::endl;
        return -1;
    }

    vector<Point2D> points = getPoints(8);

    GUILogic guiLogic;
    std::vector<Matching> matchings = PerfectMatchingFinder::getAllMatchings(points);
    std::vector <std::vector<bool>> adjMatrix = PerfectMatchingFinder::getAdjacencyMatrix(matchings);
    // std::vector<std::vector<int>> automorphisms = AutomorphismFinder::findAutomorphisms(adjMatrix, matchings);

    // for (auto automorphism : automorphisms) {
    //     for (int i : automorphism) {
    //         std::cout << i << ", ";
    //     }
    //     std::cout << std::endl;
    // }

    guiLogic.run(points, matchings, adjMatrix); // Start the GUI logic

    // vector<Matching> matchings = PerfectMatchingFinder::getAllMatchings(points);
    // for (const auto& matching : matchings) {
    //     for (const auto& segment : matching) {
    //         cout << segment << " ";
    //     }
    //     cout << endl;
    // }

    return 0;
}


vector<Point2D> getPoints(unsigned int n) {
    vector<Point2D> points;
    points.reserve(n);
    for (unsigned int i = 0; i < n; ++i)
        points.emplace_back(cos(2 * M_PI * i / n), sin(2 * M_PI * i / n), i);
    return points;
}

