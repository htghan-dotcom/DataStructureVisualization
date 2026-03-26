#include "Slider.h"

#include <algorithm>

Slider::Slider(float x, float y, float width, float minValue, float maxValue, float initial, const sf::Font& font)
    : minValue_(minValue), maxValue_(maxValue), value_(initial) {
    track_.setPosition(x, y);
    track_.setSize(sf::Vector2f(width, 4.f));
    track_.setFillColor(sf::Color(110, 110, 120));

    knob_.setRadius(8.f);
    knob_.setFillColor(sf::Color(230, 230, 240));

    label_.setFont(font);
    label_.setCharacterSize(13);
    label_.setFillColor(sf::Color::White);

    updateKnobPosition();
}

bool Slider::contains(const sf::Vector2f& point) const {
    const auto bounds = track_.getGlobalBounds();
    sf::FloatRect expanded(bounds.left - 8.f, bounds.top - 10.f, bounds.width + 16.f, bounds.height + 20.f);
    return expanded.contains(point);
}

void Slider::setActive(bool active) {
    active_ = active;
}

void Slider::onMouseMoved(const sf::Vector2f& point) {
    if (!active_) {
        return;
    }

    float x = std::clamp(point.x, track_.getPosition().x, track_.getPosition().x + track_.getSize().x);
    float t = (x - track_.getPosition().x) / track_.getSize().x;
    value_ = minValue_ + t * (maxValue_ - minValue_);
    updateKnobPosition();
}

void Slider::setValue(float value) {
    value_ = std::clamp(value, minValue_, maxValue_);
    updateKnobPosition();
}

float Slider::getValue() const {
    return value_;
}

void Slider::draw(sf::RenderWindow& window) const {
    window.draw(track_);
    window.draw(knob_);
    window.draw(label_);
}

void Slider::updateKnobPosition() {
    float t = (value_ - minValue_) / (maxValue_ - minValue_);
    float x = track_.getPosition().x + t * track_.getSize().x;
    knob_.setPosition(x - knob_.getRadius(), track_.getPosition().y - 6.f);

    label_.setString("Speed: " + std::to_string(static_cast<int>(value_ * 10.f) / 10.0f));
    label_.setPosition(track_.getPosition().x, track_.getPosition().y + 10.f);
}
