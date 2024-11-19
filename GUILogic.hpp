#ifndef GUILOGIC_HPP
#define GUILOGIC_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include "PerfectMatching.hpp"

using std::cout;
using std::endl;
using std::vector;

struct Match {
    Segment segment; // Define Match using Segment
};

class GUILogic {
private:
    const int VERTEX_RADIUS = 25;
    const int TEXT_SIZE = VERTEX_RADIUS-5;
    const sf::Color VERTEX_COLOR = sf::Color(255,0,255);
    const sf::Color EDGE_COLOR = sf::Color::White;

    const int POINT_RADIUS = 5;
    const sf::Color MATCHING_POINT_COLOR = sf::Color::White;
    const sf::Color MATCHING_EDGE_COLOR = sf::Color(59, 176, 255);

    const int MAIN_WINDOW_WIDTH;
    const int MAIN_WINDOW_HEIGHT;

public:
    inline GUILogic() : MAIN_WINDOW_WIDTH(sf::VideoMode::getDesktopMode().width),
                        MAIN_WINDOW_HEIGHT(sf::VideoMode::getDesktopMode().height - 50) {}
    void run(const std::vector<Point2D> &points);

private:
    std::vector<Point2D> initializePoints();
    void drawMainWindow(sf::RenderWindow& window, const std::vector<Matching>& matchings, const std::vector<std::vector<bool>>& adjMatrix, const std::vector<sf::Vector2f>& positions);   
    void drawAdjacencyEdges(sf::RenderWindow& window, const std::vector<Matching>& matchings, const std::vector<std::vector<bool>>& adjMatrix);
    sf::Vector2f getMidpoint(const Matching& matching);
    int getClickedVertex(const sf::Vector2f& mousePos, const std::vector<sf::Vector2f>& positions);
    void drawSegmentsInNewWindow(const Matching& segments);

};

#endif // GUILOGIC_HPP
