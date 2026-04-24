#include "Button.h"

#include <algorithm>
#include <cmath>

Button::Button(const std::string& label, const sf::Font& font)
 {
    text_.emplace(font, label, 15);
    
    radius_ = 8.0f; // Bo góc mềm mại hơn để hợp với nút to
    mTopLeft.setRadius(radius_);
    mTopRight.setRadius(radius_);
    mBottomLeft.setRadius(radius_);
    mBottomRight.setRadius(radius_);
}


void Button::setLabel(const std::string& label) {
    if (text_) text_->setString(label);
    alignText();
}

void Button::setSize(float width, float height) {
    bounds_.size.x = width;
    bounds_.size.y = height;
    
    mHorizRect.setSize(sf::Vector2f(width - radius_ * 2.0f, height));
    mVertRect.setSize(sf::Vector2f(width, height - radius_ * 2.0f));
    
    setPosition(bounds_.position.x, bounds_.position.y);
}

void Button::setPosition(float x, float y) {
    bounds_.position.x = x;
    bounds_.position.y = y;
    
    float w = bounds_.size.x;
    float h = bounds_.size.y;
    
    mTopLeft.setPosition({x, y});
    mTopRight.setPosition({x + w - radius_ * 2.0f, y});
    mBottomLeft.setPosition({x, y + h - radius_ * 2.0f});
    mBottomRight.setPosition({x + w - radius_ * 2.0f, y + h - radius_ * 2.0f});

    mHorizRect.setPosition({x + radius_, y});
    mVertRect.setPosition({x, y + radius_});
    
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
    sf::Color baseColor;
    sf::Color textColor = sf::Color::White;
    std::string lbl = text_ ? text_->getString().toAnsiString() : "";

    if (!enabled_) {
        if (styleRole_ == StyleRole::IconOnly) {
            baseColor = sf::Color::Transparent;
        } else if (lbl.find("Step") != std::string::npos) {
            baseColor = sf::Color(230, 234, 238); // Xám cực nhạt cho nút disabled
            textColor = sf::Color(160, 164, 168);
        } else {
            baseColor = sf::Color(202, 208, 214);
            textColor = sf::Color(120, 124, 128);
        }
    } else {
        switch (styleRole_) {
            case StyleRole::Play: baseColor = sf::Color(90, 150, 44); break;
            case StyleRole::Danger: baseColor = sf::Color(211, 73, 98); break;
            case StyleRole::Algorithm: baseColor = sf::Color(65, 80, 95); textColor = sf::Color(180, 195, 210); break;
            case StyleRole::IconOnly: baseColor = sf::Color::Transparent; break;
            default: 
                if (lbl.find("Add Node") != std::string::npos || lbl.find("Edit Edges") != std::string::npos) {
                    baseColor = sf::Color(65, 125, 195); // Xanh dương cho Edit thao tác
                } else if (lbl.find("New") != std::string::npos || lbl.find("Undo") != std::string::npos || lbl.find("Random") != std::string::npos || lbl.find("View") != std::string::npos) {
                    baseColor = sf::Color(105, 115, 125); // Xám trung tính cho System
                } else if (lbl.find("BUILD") != std::string::npos) {
                    baseColor = sf::Color(46, 175, 100); // Xanh lục Primary thu hút mắt
                } else if (lbl.find("Step") != std::string::npos) {
                    baseColor = sf::Color(240, 244, 248); // Màu sáng tạo background nổi bật
                    textColor = sf::Color(40, 48, 56); // Chữ tối màu
                } else {
                    baseColor = sf::Color(95, 131, 151); // Mặc định
                }
                break;
        }

        if (selected_) {
            if (styleRole_ == StyleRole::Algorithm) {
                baseColor = sf::Color(241, 186, 88);
            } else if (styleRole_ == StyleRole::Danger || lbl.find("Delete") != std::string::npos) {
                baseColor = sf::Color(197, 100, 100);
            } else if (styleRole_ == StyleRole::IconOnly) {
                baseColor = sf::Color(0, 0, 0, 40);
            } else {
                baseColor = sf::Color(114, 149, 170);
            }
            textColor = sf::Color(20, 20, 20);
        } else if (isHovered) {
            if (styleRole_ == StyleRole::IconOnly) {
                baseColor = sf::Color(0, 0, 0, 20); // Subtle hover effect
            } else if (lbl.find("Step") != std::string::npos) {
                baseColor = sf::Color(220, 226, 232); // Hover đậm hơn chút
            } else {
                baseColor = sf::Color(static_cast<std::uint8_t>(std::max(0, baseColor.r - 20)), 
                                      static_cast<std::uint8_t>(std::max(0, baseColor.g - 20)), 
                                      static_cast<std::uint8_t>(std::max(0, baseColor.b - 20)));
            }
        }

        if (flashFrames_ > 0) {
            baseColor = sf::Color(255, 255, 255, 200);
            textColor = sf::Color::Black;
            if (text_) text_->setScale({0.92f, 0.92f}); // Tạo cảm giác lún click
            flashFrames_--;
        } else {
            if (text_) text_->setScale({1.0f, 1.0f});
        }
    }

    if (lbl.find("Step") != std::string::npos) {
        radius_ = std::min(bounds_.size.y * 0.5f, bounds_.size.x * 0.5f);
    } else {
        radius_ = 8.0f;
    }

    mTopLeft.setRadius(radius_);
    mTopRight.setRadius(radius_);
    mBottomLeft.setRadius(radius_);
    mBottomRight.setRadius(radius_);

    float x = bounds_.position.x;
    float y = bounds_.position.y;
    float w = bounds_.size.x;
    float h = bounds_.size.y;
    
    mTopLeft.setPosition({x, y});
    mTopRight.setPosition({x + w - radius_ * 2.0f, y});
    mBottomLeft.setPosition({x, y + h - radius_ * 2.0f});
    mBottomRight.setPosition({x + w - radius_ * 2.0f, y + h - radius_ * 2.0f});

    mHorizRect.setSize(sf::Vector2f(w - radius_ * 2.0f, h));
    mVertRect.setSize(sf::Vector2f(w, h - radius_ * 2.0f));
    mHorizRect.setPosition({x + radius_, y});
    mVertRect.setPosition({x, y + radius_});

    mTopLeft.setFillColor(baseColor);
    mTopRight.setFillColor(baseColor);
    mBottomLeft.setFillColor(baseColor);
    mBottomRight.setFillColor(baseColor);
    mHorizRect.setFillColor(baseColor);
    mVertRect.setFillColor(baseColor);
    if (text_) text_->setFillColor(textColor);
}

void Button::draw(sf::RenderWindow& window) {
    sf::Vector2i mousePosi = sf::Mouse::getPosition(window);
    sf::Vector2f mousePos = window.mapPixelToCoords(mousePosi);
    bool isHovered = enabled_ && bounds_.contains(mousePos);
    
    updateColors(isHovered);
    
    window.draw(mTopLeft);
    window.draw(mTopRight);
    window.draw(mBottomLeft);
    window.draw(mBottomRight);
    window.draw(mHorizRect);
    window.draw(mVertRect);
    
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
        iconSprite.setColor(text_ ? text_->getFillColor() : sf::Color::White); // Đổi màu giống chữ (sáng/tối/mờ)
        
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
