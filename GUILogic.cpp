#include <SFML/Graphics.hpp>
#include <vector>
#include "GUILogic.hpp"
#include "PerfectMatching.hpp"
#include <math.h>

void GUILogic::drawMatchings(sf::RenderWindow& window, const std::vector<Matching>& matchings, const std::vector<std::vector<bool>>& adjMatrix, const std::vector<sf::Vector2f>& positions) {
    static sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSerif-Bold.ttf")) {
        // Handle error
    }

    // Draw vertices
    for (size_t i = 0; i < matchings.size(); ++i) {
        // Create vertex
        sf::CircleShape vertex(35);
        vertex.setFillColor(vertexColor);
        vertex.setPosition(positions[i].x - vertex.getRadius(), positions[i].y - vertex.getRadius());
        window.draw(vertex);

        // Draw vertex number
        sf::Text vertexNumber;
        vertexNumber.setFont(font);
        vertexNumber.setString(std::to_string(i));
        vertexNumber.setCharacterSize(30);
        vertexNumber.setFillColor(sf::Color::White);
        vertexNumber.setPosition(positions[i].x - 5, positions[i].y - 20);
        window.draw(vertexNumber);
    }

    // Draw edges based on adjacency matrix
    for (size_t i = 0; i < matchings.size(); ++i) {
        for (size_t j = 0; j < matchings.size(); ++j) {
            if (adjMatrix[i][j]) {
                sf::Vertex line[] = {
                    sf::Vertex(positions[i], edgeColor),
                    sf::Vertex(positions[j], edgeColor)
                };
                window.draw(line, 2, sf::Lines); // Fixed line width
            }
        }
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
// Function to draw the segments of a matching in a new window
void GUILogic::drawSegmentsInNewWindow(const Matching& segments) {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Matching Segments");
    const float pointRadius = 5.0f;  // Radius of the endpoint circles
    const float lineThickness = 5.0f; // Thickness of the line

    // Determine the minimum and maximum coordinates
    double minX = segments.getMatch(0).p1().x(), minY = segments.getMatch(0).p1().y();
    double maxX = minX, maxY = minY;

    for (const auto& segment : segments) {
        minX = std::min({minX, segment.p1().x(), segment.p2().x()});
        minY = std::min({minY, segment.p1().y(), segment.p2().y()});
        maxX = std::max({maxX, segment.p1().x(), segment.p2().x()});
        maxY = std::max({maxY, segment.p1().y(), segment.p2().y()});
    }

    // Calculate the scaling factors
    const float padding = 50.0f; // Padding from the edges of the window
    const float windowWidth = 800.0f;
    const float windowHeight = 600.0f;

    double rangeX = maxX - minX;
    double rangeY = maxY - minY;

    float scaleX = (windowWidth - 2 * padding) / rangeX;
    float scaleY = (windowHeight - 2 * padding) / rangeY;

    // Use the smaller scale to maintain the aspect ratio
    float scale = std::min(scaleX, scaleY);

    // Normalized segments
    std::vector<Segment> normalizedSegments = segments.segments();

    for (auto& segment : normalizedSegments) {
        // Normalize the points to fit the screen
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

        // Draw each normalized segment
        for (const auto& segment : normalizedSegments) {
            // Get start and end points
            sf::Vector2f p1(segment.p1().x(), segment.p1().y());
            sf::Vector2f p2(segment.p2().x(), segment.p2().y());

            // Draw thicker line using a rectangle
            sf::Vector2f direction = p2 - p1;
            float length = sqrt(direction.x * direction.x + direction.y * direction.y);
            sf::RectangleShape thickLine(sf::Vector2f(length, lineThickness));
            thickLine.setPosition(p1);
            thickLine.setFillColor(matchingEdgeColor);

            // Rotate the rectangle to align with the segment
            float angle = atan2(direction.y, direction.x) * 180 / 3.14159f;
            thickLine.setRotation(angle);
            window.draw(thickLine);

            // Draw the start and end points as circles
            sf::CircleShape startPoint(pointRadius);
            startPoint.setFillColor(matchingVertexColor);
            startPoint.setPosition(p1.x - pointRadius, p1.y - pointRadius); // Center the circle
            window.draw(startPoint);

            sf::CircleShape endPoint(pointRadius);
            endPoint.setFillColor(matchingVertexColor);
            endPoint.setPosition(p2.x - pointRadius, p2.y - pointRadius); // Center the circle
            window.draw(endPoint);
        }

        window.display();
    }
}

void GUILogic::run(const std::vector<Point2D>& points) {
    std::vector<Matching> matchings = PerfectMatchingFinder::getAllMatchings(points);
    std::vector<std::vector<bool>> adjMatrix = PerfectMatchingFinder::getAdjacencyMatrix(matchings);

    // Define window size
    sf::VideoMode windowMode(800, 600);
    
    // Center the window on the screen
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    int windowX = (desktopMode.width - windowMode.width) / 2;
    int windowY = (desktopMode.height - windowMode.height) / 2;

    // Create a centered window
    sf::RenderWindow window(windowMode, "Perfect Matching GUI | Oriya Dahan & Chaya Keller");
    window.setPosition(sf::Vector2i(windowX, windowY));

    std::vector<sf::Vector2f> positions; // Store positions of vertices

    // Precompute vertex positions
    float centerX = 400;
    float centerY = 300;
    float radius = 250;
    for (size_t i = 0; i < matchings.size(); ++i) {
        float angle = 2 * 3.14159f * i / matchings.size();
        float x = centerX + radius * cos(angle);
        float y = centerY + radius * sin(angle);
        positions.push_back(sf::Vector2f(x, y));
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Check for mouse click
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                int clickedVertex = getClickedVertex(mousePos, positions);
                if (clickedVertex != -1) {
                    // Open a new window and draw the segments of the clicked vertex
                    drawSegmentsInNewWindow(matchings[clickedVertex]);
                }
            }
        }

        window.clear(sf::Color::Black);
        drawMatchings(window, matchings, adjMatrix, positions); // Pass positions
        window.display();
    }
}