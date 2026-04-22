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
    background_.setFillColor(sf::Color(95, 131, 151));
    background_.setOutlineThickness(1.f);
    background_.setOutlineColor(sf::Color(84, 116, 134));

    text_.setFont(font);
    text_.setString(label);
    text_.setCharacterSize(14);
    text_.setFillColor(sf::Color(238, 244, 247));

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

sf::FloatRect Button::bounds() const {
    return background_.getGlobalBounds();
}

void Button::draw(sf::RenderWindow& window) {
    sf::RectangleShape background = background_;
    sf::Text text = text_;

    const bool flashing = flashFrames_ > 0;
    if (!enabled_) {
        background.setFillColor(sf::Color(202, 208, 214));
        background.setOutlineColor(sf::Color(176, 184, 191));
        text.setFillColor(sf::Color(146, 154, 162));
    } else {
        sf::Color fill(95, 131, 151);
        sf::Color outline(84, 116, 134);
        sf::Color foreground(238, 244, 247);

        if (styleRole_ == StyleRole::Play) {
            fill = sf::Color(110, 154, 91);
            outline = sf::Color(86, 126, 68);
            foreground = sf::Color(250, 250, 250);
        } else if (styleRole_ == StyleRole::Danger) {
            fill = sf::Color(217, 176, 176);
            outline = sf::Color(175, 118, 118);
            foreground = sf::Color(70, 34, 34);
        }

        if (selected_) {
            if (styleRole_ == StyleRole::Algorithm) {
                fill = sf::Color(245, 170, 70);
                outline = sf::Color(188, 124, 44);
                foreground = sf::Color(245, 247, 250);
            } else if (styleRole_ == StyleRole::Danger) {
                fill = sf::Color(197, 100, 100);
                outline = sf::Color(164, 74, 74);
                foreground = sf::Color(255, 247, 247);
            } else {
                fill = sf::Color(114, 149, 170);
                outline = sf::Color(84, 116, 134);
                foreground = sf::Color(245, 247, 250);
            }
        }

        if (flashing) {
            fill = sf::Color(114, 149, 170);
            outline = sf::Color(84, 116, 134);
            foreground = sf::Color(245, 247, 250);
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
                   std::clamp(bounds.height * 0.45f, 6.0f, 14.0f),
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
