#include "Button.h"

#include <algorithm>

namespace {
void drawRoundedFill(sf::RenderWindow& window, const sf::FloatRect& rect, float radius, const sf::Color& color) {
    if (rect.width <= 0.f || rect.height <= 0.f) {
        return;
    }

    const float r = std::clamp(radius, 0.0f, std::min(rect.width, rect.height) * 0.5f);
    if (r <= 0.01f) {
        sf::RectangleShape box(sf::Vector2f(rect.width, rect.height));
        box.setPosition(rect.left, rect.top);
        box.setFillColor(color);
        window.draw(box);
        return;
    }

    sf::RectangleShape center(sf::Vector2f(rect.width - 2.f * r, rect.height));
    center.setPosition(rect.left + r, rect.top);
    center.setFillColor(color);
    window.draw(center);

    sf::RectangleShape middle(sf::Vector2f(rect.width, rect.height - 2.f * r));
    middle.setPosition(rect.left, rect.top + r);
    middle.setFillColor(color);
    window.draw(middle);

    sf::CircleShape corner(r);
    corner.setFillColor(color);

    corner.setPosition(rect.left, rect.top);
    window.draw(corner);

    corner.setPosition(rect.left + rect.width - 2.f * r, rect.top);
    window.draw(corner);

    corner.setPosition(rect.left, rect.top + rect.height - 2.f * r);
    window.draw(corner);

    corner.setPosition(rect.left + rect.width - 2.f * r, rect.top + rect.height - 2.f * r);
    window.draw(corner);
}

void drawRoundedBox(sf::RenderWindow& window,
                    const sf::FloatRect& rect,
                    float radius,
                    float outlineThickness,
                    const sf::Color& fill,
                    const sf::Color& outline) {
    drawRoundedFill(window, rect, radius, outline);

    if (outlineThickness <= 0.01f) {
        return;
    }

    const float inset = outlineThickness;
    const sf::FloatRect inner(rect.left + inset,
                              rect.top + inset,
                              std::max(0.f, rect.width - inset * 2.f),
                              std::max(0.f, rect.height - inset * 2.f));
    drawRoundedFill(window, inner, std::max(0.f, radius - inset), fill);
}
}  // namespace

Button::Button(const std::string& label, const sf::Font& font) {
    background_.setSize(sf::Vector2f(180.f, 34.f));
    background_.setFillColor(sf::Color(35, 40, 52));
    background_.setOutlineThickness(1.f);
    background_.setOutlineColor(sf::Color(90, 90, 100));

    text_.setFont(font);
    text_.setString(label);
    text_.setCharacterSize(14);
    text_.setFillColor(sf::Color::White);

    alignText();
}

void Button::setLabel(const std::string& label) {
    text_.setString(label);
    alignText();
}

void Button::setSize(float width, float height) {
    background_.setSize(sf::Vector2f(width, height));
    alignText();
}

void Button::setPosition(float x, float y) {
    background_.setPosition(x, y);
    alignText();
}

void Button::setEnabled(bool enabled) {
    enabled_ = enabled;
}

bool Button::isEnabled() const {
    return enabled_;
}

void Button::setStyleRole(StyleRole role) {
    styleRole_ = role;
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
    if (!enabled_) {
        background.setFillColor(sf::Color(45, 50, 58));
        background.setOutlineColor(sf::Color(85, 85, 90));
        text.setFillColor(sf::Color(155, 155, 160));
    } else {
        sf::Color fill(35, 40, 52);
        sf::Color outline(90, 90, 100);
        sf::Color foreground = sf::Color::White;

        if (styleRole_ == StyleRole::Play) {
            fill = sf::Color(44, 120, 66);
            outline = sf::Color(95, 185, 122);
        } else if (styleRole_ == StyleRole::Danger) {
            fill = sf::Color(45, 40, 52);
            outline = sf::Color(98, 76, 76);
        }

        if (selected_) {
            if (styleRole_ == StyleRole::Algorithm) {
                fill = sf::Color(255, 171, 64);
                outline = sf::Color(190, 110, 34);
                foreground = sf::Color(25, 25, 25);
            } else if (styleRole_ == StyleRole::Danger) {
                fill = sf::Color(186, 64, 64);
                outline = sf::Color(220, 108, 108);
                foreground = sf::Color(250, 245, 245);
            } else {
                fill = sf::Color(255, 221, 92);
                outline = sf::Color(130, 106, 16);
                foreground = sf::Color(30, 30, 30);
            }
        }

        if (flashing) {
            fill = sf::Color(255, 221, 92);
            outline = sf::Color(130, 106, 16);
            foreground = sf::Color(30, 30, 30);
        }

        background.setFillColor(fill);
        background.setOutlineColor(outline);
        text.setFillColor(foreground);
    }

    const sf::FloatRect bounds(background.getPosition().x,
                               background.getPosition().y,
                               background.getSize().x,
                               background.getSize().y);
    drawRoundedBox(window,
                   bounds,
                   6.0f,
                   background.getOutlineThickness(),
                   background.getFillColor(),
                   background.getOutlineColor());
    window.draw(text);

    if (flashFrames_ > 0) {
        --flashFrames_;
    }
}

void Button::alignText() {
    const sf::FloatRect bounds = text_.getLocalBounds();
    const sf::Vector2f pos = background_.getPosition();
    const sf::Vector2f size = background_.getSize();
    text_.setPosition(pos.x + (size.x - bounds.width) * 0.5f - bounds.left,
                      pos.y + (size.y - bounds.height) * 0.5f - bounds.top);
}
