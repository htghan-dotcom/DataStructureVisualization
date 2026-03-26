#include "Button.h"

Button::Button(const std::string& label, const sf::Font& font) {
    background_.setSize(sf::Vector2f(180.f, 34.f));
    background_.setFillColor(sf::Color(35, 40, 52));
    background_.setOutlineThickness(1.f);
    background_.setOutlineColor(sf::Color(90, 90, 100));

    text_.setFont(font);
    text_.setString(label);
    text_.setCharacterSize(14);
    text_.setFillColor(sf::Color::White);
}

void Button::setLabel(const std::string& label) {
    text_.setString(label);
}

void Button::setSize(float width, float height) {
    background_.setSize(sf::Vector2f(width, height));
}

void Button::setPosition(float x, float y) {
    background_.setPosition(x, y);
    text_.setPosition(x + 12.f, y + (background_.getSize().y - text_.getCharacterSize()) * 0.5f - 2.f);
}

void Button::setEnabled(bool enabled) {
    enabled_ = enabled;
}

bool Button::isEnabled() const {
    return enabled_;
}

void Button::flash() {
    flashFrames_ = 2;
}

void Button::setSelected(bool selected) {
    selected_ = selected;
}

bool Button::contains(const sf::Vector2f& point) const {
    return enabled_ && background_.getGlobalBounds().contains(point);
}

void Button::draw(sf::RenderWindow& window) {
    sf::RectangleShape background = background_;
    sf::Text text = text_;

    const bool flashing = flashFrames_ > 0;
    if (enabled_ && (flashing || selected_)) {
        background.setFillColor(sf::Color(255, 221, 92));
        background.setOutlineColor(sf::Color(130, 106, 16));
        text.setFillColor(sf::Color(30, 30, 30));
    } else if (enabled_) {
        background.setFillColor(sf::Color(35, 40, 52));
        background.setOutlineColor(sf::Color(90, 90, 100));
        text.setFillColor(sf::Color::White);
    } else {
        background.setFillColor(sf::Color(45, 50, 58));
        background.setOutlineColor(sf::Color(85, 85, 90));
        text.setFillColor(sf::Color(155, 155, 160));
    }

    window.draw(background);
    window.draw(text);

    if (flashFrames_ > 0) {
        --flashFrames_;
    }
}
