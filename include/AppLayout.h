#pragma once
#include <SFML/Graphics.hpp>
#include "GUI.h"
#include <iostream>

class AppLayout {
private:
    sf::RectangleShape mFooter;
    sf::Text mHeaderText;
    sf::RectangleShape mDescriptionBox;
    sf::Text mDescriptionText;
    sf::Font mFontBold;
    sf::Font mFontRegular;
    sf::Texture mHomeTex;

public:
    ImageButton mHomeBtn;
    bool mGoHome = false;

    AppLayout();
    void update(sf::Vector2i mousePos);
    void draw(sf::RenderWindow& window);
    void setDescription(const std::string& text);
};
