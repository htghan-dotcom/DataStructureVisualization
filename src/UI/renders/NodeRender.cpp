#include "NodeRender.h"

#include "../../config/Config.h"

namespace {
sf::Vector2f graphToScreen(float nx, float ny, const sf::FloatRect& graphViewport) {
        return sf::Vector2f(graphViewport.left + nx * graphViewport.width,
                                                graphViewport.top + ny * graphViewport.height);
}
}

void NodeRender::draw(sf::RenderWindow& window,
                      const Node& node,
                                            const sf::FloatRect& graphViewport,
                      float radius,
                      const sf::Font& font,
                      bool isSelected,
                      bool isActive) {
    // Phóng to nhẹ kích thước Node khi được chọn/nhấn giữ
    if (isSelected) {
        radius *= 1.2f;
    }

    sf::CircleShape circle(radius);
    circle.setOrigin(radius, radius);
    circle.setPosition(graphToScreen(node.x, node.y, graphViewport));

    sf::Color nodeColor = config::kNodeFillColor;
    sf::Color outlineColor = config::kNodeOutlineColor;
    float outlineThickness = 2.4f;

    if (isSelected) {
        nodeColor = sf::Color(245, 216, 224);
        outlineColor = sf::Color(201, 63, 88); // Viền đỏ đô nổi bật
        outlineThickness = 4.0f;               // Viền dày hơn
    } else if (isActive) {
        nodeColor = sf::Color(252, 235, 176);
        outlineColor = sf::Color(220, 180, 80);
        outlineThickness = 3.0f;
    }

    circle.setFillColor(nodeColor);

    circle.setOutlineColor(outlineColor);
    circle.setOutlineThickness(outlineThickness);
    window.draw(circle);

    sf::Text idText(std::to_string(node.id), font, 13);
    const int luminance = static_cast<int>(nodeColor.r) + static_cast<int>(nodeColor.g) + static_cast<int>(nodeColor.b);
    idText.setFillColor(luminance > 440 ? sf::Color(36, 42, 48) : sf::Color(245, 245, 245));
    const sf::FloatRect textBounds = idText.getLocalBounds();
    idText.setPosition(circle.getPosition().x - textBounds.width * 0.5f - textBounds.left,
                       circle.getPosition().y - textBounds.height * 0.5f - textBounds.top);
    window.draw(idText);
}
