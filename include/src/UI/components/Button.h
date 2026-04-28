#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <optional>

class Button {
public:
    enum class StyleRole {
        Default,
        Primary,
        Danger,
        IconOnly,
        Pill
    };

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
    void updateColors(bool isHovered);

    sf::FloatRect bounds_;
    std::optional<sf::Text> text_;
    const sf::Texture* iconTexture_ = nullptr;
    bool enabled_ = true;
    bool selected_ = false;
    int flashFrames_ = 0;
    StyleRole styleRole_ = StyleRole::Default;

    // Cached colors from updateColors
    sf::Color baseColor_;
    sf::Color textColor_;
};