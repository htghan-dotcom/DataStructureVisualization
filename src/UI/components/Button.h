#pragma once

#include <SFML/Graphics.hpp>
#include "../../config/Common.h"

class Button {
public:
    Button() = default;
    Button(const std::string& label, const sf::Font& font);

    void setLabel(const std::string& label);
    void setSize(float width, float height);
    void setPosition(float x, float y);
    void setEnabled(bool enabled);
    bool isEnabled() const;
    void flash();
    void setSelected(bool selected);
    bool contains(const sf::Vector2f& point) const;
    void draw(sf::RenderWindow& window);

private:
    bool enabled_ = true;
    bool selected_ = false;
    int flashFrames_ = 0;
    sf::RectangleShape background_;
    sf::Text text_;
};
