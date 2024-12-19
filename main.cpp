#include <iostream>
#include <cmath>
#include <vector>
#include <X11/Xlib.h>
#undef None
#include "PerfectMatching.hpp"
#include "GUILogic.hpp"

using std::vector;
using std::cout;
using std::endl;

vector<Point2D> getPoints(unsigned int n);

int main() {
    if (!XInitThreads()) {
        std::cerr << "Failed to initialize Xlib threading." << std::endl;
        return -1;
    }

    vector<Point2D> points = getPoints(10);

    GUILogic guiLogic;
    guiLogic.run(points); // Start the GUI logic

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
        points.emplace_back(cos(2 * M_PI * i / n), sin(2 * M_PI * i / n));
    return points;
}