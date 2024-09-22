#include <iostream>
#include <vector>
#include "PerfectMatching.hpp"
#include "GUILogic.hpp"

using std::vector;
using std::cout;
using std::endl;

int main() {
    vector<Point2D> points = {
            Point2D(0,0),
            Point2D(0,1),
            Point2D(1,0),
            Point2D(1,1),
            Point2D(2,1),
            Point2D(2,2),
            Point2D(3,0),
            Point2D(3,4),
            Point2D(4,1),
            Point2D(4,3)
    };

    GUILogic guiLogic;

    guiLogic.run(points); // Start the GUI logic

    return 0;
}
