#include "NodeRender.h"

#include "../../config/Config.h"

namespace {
constexpr float kGraphMinX = 274.0f;
constexpr float kGraphMaxX = 934.0f;
constexpr float kGraphMinY = 110.0f;
constexpr float kGraphMaxY = 560.0f;

sf::Vector2f graphToScreen(float nx, float ny) {
    return sf::Vector2f(kGraphMinX + nx * (kGraphMaxX - kGraphMinX),
                        kGraphMinY + ny * (kGraphMaxY - kGraphMinY));
}
}

void NodeRender::draw(sf::RenderWindow& window,
                      const Node& node,
                      float radius,
                      const sf::Font& font,
                      bool isSelected,
                      bool isActive) {
    sf::CircleShape circle(radius);
    circle.setOrigin(radius, radius);
    circle.setPosition(graphToScreen(node.x, node.y));

    sf::Color nodeColor = config::kNodeFillColor;

    if (isSelected) {
        nodeColor = sf::Color(245, 216, 224);
    } else if (isActive) {
        nodeColor = sf::Color(252, 235, 176);
    }

    circle.setFillColor(nodeColor);

    circle.setOutlineColor(config::kNodeOutlineColor);
    circle.setOutlineThickness(2.4f);
    window.draw(circle);

    sf::Text idText(std::to_string(node.id), font, 13);
    const int luminance = static_cast<int>(nodeColor.r) + static_cast<int>(nodeColor.g) + static_cast<int>(nodeColor.b);
    idText.setFillColor(luminance > 440 ? sf::Color(36, 42, 48) : sf::Color(245, 245, 245));
    const sf::FloatRect textBounds = idText.getLocalBounds();
    idText.setPosition(circle.getPosition().x - textBounds.width * 0.5f - textBounds.left,
                       circle.getPosition().y - textBounds.height * 0.5f - textBounds.top);
    window.draw(idText);
}
