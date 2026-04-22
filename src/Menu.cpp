#include "Menu.h"
#include "Common.h"

MainMenu::MainMenu(float w, float h)
: mWindowWidth(w), mWindowHeight(h), mActiveIdx(-1)
{
    if (!mFont.openFromFile("/System/Library/Fonts/Helvetica.ttc")){
        cerr << "Could not load font" << endl;
    }
    
    vector<string> nums = {"01", "02", "03", "04"};
    vector<string> names = {"Doubly\nLinked List", "Hash\nTable", "Red - Black\nTree", "Graph"};
    
    vector<sf::Color> colors = {
        sf::Color(226, 239, 255),
        sf::Color(234, 252, 235),
        sf::Color(246, 247, 240),
        sf::Color(251, 240, 245)
    };
    
    float defWidth = mWindowWidth / static_cast<float>(names.size());
    for (size_t i = 0; i < names.size(); i++){
        mCards.emplace_back(nums[i], names[i], colors[i], defWidth, mWindowHeight, mFont);
    }
}

std::optional<int> MainMenu::update(const sf::RenderWindow& window){
    sf::Vector2f worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    sf::Vector2i mousePos(static_cast<int>(worldPos.x), static_cast<int>(worldPos.y));

    float currentX = 0;
    int hoveredIdx = -1;

    for (size_t i = 0; i < mCards.size(); i++){
        if (mousePos.x >= currentX and mousePos.x <= currentX + mCards[i].currentWidth and mousePos.y >= 0 and mousePos.y <= mWindowHeight){
            hoveredIdx = static_cast<int>(i);
        }
        currentX += mCards[i].currentWidth;
    }
    
    mActiveIdx = hoveredIdx;

    optional<int> clickedDS = nullopt;
    if (mActiveIdx != -1 and sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
        float paddingX = 40.f;
        float btnW = 140.f;
        float btnH = 40.f;
        float btnX = mActiveIdx * mCards[mActiveIdx].currentWidth + paddingX;
        float btnY = mCards[mActiveIdx].textYPos + mCards[mActiveIdx].nameText.getLocalBounds().size.y + 40.f;

        sf::FloatRect btnBounds(sf::Vector2f(btnX, btnY), sf::Vector2f(btnW, btnH));
        if (btnBounds.contains(sf::Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))){
            clickedDS = mActiveIdx;
        }
    }

    for (size_t i = 0; i < mCards.size(); i++){
        if (static_cast<int>(i) == mActiveIdx){
            float targetY = mWindowHeight * 0.65f;
            mCards[i].textYPos += (targetY - mCards[i].textYPos) * 0.15f;
            mCards[i].elementsAlpha = min(255.f, mCards[i].elementsAlpha + 15.f);
        } else {
            float targetY = mWindowHeight * 0.45f;
            mCards[i].textYPos += (targetY - mCards[i].textYPos) * 0.15f;
            mCards[i].elementsAlpha = max(0.f, mCards[i].elementsAlpha - 15.f);
        }
    }
    
    return clickedDS;
}

void MainMenu::draw(sf::RenderTarget& target){
    float drawX = 0.f;
    
    for (size_t i = 0; i < mCards.size(); i++){
        auto& card = mCards[i];
        
        card.bgShape.setPosition(sf::Vector2f(drawX, 0.f));
        
        float t = card.elementsAlpha / 255.f;
        
        uint8_t r = static_cast<uint8_t>(255 + (card.bgColor.r - 255) * t);
        uint8_t g = static_cast<uint8_t>(255 + (card.bgColor.g - 255) * t);
        uint8_t b = static_cast<uint8_t>(255 + (card.bgColor.b - 255) * t);
        
        card.bgShape.setFillColor(sf::Color(r, g, b));
        target.draw(card.bgShape);
        
        float paddingX = 40.f;

        card.numText.setPosition(sf::Vector2f(drawX + paddingX, card.textYPos - 30.f));
        card.nameText.setPosition(sf::Vector2f(drawX + paddingX, card.textYPos));
        
        target.draw(card.numText);
        target.draw(card.nameText);

        if (card.elementsAlpha > 5.f){
            float btnW = 140.f;
            float btnH = 40.f;
            float radius = btnH / 2.f;
            float btnX = drawX + paddingX;
            float btnY = card.textYPos + card.nameText.getLocalBounds().size.y + 40.f;
            
            sf::Color btnColor(220, 220, 220, static_cast<uint8_t>(card.elementsAlpha));
            
            sf::CircleShape leftCircle(radius);
            leftCircle.setPosition(sf::Vector2f(btnX, btnY));
            leftCircle.setFillColor(btnColor);

            sf::CircleShape rightCircle(radius);
            rightCircle.setPosition(sf::Vector2f(btnX + btnW - btnH, btnY));
            rightCircle.setFillColor(btnColor);

            sf::RectangleShape centerRect(sf::Vector2f(btnW - btnH, btnH));
            centerRect.setPosition(sf::Vector2f(btnX + radius, btnY));
            centerRect.setFillColor(btnColor);
                   
            target.draw(leftCircle);
            target.draw(rightCircle);
            target.draw(centerRect);
            
            sf::FloatRect viewBounds = card.viewMoreText.getLocalBounds();
            card.viewMoreText.setFillColor(sf::Color(30, 30, 30, static_cast<uint8_t>(card.elementsAlpha)));
            card.viewMoreText.setPosition(sf::Vector2f(
                btnX + (btnW - viewBounds.size.x) / 2.f,
                btnY + (btnH - viewBounds.size.y) / 2.f - 4.f
            ));
            target.draw(card.viewMoreText);
        }
        
        drawX += card.currentWidth;
    }
}
