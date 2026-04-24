#pragma once

#include <SFML/Graphics.hpp>
#include <optional>
#include "../../config/Common.h"

class Button {
public:
    enum class StyleRole {
        Default,
        Play,
        Danger,
        Algorithm,
        IconOnly
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
    void setIcon(const sf::Texture* texture);
    bool contains(const sf::Vector2f& point) const;
    sf::FloatRect bounds() const;
    void draw(sf::RenderWindow& window);

private:
    void alignText();

    bool enabled_ = true;
    bool selected_ = false;
    StyleRole styleRole_ = StyleRole::Default;
    int flashFrames_ = 0;
    
    const sf::Texture* iconTexture_ = nullptr;
    float radius_ = 8.0f;
    sf::CircleShape mTopLeft, mTopRight, mBottomLeft, mBottomRight;
    sf::RectangleShape mHorizRect, mVertRect;
    sf::FloatRect bounds_;
    std::optional<sf::Text> text_;
    void updateColors(bool isHovered);
};
