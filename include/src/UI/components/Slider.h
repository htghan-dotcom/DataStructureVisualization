#pragma once
#include <optional>
#include <SFML/Graphics.hpp>

class Slider {
public:
    Slider() = default;
    Slider(float x, float y, float width, float minValue, float maxValue, float initial, const sf::Font& font);

    bool contains(const sf::Vector2f& point) const;
    void setPosition(float x, float y);
    void setActive(bool active);
    void onMouseMoved(const sf::Vector2f& point);
    void setValue(float value);
    float getValue() const;
    void draw(sf::RenderWindow& window) const;

private:
    float minValue_ = 1.0f;
    float maxValue_ = 10.0f;
    float value_ = 1.0f;
    bool active_ = false;

    sf::RectangleShape track_;
    sf::CircleShape knob_;
    
    std::optional<sf::Text> label_;

    void updateKnobPosition();
};
