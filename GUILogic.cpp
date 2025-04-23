#include "GUILogic.hpp"
#include "PerfectMatching.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <math.h>
#include <thread>
#include <numeric>

void GUILogic::drawMainWindow(sf::RenderWindow& window, const std::vector<Matching>& matchings, const std::vector<std::vector<bool>>& adjMatrix, const std::vector<sf::Vector2f>& positions) {
    static sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSerif-Bold.ttf")) {
        std::cout << "Error loading font" << std::endl;
    }

    // Choose vertex colors based on adjacency matrix
    vector<sf::Color> vertexColors(matchings.size(), DEFAULT_VERTEX_COLOR);
    for (size_t i = 0; i < matchings.size(); ++i) {
        if (_isVertexChosen[i]) {
            vertexColors[i] = CHOSEN_VERTEX_COLOR;
            for (size_t j = 0; j < matchings.size(); ++j) {
                if (adjMatrix[i][j] && !_isVertexChosen[j]) {
                    vertexColors[j] = NEAR_CHOSEN_VERTEX_COLOR;
                }
            }
        }
    }

    // Draw edges based on adjacency matrix
    for (size_t i = 0; i < matchings.size(); ++i) {
        for (size_t j = 0; j < matchings.size(); ++j) {
            if (adjMatrix[i][j]) {
                bool isChosen = _isVertexChosen[i] || _isVertexChosen[j];
                sf::Color edgeColor = isChosen ? CHOSEN_EDGE_COLOR : EDGE_COLOR;
                sf::Vertex line[] = {
                    sf::Vertex(positions[i], edgeColor),
                    sf::Vertex(positions[j], edgeColor)
                };
                window.draw(line, 2, sf::Lines);
            }
        }
    }

    // Draw vertices
    for (size_t i = 0; i < matchings.size(); ++i) {
        // Create vertex
        sf::CircleShape vertex(VERTEX_RADIUS);
        vertex.setFillColor(vertexColors[i]);
        vertex.setPosition(positions[i].x - vertex.getRadius(), positions[i].y - vertex.getRadius());
        window.draw(vertex);

        // Draw vertex number
        sf::Text vertexNumber;
        vertexNumber.setFont(font);
        int degree = std::accumulate(adjMatrix[i].begin(), adjMatrix[i].end(), 0);
        vertexNumber.setString(std::to_string(degree) + " \n(" + std::to_string(i) + ")");
        vertexNumber.setCharacterSize(TEXT_SIZE/1.25);
        vertexNumber.setFillColor(sf::Color::Black);
        vertexNumber.setPosition(positions[i].x - 5, positions[i].y - 20);
        window.draw(vertexNumber);
    }

    
}

// Helper function to check if the mouse is over a vertex
int GUILogic::getClickedVertex(const sf::Vector2f& mousePos, const std::vector<sf::Vector2f>& positions) {
    for (size_t i = 0; i < positions.size(); ++i) {
        float dx = mousePos.x - positions[i].x;
        float dy = mousePos.y - positions[i].y;
        float distance = sqrt(dx * dx + dy * dy);
        if (distance < 35) { // Radius of the vertex
            return i;
        }
    }
    return -1; // No vertex clicked
}

// Function to draw the segments of a matching in a new window
void GUILogic::drawSegmentsInNewWindow(const Matching& segments, int vertexIndex) {
    // Create a thread for the new window
    std::thread([this, segments, vertexIndex]() {
        sf::RenderWindow window(sf::VideoMode(800, 600), "Matching Segments");

        const float pointRadius = 5.0f;
        const float lineThickness = 5.0f;

        // Determine min and max coordinates for normalization
        double minX = segments.getMatch(0).p1().x(), minY = segments.getMatch(0).p1().y();
        double maxX = minX, maxY = minY;
        for (const auto& segment : segments) {
            minX = std::min({minX, segment.p1().x(), segment.p2().x()});
            minY = std::min({minY, segment.p1().y(), segment.p2().y()});
            maxX = std::max({maxX, segment.p1().x(), segment.p2().x()});
            maxY = std::max({maxY, segment.p1().y(), segment.p2().y()});
        }

        const float padding = 50.0f;
        const float windowWidth = 800.0f;
        const float windowHeight = 600.0f;
        double rangeX = maxX - minX;
        double rangeY = maxY - minY;
        float scaleX = (windowWidth - 2 * padding) / rangeX;
        float scaleY = (windowHeight - 2 * padding) / rangeY;
        float scale = std::min(scaleX, scaleY);

        std::vector<Segment> normalizedSegments = segments.segments();
        for (auto& segment : normalizedSegments) {
            Point2D p1 = (segment.p1() - Point2D(minX, minY)) * scale + Point2D(padding, padding);
            Point2D p2 = (segment.p2() - Point2D(minX, minY)) * scale + Point2D(padding, padding);
            segment = Segment(p1, p2);
        }

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            window.clear(sf::Color::Black);
            for (const auto& segment : normalizedSegments) {
                sf::Vector2f p1(segment.p1().x(), segment.p1().y());
                sf::Vector2f p2(segment.p2().x(), segment.p2().y());
                sf::Vector2f direction = p2 - p1;

                float length = sqrt(direction.x * direction.x + direction.y * direction.y);
                sf::RectangleShape thickLine(sf::Vector2f(length, lineThickness));
                thickLine.setPosition(p1);
                thickLine.setFillColor(MATCHING_EDGE_COLOR);
                float angle = atan2(direction.y, direction.x) * 180 / 3.14159f;
                thickLine.setRotation(angle);
                window.draw(thickLine);

                sf::CircleShape startPoint(pointRadius);
                startPoint.setFillColor(MATCHING_POINT_COLOR);
                startPoint.setPosition(p1.x - pointRadius, p1.y - pointRadius);
                window.draw(startPoint);

                sf::CircleShape endPoint(pointRadius);
                endPoint.setFillColor(MATCHING_POINT_COLOR);
                endPoint.setPosition(p2.x - pointRadius, p2.y - pointRadius);
                window.draw(endPoint);
            }
            window.display();
        }
        _isVertexChosen[vertexIndex] = false;
    }).detach(); // Detach thread to allow independent operation
}
void GUILogic::run(const std::vector<Point2D>& points, std::vector<Matching>& matchings, std::vector <std::vector<bool>> &adjMatrix) {
    _adjMatrix = adjMatrix;
    
    std::cout << "Number of matchings: " << matchings.size() << std::endl;
    _isVertexChosen = std::vector<bool>(matchings.size(), false);

    sf::RenderWindow window(sf::VideoMode(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT), "Perfect Matching GUI");
    std::vector<sf::Vector2f> positions;

    float centerX = MAIN_WINDOW_WIDTH / 2;
    float centerY = MAIN_WINDOW_HEIGHT / 2;
    float radius = std::min(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT) / 2 - 50;
    for (size_t i = 0; i < matchings.size(); ++i) {
        float angle = 2 * M_PI * i / matchings.size();
        float x = centerX + radius * cos(angle);
        float y = centerY + radius * sin(angle);
        positions.push_back(sf::Vector2f(x, y));
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                int clickedVertex = getClickedVertex(mousePos, positions);
                if (clickedVertex != -1) {
                    _isVertexChosen[clickedVertex] = true;
                    drawSegmentsInNewWindow(matchings[clickedVertex], clickedVertex);
                }
            }
        }

        window.clear(sf::Color::Black);
        drawMainWindow(window, matchings, _adjMatrix, positions);
        window.display();
    }
}