#include "NodeRender.h"

#include "../../config/Config.h"

namespace {
constexpr float kGraphMinX = 230.0f;
constexpr float kGraphMaxX = 840.0f;
constexpr float kGraphMinY = 220.0f;
constexpr float kGraphMaxY = 680.0f;

sf::Vector2f graphToScreen(float nx, float ny) {
    return sf::Vector2f(kGraphMinX + nx * (kGraphMaxX - kGraphMinX),
                        kGraphMinY + ny * (kGraphMaxY - kGraphMinY));
}
}

void NodeRender::draw(sf::RenderWindow& window, const Node& node, float radius, const sf::Font& font) {
    sf::CircleShape circle(radius);
    circle.setOrigin(radius, radius);
    circle.setPosition(graphToScreen(node.x, node.y));
    circle.setFillColor(config::kNodeFillColor);
    circle.setOutlineColor(config::kNodeOutlineColor);
    circle.setOutlineThickness(2.f);
    window.draw(circle);

    sf::Text idText(std::to_string(node.id), font, 13);
    idText.setFillColor(sf::Color::White);
    idText.setPosition(circle.getPosition().x - 5.f, circle.getPosition().y - 9.f);
    window.draw(idText);
}
