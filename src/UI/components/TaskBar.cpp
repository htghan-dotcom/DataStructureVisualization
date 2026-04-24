#include "TaskBar.h"
#include <iostream>

TaskBar::TaskBar() {
    // Nền tối bán trong suốt (Kính mờ)
    background_.setFillColor(sf::Color(30, 36, 44, 230)); 
}

void TaskBar::setBounds(const sf::FloatRect& bounds) {
    bounds_ = bounds;
    background_.setPosition({bounds_.position.x, bounds_.position.y});
    background_.setSize({bounds_.size.x, bounds_.size.y});
    updateLayout(); // Sắp xếp lại các icon khi vùng nền thay đổi
}

void TaskBar::setBackgroundColor(const sf::Color& color) {
    background_.setFillColor(color);
}

bool TaskBar::addIcon(const std::string& texturePath, const std::string& actionId) {
    IconItem item;
    if (!item.texture.loadFromFile(texturePath)) {
        std::cerr << "Failed to load TaskBar icon: " << texturePath << "\n";
        return false;
    }
    
    // Cực kỳ quan trọng: Bật Smooth để icon ảnh thu nhỏ không bị răng cưa
    item.texture.setSmooth(true);
    item.sprite.emplace(item.texture);
    item.actionId = actionId;
    
    icons_.push_back(std::move(item));
    updateLayout();
    return true;
}

void TaskBar::updateLayout() {
    if (icons_.empty()) return;

    // Tính toán tổng chiều rộng để căn giữa các icon trên TaskBar
    float totalWidth = icons_.size() * iconSize_ + (icons_.size() - 1) * spacing_;
    
    float startX = bounds_.position.x + (bounds_.size.x - totalWidth) / 2.0f;
    float centerY = bounds_.position.y + bounds_.size.y / 2.0f;

    for (size_t i = 0; i < icons_.size(); ++i) {
        icons_[i].sprite.emplace(icons_[i].texture); // Rebind texture reference in case vector resized
        // Đặt tâm điểm (origin) ở giữa tấm ảnh để khi scale lên sẽ không bị lệch
        sf::Vector2f texSize(static_cast<float>(icons_[i].texture.getSize().x),
                             static_cast<float>(icons_[i].texture.getSize().y));
        icons_[i].sprite->setOrigin({texSize.x / 2.0f, texSize.y / 2.0f});
        
        // Tính vị trí
        float x = startX + i * (iconSize_ + spacing_) + iconSize_ / 2.0f;
        icons_[i].sprite->setPosition({x, centerY});
    }
}

void TaskBar::update(const sf::Vector2f& mousePos) {
    for (auto& item : icons_) {
        sf::Vector2f texSize(static_cast<float>(item.texture.getSize().x),
                             static_cast<float>(item.texture.getSize().y));
        item.isHovered = item.sprite->getGlobalBounds().contains(mousePos);
        
        // Hover animation logic
        float currentTargetScale = iconSize_ / texSize.x;
        if (item.isHovered) {
            currentTargetScale *= 1.25f; // Phóng to 25% khi rê chuột
            item.sprite->setColor(sf::Color(255, 255, 255, 255)); // Sáng hoàn toàn
        } else {
            item.sprite->setColor(sf::Color(200, 205, 210, 210)); // Mờ đi một chút
        }
        item.sprite->setScale({currentTargetScale, currentTargetScale});
    }
}

std::string TaskBar::getClickedActionId(const sf::Vector2f& mousePos) const {
    for (const auto& item : icons_) {
        if (item.sprite->getGlobalBounds().contains(mousePos)) return item.actionId;
    }
    return "";
}

void TaskBar::draw(sf::RenderWindow& window) const {
    window.draw(background_);
    for (const auto& item : icons_) window.draw(*item.sprite);
}