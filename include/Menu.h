#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <optional>

struct MenuCard{
    std::string number;
    std::string name;
    sf::Color bgColor;
    
    float currentWidth;
    
    sf::RectangleShape bgShape;
    sf::Text numText;
    sf::Text nameText;
    sf::Text viewMoreText;

    float textYPos;
    float elementsAlpha;

    MenuCard(const std::string& num, const std::string& n, sf::Color c, float w, float h, const sf::Font& font)
        : number(num), name(n), bgColor(c), currentWidth(w),
          numText(font), nameText(font), viewMoreText(font),
          textYPos(h / 2.f - 40.f), elementsAlpha(0.f)
    {
        bgShape.setSize({currentWidth, h});
        bgShape.setFillColor(bgColor);
        
        numText.setString(number);
        numText.setCharacterSize(16);
        numText.setFillColor(sf::Color::Black);
        
        nameText.setString(name);
        nameText.setCharacterSize(38);
        nameText.setFillColor(sf::Color::Black);
        nameText.setStyle(sf::Text::Bold);
        
        viewMoreText.setString("View more >");
        viewMoreText.setCharacterSize(16);
        viewMoreText.setFillColor(sf::Color::Black);
    }

    MenuCard(MenuCard&&) noexcept = default;
    MenuCard& operator=(MenuCard&&) noexcept = default;
    MenuCard(const MenuCard&) = delete;
    MenuCard& operator=(const MenuCard&) = delete;
};

class MainMenu{
private:
    std::vector<MenuCard> mCards;
    sf::Font mFont;
    float mWindowWidth;
    float mWindowHeight;
    int mActiveIdx;

public:
    MainMenu(float w, float h);
    std::optional<int> update(const sf::RenderWindow& window);
    void draw(sf::RenderTarget& target);
    
    sf::FloatRect getCardBounds(int index) const{
        return mCards[index].bgShape.getGlobalBounds();
    }
    
    sf::Color getCardColor(int index) const{
        return mCards[index].bgColor;
    }
    
    int getActiveDS() const{
        return mActiveIdx;
    }

    sf::FloatRect getThumbnailBounds(int index) const{
        float cardX = mCards[index].bgShape.getPosition().x;
        return sf::FloatRect({cardX + 20.f, 60.f}, {mCards[index].currentWidth - 40.f, mWindowHeight * 0.5f});
    }
};
