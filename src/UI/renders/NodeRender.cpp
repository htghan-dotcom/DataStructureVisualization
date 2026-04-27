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

    if (isSelected) {
        circle.setFillColor(sf::Color(255, 170, 40));
    } else if (isActive) {
        circle.setFillColor(sf::Color(255, 210, 70));
    } else {
        circle.setFillColor(config::kNodeFillColor);
    }

    circle.setOutlineColor(config::kNodeOutlineColor);
    circle.setOutlineThickness(2.4f);
    window.draw(circle);

    sf::Text idText(std::to_string(node.id), font, 13);
    idText.setFillColor(sf::Color::White);
    idText.setPosition(circle.getPosition().x - 5.f, circle.getPosition().y - 9.f);
    window.draw(idText);
}
