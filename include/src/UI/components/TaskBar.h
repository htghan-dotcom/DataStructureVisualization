#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <optional>

class TaskBar {
public:
    TaskBar();
    
    // Cấu hình vùng vẽ và màu nền của TaskBar
    void setBounds(const sf::FloatRect& bounds);
    void setBackgroundColor(const sf::Color& color);
    
    // Thêm Icon (.png) vào thanh công cụ kèm theo một chuỗi ID để định danh hành động
    bool addIcon(const std::string& texturePath, const std::string& actionId);
    
    // Gọi hàm này mỗi frame để tạo hiệu ứng Hover
    void update(const sf::Vector2f& mousePos);
    
    // Trả về actionId của Icon nếu người dùng click vào nó, ngược lại trả về ""
    std::string getClickedActionId(const sf::Vector2f& mousePos) const;
    
    // Gọi hàm này để vẽ TaskBar lên cửa sổ
    void draw(sf::RenderWindow& window) const;

private:
    void updateLayout();

    struct IconItem {
        sf::Texture texture;
        std::optional<sf::Sprite> sprite;
        std::string actionId;
        bool isHovered = false;
    };

    sf::RectangleShape background_;
    sf::FloatRect bounds_;
    std::vector<IconItem> icons_;
    float iconSize_ = 32.0f;   // Kích thước chuẩn của mỗi icon
    float spacing_ = 24.0f;    // Khoảng cách giữa các icon
};