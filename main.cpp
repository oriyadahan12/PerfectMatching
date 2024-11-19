#include <iostream>
#include <vector>
#include "PerfectMatching.hpp"
#include "GUILogic.hpp"

using std::vector;
using std::cout;
using std::endl;

int main() {
    vector<Point2D> points = {
            Point2D(1,0),
            Point2D(0.809,0.587),
            Point2D(-1,0),
            Point2D(-0.309,0.951),
            Point2D(-0.809,0.587),
            Point2D(-0.809,-0.587),
            Point2D(-0.309,-0.951),
            Point2D(0.309,-0.951),
            Point2D(0.809,-0.587),
            Point2D(0.309,0.951)
    };

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
