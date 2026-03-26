#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <cmath>

class button {
private:
    sf::RectangleShape shape;
    sf::Text text;
public:
    button(float x, float y, float width, float height, const std::string& buttonText, sf::Font& font) : text(font) {
        shape.setPosition(sf::Vector2f(x, y)); 
        shape.setSize(sf::Vector2f(width, height));
        shape.setFillColor(sf::Color(70, 130, 180));
        shape.setOutlineThickness(2.f); 
        shape.setOutlineColor(sf::Color::White);
        
        // text.setFont(font); 
        text.setString(buttonText); 
        text.setCharacterSize(16);
        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2.0f, bounds.position.y + bounds.size.y / 2.0f));
        text.setPosition(sf::Vector2f(x + width/2.0f, y + height/2.0f));
    }

    bool isClicked(sf::Vector2i mousePos) {
        return shape.getGlobalBounds().contains(sf::Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)));
    }
    
    void draw(sf::RenderWindow& window) { 
        window.draw(shape); window.draw(text); 
    }
};