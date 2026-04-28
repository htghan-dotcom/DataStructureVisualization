#include "Menu.h"
#include "ThemeManager.h"
#include "Common.h"

MainMenu::MainMenu(float w, float h)
    : mWindowWidth(w),
      mWindowHeight(h),
      mActiveIdx(-1)
{
    if (!mFont.openFromFile("assets/fonts/Inter-Bold.ttf")){cerr << "Could not load font" << endl;}
    
    vector<string> nums = {"01", "02", "03", "04"};
    vector<string> names = {"Doubly\nLinked List", "Hash\nTable", "Red - Black\nTree", "Minimum\nSpanning\nTree"};
    
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
    mExitTex.setSmooth(true); mExitTex.generateMipmap();
    
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
        float interp = 0.15f;
        float alphaDelta = 15.f;
        if (i == 3) {
            // Make the MST preview appear/disappear faster than other cards
            interp = 0.35f;
            alphaDelta = 40.f;
        }

        if (static_cast<int>(i) == mActiveIdx){
            float targetY = mWindowHeight * 0.65f;
            mCards[i].textYPos += (targetY - mCards[i].textYPos) * interp;
            mCards[i].elementsAlpha = min(255.f, mCards[i].elementsAlpha + alphaDelta);
        } else {
            float targetY = mWindowHeight * 0.45f;
            mCards[i].textYPos += (targetY - mCards[i].textYPos) * interp;
            mCards[i].elementsAlpha = max(0.f, mCards[i].elementsAlpha - alphaDelta);
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

        // Draw MST thumbnail preview directly inside the 4th menu card.
        if (i == 3 && card.elementsAlpha > 0.5f){
            float frameX = drawX + 34.f;
            float frameY = 250.f;
            float frameW = card.currentWidth - 68.f;
            float frameH = 290.f;

            uint8_t previewAlpha = static_cast<uint8_t>(std::clamp(card.elementsAlpha, 0.f, 255.f));

            sf::ConvexShape frame = createRoundedRect({frameW, frameH}, 24.f);
            frame.setPosition(sf::Vector2f(frameX, frameY));
            frame.setFillColor(sf::Color::Transparent);
            target.draw(frame);

            auto toLocal = [&](float nx, float ny){
                return sf::Vector2f(frameX + 24.f + nx * (frameW - 48.f), frameY + 24.f + ny * (frameH - 48.f));
            };

            vector<sf::Vector2f> pts = {
                toLocal(0.08f, 0.30f),
                toLocal(0.38f, 0.10f),
                toLocal(0.78f, 0.24f),
                toLocal(0.86f, 0.70f),
                toLocal(0.56f, 0.92f),
                toLocal(0.16f, 0.74f)
            };

            auto drawEdge = [&](int a, int b, bool hi = false){
                sf::Vector2f p1 = pts[a];
                sf::Vector2f p2 = pts[b];
                sf::Vector2f d(p2.x - p1.x, p2.y - p1.y);
                float len = sqrt(d.x * d.x + d.y * d.y);
                if (len <= 0.001f) return;

                sf::RectangleShape line(sf::Vector2f(len, hi ? 4.f : 3.f));
                line.setOrigin(sf::Vector2f(0.f, hi ? 2.f : 1.5f));
                line.setPosition(p1);
                line.setRotation(sf::degrees(atan2(d.y, d.x) * 180.f / static_cast<float>(M_PI)));
                sf::Color lineCol = hi ? ThemeManager::current.primary : sf::Color(120, 130, 145);
                lineCol.a = previewAlpha;
                line.setFillColor(lineCol);
                target.draw(line);
            };

            drawEdge(0, 1);
            drawEdge(1, 2);
            drawEdge(2, 3, true);
            drawEdge(3, 4);
            drawEdge(4, 5);
            drawEdge(5, 0);
            drawEdge(0, 2);
            drawEdge(1, 4);

            for (const auto& p : pts){
                sf::CircleShape node(10.f);
                node.setOrigin(sf::Vector2f(10.f, 10.f));
                node.setPosition(p);
                sf::Color nodeCol(165, 165, 165);
                nodeCol.a = previewAlpha;
                node.setFillColor(nodeCol);
                node.setOutlineThickness(2.f);
                sf::Color outlineCol = ThemeManager::current.textColor;
                outlineCol.a = previewAlpha;
                node.setOutlineColor(outlineCol);
                target.draw(node);
            }
        }

        if (card.elementsAlpha > 5.f){
            float btnW = 230.f;
            float btnH = 67.f;
            float btnX = drawX + 65.f;
            float btnY = 842.f;
            
            sf::Color pColor = ThemeManager::current.primary;
            
            sf::ConvexShape pillBtn = createRoundedRect({btnW, btnH}, btnH / 2.f);
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

