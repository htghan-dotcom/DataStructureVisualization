#include "Menu.h"
#include "ThemeManager.h"
#include "config/Common.h"

using namespace std;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

sf::ConvexShape createPillShape(float w, float h){
    float radius = h / 2.f;
    int pointsPerCorner = 15;
    sf::ConvexShape shape(pointsPerCorner * 4);
    int index = 0;
    
    for (int i = 0; i < pointsPerCorner; ++i){
        float angle = i * (M_PI / 2) / (pointsPerCorner - 1);
        shape.setPoint(index++, sf::Vector2f(w - radius + radius * sin(angle), radius - radius * cos(angle)));
    }
    for (int i = 0; i < pointsPerCorner; ++i){
        float angle = M_PI / 2 + i * (M_PI / 2) / (pointsPerCorner - 1);
        shape.setPoint(index++, sf::Vector2f(w - radius + radius * sin(angle), h - radius - radius * cos(angle)));
    }
    for (int i = 0; i < pointsPerCorner; ++i){
        float angle = M_PI + i * (M_PI / 2) / (pointsPerCorner - 1);
        shape.setPoint(index++, sf::Vector2f(radius + radius * sin(angle), h - radius - radius * cos(angle)));
    }

    for(int i = 0; i < pointsPerCorner; ++i){
        float angle = 3 * M_PI / 2 + i * (M_PI / 2) / (pointsPerCorner - 1);
        shape.setPoint(index++, sf::Vector2f(radius + radius * sin(angle), radius - radius * cos(angle)));
    }
    return shape;
}

MainMenu::MainMenu(float w, float h)
    : mWindowWidth(w),
      mWindowHeight(h),
      mActiveIdx(-1)
{
    if (!mFont.openFromFile("assets/fonts/Inter-Bold.ttf")){cerr << "Could not load font" << endl;}
    
    vector<string> nums = {"01", "02", "03", "04"};
    vector<string> names = {"Doubly\nLinked List", "Hash\nTable", "Red - Black\nTree", "Graph"};
    
    vector<sf::Color> colors = {
        sf::Color(246, 247, 240),
        sf::Color(246, 247, 240),
        sf::Color(246, 247, 240),
        sf::Color(246, 247, 240),
    };
    
    float defWidth = mWindowWidth / static_cast<float>(names.size());
    for (size_t i = 0; i < names.size(); i++){
        mCards.emplace_back(nums[i], names[i], colors[i], defWidth, mWindowHeight, mFont);
    }
    
    if (!mExitTex.loadFromFile("assets/images/exitButton.png")){cerr << "Cannot load exitButton.png" << endl;}
    mExitTex.setSmooth(true); 
    (void)mExitTex.generateMipmap();
    
    mExitBtn.setup(mExitTex, 43.f, 62.f, 48.f, 48.f);
    mExitBtn.setCallback([this](){
        mGoBack = true;
    });
}

std::optional<int> MainMenu::update(const sf::RenderWindow& window){
    sf::Vector2f worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    sf::Vector2i mousePos(static_cast<int>(worldPos.x), static_cast<int>(worldPos.y));
    
    mExitBtn.update(mousePos);

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
        float btnW = 230.f;
        float btnH = 67.f;
        float btnX = mActiveIdx * mCards[mActiveIdx].currentWidth + 65.f;
        float btnY = 842.f;

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
        sf::Color baseBg = ThemeManager::current.screenBg;
        sf::Color targetBg = ThemeManager::current.bg;
        
        uint8_t r = static_cast<uint8_t>(baseBg.r + (targetBg.r - baseBg.r) * t);
        uint8_t g = static_cast<uint8_t>(baseBg.g + (targetBg.g - baseBg.g) * t);
        uint8_t b = static_cast<uint8_t>(baseBg.b + (targetBg.b - baseBg.b) * t);
        
        card.bgShape.setFillColor(sf::Color(r, g, b));
        target.draw(card.bgShape);
        
        float paddingX = 40.f;

        card.numText.setPosition(sf::Vector2f(drawX + paddingX, card.textYPos - 30.f));
        card.nameText.setPosition(sf::Vector2f(drawX + paddingX, card.textYPos));
        
        card.numText.setFillColor(ThemeManager::current.primary);
        card.nameText.setFillColor(ThemeManager::current.textColor);
        
        target.draw(card.numText);
        target.draw(card.nameText);

        if (card.elementsAlpha > 5.f){
            float btnW = 230.f;
            float btnH = 67.f;
            float btnX = drawX + 65.f;
            float btnY = 842.f;
            
            sf::Color pColor = ThemeManager::current.primary;
            
            sf::ConvexShape pillBtn = createPillShape(btnW, btnH);
            pillBtn.setPosition(sf::Vector2f(btnX, btnY));
            pillBtn.setFillColor(pColor);
            target.draw(pillBtn);
            
            sf::FloatRect viewBounds = card.viewMoreText.getLocalBounds();
            
            sf::Color textCol = ThemeManager::current.bg;
            textCol.a = static_cast<uint8_t>(card.elementsAlpha);
            card.viewMoreText.setFillColor(textCol);
            
            card.viewMoreText.setPosition(sf::Vector2f(
                btnX + (btnW - viewBounds.size.x) / 2.f,
                btnY + (btnH - viewBounds.size.y) / 2.f - 5.f
            ));
            target.draw(card.viewMoreText);
        }
        
        drawX += card.currentWidth;
    }
    
    mExitBtn.mSprite->setColor(ThemeManager::current.primary);
    mExitBtn.draw(target);
}
