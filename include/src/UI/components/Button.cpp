#include "Button.h"
#include "ThemeManager.h"

#include <algorithm>
#include <cmath>

// Helper function from leader's code to create rounded rectangles
namespace {
    sf::ConvexShape createRoundedRect(sf::Vector2f size, float radius) {
        radius = std::min({radius, size.x / 2.f, size.y / 2.f});
        int pointsPerCorner = 15;
        sf::ConvexShape shape(pointsPerCorner * 4);
        int index = 0;
        const float PI = 3.14159265358979323846f;
        
        for(int i = 0; i < pointsPerCorner; ++i){
            float angle = i * (PI / 2) / (pointsPerCorner - 1);
            shape.setPoint(index++, sf::Vector2f(size.x - radius + radius * std::sin(angle), radius - radius * std::cos(angle)));
        }
        for(int i = 0; i < pointsPerCorner; ++i){
            float angle = PI / 2 + i * (PI / 2) / (pointsPerCorner - 1);
            shape.setPoint(index++, sf::Vector2f(size.x - radius + radius * std::sin(angle), size.y - radius - radius * std::cos(angle)));
        }
        for(int i = 0; i < pointsPerCorner; ++i){
            float angle = PI + i * (PI / 2) / (pointsPerCorner - 1);
            shape.setPoint(index++, sf::Vector2f(radius + radius * std::sin(angle), size.y - radius - radius * std::cos(angle)));
        }
        for(int i = 0; i < pointsPerCorner; ++i){
            float angle = 3 * PI / 2 + i * (PI / 2) / (pointsPerCorner - 1);
            shape.setPoint(index++, sf::Vector2f(radius + radius * std::sin(angle), radius - radius * std::cos(angle)));
        }
        return shape;
    }
}

Button::Button(const std::string& label, const sf::Font& font)
 {
    text_.emplace(font, label, 15);
}


void Button::setLabel(const std::string& label) {
    if (text_) text_->setString(label);
    alignText();
}

void Button::setSize(float width, float height) {
    bounds_.size.x = width;
    bounds_.size.y = height;
    setPosition(bounds_.position.x, bounds_.position.y);
}

