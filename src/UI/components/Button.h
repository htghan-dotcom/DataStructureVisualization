#pragma once

#include <SFML/Graphics.hpp>
#include "../../config/Common.h"

class Button {
public:
    enum class StyleRole {
        Default,
        Play,
        Danger,
        Algorithm
    };

    Button() = default;
    Button(const std::string& label, const sf::Font& font);

    void setLabel(const std::string& label);
    void setSize(float width, float height);
    void setPosition(float x, float y);
    void setEnabled(bool enabled);
    bool isEnabled() const;
    void setStyleRole(StyleRole role);
    void flash();
    void setSelected(bool selected);
    bool contains(const sf::Vector2f& point) const;
    void draw(sf::RenderWindow& window);

private:
    void alignText();

    bool enabled_ = true;
    bool selected_ = false;
    StyleRole styleRole_ = StyleRole::Default;
    int flashFrames_ = 0;
    sf::RectangleShape background_;
    sf::Text text_;
};
