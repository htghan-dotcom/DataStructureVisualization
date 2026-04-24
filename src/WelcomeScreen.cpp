#include "WelcomeScreen.h"
#include "Common.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;

sf::ConvexShape WelcomeScreen::createRoundedRect(sf::Vector2f size, float radius){
    int pointsPerCorner = 15;
    sf::ConvexShape shape(pointsPerCorner * 4);
    int index = 0;
    
    for(int i = 0; i < pointsPerCorner; ++i){
        float angle = i * (M_PI / 2) / (pointsPerCorner - 1);
        shape.setPoint(index++, sf::Vector2f(size.x - radius + radius * sin(angle), radius - radius * cos(angle)));
    }
    
    for(int i = 0; i < pointsPerCorner; ++i){
        float angle = M_PI / 2 + i * (M_PI / 2) / (pointsPerCorner - 1);
        shape.setPoint(index++, sf::Vector2f(size.x - radius + radius * sin(angle), size.y - radius - radius * cos(angle)));
    }
    
    for(int i = 0; i < pointsPerCorner; ++i){
        float angle = M_PI + i * (M_PI / 2) / (pointsPerCorner - 1);
        shape.setPoint(index++, sf::Vector2f(radius + radius * sin(angle), size.y - radius - radius * cos(angle)));
    }
    
    for(int i = 0; i < pointsPerCorner; ++i){
        float angle = 3 * M_PI / 2 + i * (M_PI / 2) / (pointsPerCorner - 1);
        shape.setPoint(index++, sf::Vector2f(radius + radius * sin(angle), radius - radius * cos(angle)));
    }
    return shape;
}

sf::VertexArray WelcomeScreen::createGradientRoundedRect(sf::Vector2f size, float radius, sf::Vector2f pos, sf::Color cBottomLeft, sf::Color cTopRight){
    int pointsPerCorner = 15;
    int totalPoints = pointsPerCorner * 4;
    
    sf::VertexArray va(sf::PrimitiveType::Triangles, totalPoints * 3);
    
    sf::Vector2f center = size / 2.0f;
    
    auto lerpColor = [](sf::Color a, sf::Color b, float t){
        t = std::max(0.0f, std::min(1.0f, t));
        return sf::Color(
            static_cast<uint8_t>(a.r + (b.r - a.r) * t),
            static_cast<uint8_t>(a.g + (b.g - a.g) * t),
            static_cast<uint8_t>(a.b + (b.b - a.b) * t)
        );
    };

    sf::Color centerColor = lerpColor(cBottomLeft, cTopRight, 0.5f);
    std::vector<sf::Vector2f> p(totalPoints);
    int index = 0;
    
    for(int i = 0; i < pointsPerCorner; ++i){
        float angle = i * (M_PI / 2) / (pointsPerCorner - 1);
        p[index++] = sf::Vector2f(size.x - radius + radius * sin(angle), radius - radius * cos(angle));
    }
    for(int i = 0; i < pointsPerCorner; ++i){
        float angle = M_PI / 2 + i * (M_PI / 2) / (pointsPerCorner - 1);
        p[index++] = sf::Vector2f(size.x - radius + radius * sin(angle), size.y - radius - radius * cos(angle));
    }
    for(int i = 0; i < pointsPerCorner; ++i){
        float angle = M_PI + i * (M_PI / 2) / (pointsPerCorner - 1);
        p[index++] = sf::Vector2f(radius + radius * sin(angle), size.y - radius - radius * cos(angle));
    }
    for(int i = 0; i < pointsPerCorner; ++i){
        float angle = 3 * M_PI / 2 + i * (M_PI / 2) / (pointsPerCorner - 1);
        p[index++] = sf::Vector2f(radius + radius * sin(angle), radius - radius * cos(angle));
    }

    auto calcColor = [&](sf::Vector2f pt){
        float t = (pt.x + (size.y - pt.y)) / (size.x + size.y);
        return lerpColor(cBottomLeft, cTopRight, t);
    };

    for (int i = 0; i < totalPoints; ++i){
        int next = (i + 1) % totalPoints;
        
        va[i*3].position = pos + center;
        va[i*3].color = centerColor;
        
        va[i*3+1].position = pos + p[i];
        va[i*3+1].color = calcColor(p[i]);
        
        va[i*3+2].position = pos + p[next];
        va[i*3+2].color = calcColor(p[next]);
    }
    
    return va;
}

