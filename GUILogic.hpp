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
    // Color vertexColor = sf::Color::Color(255,0,255);

public:
    void run(const std::vector<Point2D> &points);

private:
    std::vector<Point2D> initializePoints();
    void drawMatchings(sf::RenderWindow& window, const std::vector<std::vector<Segment>>& matchings, const std::vector<std::vector<bool>>& adjMatrix, const std::vector<sf::Vector2f>& positions);   
    void drawAdjacencyEdges(sf::RenderWindow& window, const std::vector<std::vector<Segment>>& matchings, const std::vector<std::vector<bool>>& adjMatrix);
    sf::Vector2f getMidpoint(const std::vector<Segment>& matching);
    int getClickedVertex(const sf::Vector2f& mousePos, const std::vector<sf::Vector2f>& positions);
    void drawSegmentsInNewWindow(const std::vector<Segment>& segments);

};

#endif // GUILOGIC_HPP
