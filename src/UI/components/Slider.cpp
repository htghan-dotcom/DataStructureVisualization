#include "Slider.h"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <cstdint>

Slider::Slider(float x, float y, float width, float minValue, float maxValue, float initial, const sf::Font& font)
    : minValue_(minValue), maxValue_(maxValue), value_(initial) {
    track_.setPosition({x, y});
    track_.setSize({width, 4.f});
    track_.setFillColor(sf::Color(186, 191, 196));

    knob_.setRadius(8.f);
    knob_.setFillColor(sf::Color(113, 145, 163));

    label_.emplace(font, "", 16);
    label_->setFillColor(sf::Color(20, 24, 28));

    updateKnobPosition();
}

bool Slider::contains(const sf::Vector2f& point) const {
    const auto bounds = track_.getGlobalBounds();
    sf::FloatRect expanded({bounds.position.x - 8.f, bounds.position.y - 10.f}, {bounds.size.x + 16.f, bounds.size.y + 20.f});
    return expanded.contains(point);
}

void Slider::setPosition(float x, float y) {
    track_.setPosition({x, y});
    updateKnobPosition();
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

    const float t = std::clamp((value_ - minValue_) / (maxValue_ - minValue_), 0.0f, 1.0f);
    sf::RectangleShape progress(sf::Vector2f(track_.getSize().x * t, track_.getSize().y));
    progress.setPosition(track_.getPosition());
    progress.setFillColor(sf::Color(static_cast<std::uint8_t>(70 + 40 * t),
                                    static_cast<std::uint8_t>(140 + 70 * t),
                                    static_cast<std::uint8_t>(85 + 10 * t)));
    window.draw(progress);

    sf::CircleShape knob = knob_;
    knob.setFillColor(sf::Color(static_cast<std::uint8_t>(80 + 30 * t),
                                static_cast<std::uint8_t>(150 + 70 * t),
                                static_cast<std::uint8_t>(95 + 15 * t)));
    window.draw(knob);
    if (label_) window.draw(*label_);
}

void Slider::updateKnobPosition() {
    float t = (value_ - minValue_) / (maxValue_ - minValue_);
    float x = track_.getPosition().x + t * track_.getSize().x;
    knob_.setPosition({x - knob_.getRadius(), track_.getPosition().y - 6.f});

    std::ostringstream ss;
    ss << std::fixed << std::setprecision(1) << value_;
    if (label_) {
        label_->setString("Speed:  " + ss.str() + "x");
        sf::FloatRect textBounds = label_->getLocalBounds();
        label_->setPosition({std::roundf(track_.getPosition().x - textBounds.size.x - 16.f), 
                             std::roundf(track_.getPosition().y - textBounds.size.y * 0.5f - 4.f)});
    }
}