void Button::setPosition(float x, float y) {
    bounds_.position.x = x;
    bounds_.position.y = y;
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

void Button::setIcon(const sf::Texture* texture) {
    iconTexture_ = texture;
    alignText(); // Sắp xếp lại chữ và ảnh
}

bool Button::contains(const sf::Vector2f& point) const {
    return enabled_ && bounds_.contains(point);
}

sf::FloatRect Button::bounds() const {
    return bounds_;
}

void Button::updateColors(bool isHovered) {
    baseColor_ = ThemeManager::current.secondary;
    textColor_ = ThemeManager::current.textColor;

    if (!enabled_) {
        baseColor_ = sf::Color(ThemeManager::current.secondary.r, ThemeManager::current.secondary.g, ThemeManager::current.secondary.b, 100);
        textColor_ = sf::Color(ThemeManager::current.textColor.r, ThemeManager::current.textColor.g, ThemeManager::current.textColor.b, 100);
    } else {
        switch (styleRole_) {
            case StyleRole::Primary:
                baseColor_ = ThemeManager::current.primary;
                textColor_ = ThemeManager::current.bg;
                break;
            case StyleRole::Danger:
                baseColor_ = sf::Color(201, 63, 88); // A consistent danger color
                textColor_ = sf::Color::White;
                break;
            case StyleRole::IconOnly:
            case StyleRole::Pill: // Pill buttons in leader's code have a different bg
                baseColor_ = ThemeManager::current.bg;
                textColor_ = ThemeManager::current.textColor;
                break;
            case StyleRole::Default:
            default:
                baseColor_ = ThemeManager::current.secondary;
                textColor_ = ThemeManager::current.textColor;
                break;
        }

        if (selected_) {
            baseColor_ = ThemeManager::current.primary;
            textColor_ = ThemeManager::current.bg;
        } else if (isHovered) {
            // Darken the color slightly on hover
            baseColor_.r = static_cast<uint8_t>(std::max(0, baseColor_.r - 25));
            baseColor_.g = static_cast<uint8_t>(std::max(0, baseColor_.g - 25));
            baseColor_.b = static_cast<uint8_t>(std::max(0, baseColor_.b - 25));
        }

        if (flashFrames_ > 0) {
            baseColor_ = sf::Color::White;
            textColor_ = sf::Color::Black;
            if (text_) text_->setScale({0.92f, 0.92f}); // Tạo cảm giác lún click
            flashFrames_--;
        } else {
            if (text_) text_->setScale({1.0f, 1.0f});
        }
    }
    if (text_) text_->setFillColor(textColor_);
}

void Button::draw(sf::RenderWindow& window) {
    sf::Vector2i mousePosi = sf::Mouse::getPosition(window);
    sf::Vector2f mousePos = window.mapPixelToCoords(mousePosi);
    bool isHovered = enabled_ && bounds_.contains(mousePos);
    
    updateColors(isHovered);
    
    float radius = (styleRole_ == StyleRole::Pill) ? bounds_.size.y / 2.f : 12.f;
    sf::ConvexShape buttonShape = createRoundedRect(bounds_.size, radius);
    buttonShape.setPosition(bounds_.position);
    buttonShape.setFillColor(baseColor_);

    window.draw(buttonShape);
    
    // Vẽ text
    if (text_) window.draw(*text_);

    // Nếu có icon, vẽ icon nằm bên phải text
    if (iconTexture_ && iconTexture_->getSize().x > 0 && iconTexture_->getSize().y > 0) {
        sf::Sprite iconSprite(*iconTexture_);
        float iconH = bounds_.size.y * 0.5f;
        if (iconH <= 0.f) iconH = 12.f;
        
        float scale = iconH / static_cast<float>(iconTexture_->getSize().y);
        float scaleMod = text_ ? text_->getScale().x : 1.0f; // scale lún khi click
        
        iconSprite.setScale({scale * scaleMod, scale * scaleMod});
        
        // Tính vị trí X của icon nằm ngay bên phải text
        float textRealWidth = text_ ? text_->getLocalBounds().size.x * scaleMod : 0.0f;
        float spacing = 8.0f * scaleMod;
        float iconX = (text_ ? text_->getPosition().x : bounds_.position.x) + textRealWidth + spacing;
        
        // Căn giữa theo trục Y
        float scaledIconH = iconH * scaleMod;
        float scaledIconY = bounds_.position.y + (bounds_.size.y - scaledIconH) / 2.0f;
        
        iconSprite.setPosition({iconX, scaledIconY});
        iconSprite.setColor(textColor_); // Đổi màu giống chữ (sáng/tối/mờ)
        
        window.draw(iconSprite);
    }
}

void Button::alignText() {
    if (!text_) return;
    sf::FloatRect textBounds = text_->getLocalBounds();
    if (iconTexture_ && iconTexture_->getSize().x > 0 && iconTexture_->getSize().y > 0) {
        float iconH = bounds_.size.y * 0.5f;
        if (iconH <= 0.f) iconH = 12.f;
        float scale = iconH / static_cast<float>(iconTexture_->getSize().y);
        float iconW = static_cast<float>(iconTexture_->getSize().x) * scale;
        float spacing = 8.0f;
        
        float totalW = textBounds.size.x + spacing + iconW;
        float startX = bounds_.position.x + (bounds_.size.x - totalW) / 2.0f;
        
        // Neo text ở mép trái của tổng chiều dài khối (Text + Icon)
        // Dùng std::round để text luôn nằm trên tọa độ pixel chẵn (Khắc phục viền mờ trong SFML)
        text_->setOrigin({textBounds.position.x, std::round(textBounds.position.y + textBounds.size.y / 2.0f)});
        text_->setPosition({std::round(startX), std::round(bounds_.position.y + bounds_.size.y / 2.0f)});
    } else {
        // Căn giữa mặc định nếu không có icon
        text_->setOrigin({std::round(textBounds.position.x + textBounds.size.x / 2.0f),
                         std::round(textBounds.position.y + textBounds.size.y / 2.0f)});
        text_->setPosition({std::round(bounds_.position.x + bounds_.size.x / 2.0f),
                           std::round(bounds_.position.y + bounds_.size.y / 2.0f)});
    }
}
