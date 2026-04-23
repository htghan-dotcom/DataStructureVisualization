#include "Slider.h"

#include <algorithm>
#include <iomanip>
#include <sstream>

Slider::Slider(float x, float y, float width, float minValue, float maxValue, float initial, const sf::Font& font)
    : minValue_(minValue), maxValue_(maxValue), value_(initial) {
    track_.setPosition(x, y);
    track_.setSize(sf::Vector2f(width, 4.f));
    track_.setFillColor(sf::Color(186, 191, 196));

    knob_.setRadius(8.f);
    knob_.setFillColor(sf::Color(113, 145, 163));

    label_.setFont(font);
    label_.setCharacterSize(13);
    label_.setFillColor(sf::Color(54, 66, 76));

    updateKnobPosition();
}

bool Slider::contains(const sf::Vector2f& point) const {
    const auto bounds = track_.getGlobalBounds();
    sf::FloatRect expanded(bounds.left - 8.f, bounds.top - 10.f, bounds.width + 16.f, bounds.height + 20.f);
    return expanded.contains(point);
}

void Slider::setPosition(float x, float y) {
    track_.setPosition(x, y);
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
    progress.setFillColor(sf::Color(static_cast<sf::Uint8>(70 + 40 * t),
                                    static_cast<sf::Uint8>(140 + 70 * t),
                                    static_cast<sf::Uint8>(85 + 10 * t)));
    window.draw(progress);

    sf::CircleShape knob = knob_;
    knob.setFillColor(sf::Color(static_cast<sf::Uint8>(80 + 30 * t),
                                static_cast<sf::Uint8>(150 + 70 * t),
                                static_cast<sf::Uint8>(95 + 15 * t)));
    window.draw(knob);
    window.draw(label_);
}

void Slider::updateKnobPosition() {
    float t = (value_ - minValue_) / (maxValue_ - minValue_);
    float x = track_.getPosition().x + t * track_.getSize().x;
    knob_.setPosition(x - knob_.getRadius(), track_.getPosition().y - 6.f);

    std::ostringstream ss;
    ss << std::fixed << std::setprecision(1) << value_;
    label_.setString("Speed: " + ss.str());
    label_.setPosition(track_.getPosition().x, track_.getPosition().y + 10.f);
}