WelcomeScreen::WelcomeScreen()
    : mWelcomeText(mFontBold),
      mDataStructureText(mFontBold),
      mVisualizationText(mFontBold),
      mCppText(mFontBold),
      mStartText(mFontBold),
      mSettingIconSprite()
{
    if (!mFontBold.openFromFile("assets/fonts/Inter-Bold.ttf")){cerr << "Cannot load fonts for WelcomeScreen!" << endl;}

    sf::Color colorBottomLeft(132, 216, 68);
    sf::Color colorTopRight(90, 150, 44);

    for (int i = 0; i < 8; ++i){
        sf::ConvexShape shadow = createRoundedRect(sf::Vector2f(1161.f + i, 486.f + i), 50.f + i/2.f);
        shadow.setPosition(sf::Vector2f(141.f - i/2.f, 244.f + 9.f - i/2.f));
        shadow.setFillColor(sf::Color(0, 0, 0, 8));
        mMainShadows.push_back(shadow);
    }

    mGradientBg = createGradientRoundedRect(sf::Vector2f(1161.f, 486.f), 50.f, sf::Vector2f(141.f, 244.f), colorBottomLeft, colorTopRight);

    mWelcomeText.setFont(mFontBold);
    mWelcomeText.setString("WELCOME!");
    mWelcomeText.setCharacterSize(25);
    mWelcomeText.setPosition({207.f, 365.f});
    mWelcomeText.setFillColor(sf::Color::White);

    mDataStructureText.setFont(mFontBold);
    mDataStructureText.setString("Data Structure");
    mDataStructureText.setCharacterSize(77);
    mDataStructureText.setPosition({207.f, 405.f});
    mDataStructureText.setFillColor(sf::Color::White);

    mDecorationLine.setSize({119.f, 8.f});
    mDecorationLine.setPosition({221.f, 551.f});
    mDecorationLine.setFillColor(sf::Color::White);

    mVisualizationText.setFont(mFontBold);
    mVisualizationText.setString("Visualization");
    mVisualizationText.setCharacterSize(77);
    mVisualizationText.setPosition({363.f, 498.f});
    mVisualizationText.setFillColor(sf::Color::White);

    mCppText.setFont(mFontBold);
    mCppText.setString("C++");
    mCppText.setCharacterSize(25);
    mCppText.setPosition({767.f, 601.f});
    mCppText.setFillColor(sf::Color::White);

    for (int i = 0; i < 8; ++i){
        sf::ConvexShape sShadow = createRoundedRect(sf::Vector2f(255.f + i, 109.f + i), 50.f + i/2.f);
        sShadow.setPosition(sf::Vector2f(483.f - i/2.f, 683.f + 9.f - i/2.f));
        sShadow.setFillColor(sf::Color(0, 0, 0, 8));
        mStartShadows.push_back(sShadow);
    }

    mStartBtnShape = createRoundedRect(sf::Vector2f(255.f, 109.f), 50.f);
    mStartBtnShape.setPosition(sf::Vector2f(483.f, 683.f));
    mStartBtnShape.setFillColor(sf::Color::White);

    mStartText.setFont(mFontBold);
    mStartText.setString("Start");
    mStartText.setCharacterSize(45);
    mStartText.setFillColor(sf::Color::Black);
    
    sf::FloatRect textBounds = mStartText.getLocalBounds();
    mStartText.setOrigin(sf::Vector2f(textBounds.position.x + textBounds.size.x / 2.0f,
                                      textBounds.position.y + textBounds.size.y / 2.0f));
    mStartText.setPosition(sf::Vector2f(483.f + 255.f / 2.0f, 683.f + 109.f / 2.0f));

    for (int i = 0; i < 8; ++i){
        sf::CircleShape setShadow(54.5f + i/2.f);
        setShadow.setPosition(sf::Vector2f(848.f - i/2.f, 683.f + 9.f - i/2.f));
        setShadow.setFillColor(sf::Color(0, 0, 0, 8));
        mSettingShadows.push_back(setShadow);
    }
        
    mSettingBgShape.setRadius(54.5f);
    mSettingBgShape.setPosition(sf::Vector2f(848.f, 683.f));
    mSettingBgShape.setFillColor(sf::Color::White);
    
    if (mSettingTex.loadFromFile("assets/images/settingButton.png")){
        mSettingTex.setSmooth(true);
        mSettingTex.generateMipmap();
        mSettingIconSprite.emplace(mSettingTex);
        
        sf::FloatRect iconBounds = mSettingIconSprite->getLocalBounds();
            
        float iconScale = 72.f / iconBounds.size.x;
        mSettingIconSprite->setScale(sf::Vector2f(iconScale, iconScale));
            
        mSettingIconSprite->setOrigin(sf::Vector2f(iconBounds.size.x / 2.0f, iconBounds.size.y / 2.0f));
        mSettingIconSprite->setPosition(sf::Vector2f(848.f + 54.5f, 683.f + 54.5f));
    }
}

void WelcomeScreen::update(sf::Vector2i mousePos, const std::optional<sf::Event>& event){
    sf::FloatRect startBounds(sf::Vector2f(483.f, 683.f), sf::Vector2f(255.f, 109.f));
    sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
    mIsStartHovered = startBounds.contains(mousePosF);
    
    if (mIsStartHovered){
        mStartBtnShape.setFillColor(sf::Color(230, 230, 230));
    } else {
        mStartBtnShape.setFillColor(sf::Color::White);
    }

    if (event){
        if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()){
            if (mouseEvent->button == sf::Mouse::Button::Left && mIsStartHovered){
                mStartPressed = true;
            }
        }
    }
}

void WelcomeScreen::draw(sf::RenderWindow& window){
    for (auto& s : mMainShadows){
        window.draw(s);
    }
    
    window.draw(mGradientBg);
    
    window.draw(mWelcomeText);
    window.draw(mDataStructureText);
    window.draw(mDecorationLine);
    window.draw(mVisualizationText);
    window.draw(mCppText);

    for (auto& s : mStartShadows) window.draw(s);
    window.draw(mStartBtnShape);
    window.draw(mStartText);

    for (auto& s : mSettingShadows) window.draw(s);
    window.draw(mSettingBgShape);
    
    if (mSettingIconSprite.has_value()) {
        window.draw(mSettingIconSprite.value());
    }
}
