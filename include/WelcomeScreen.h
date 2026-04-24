#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <optional>

class WelcomeScreen {
private:
    struct FloatingNode {
        sf::Vector2f pos;
        sf::Vector2f velocity;
        float radius;
    };
    std::vector<FloatingNode> mFloatingNodes;
    
    
    sf::VertexArray mGradientBg;
    std::vector<sf::ConvexShape> mMainShadows;
    
    sf::Text mWelcomeText;
    sf::Text mDataStructureText;
    sf::RectangleShape mDecorationLine;
    sf::Text mVisualizationText;
    sf::Text mCppText;
    
    sf::Font mFontBold;
    float mWindowWidth;
    float mWindowHeight;
    
    sf::ConvexShape mStartBtnShape;
    std::vector<sf::ConvexShape> mStartShadows;
    sf::Text mStartText;
    
    sf::CircleShape mSettingBgShape;
    std::optional<sf::Sprite> mSettingIconSprite;
    std::vector<sf::CircleShape> mSettingShadows;
    sf::Texture mSettingTex;

    bool mStartPressed = false;
    bool mIsStartHovered = false;
    
    bool mSettingPressed = false;
    bool mIsSettingHovered = false;

    sf::ConvexShape createRoundedRect(sf::Vector2f size, float radius);
    sf::VertexArray createGradientRoundedRect(sf::Vector2f size, float radius, sf::Vector2f pos, sf::Color cBottomLeft, sf::Color cTopRight);

public:
    WelcomeScreen(float w, float h);
    void update(sf::Vector2i mousePos, const std::optional<sf::Event>& event);
    void draw(sf::RenderWindow& window);
    
    bool isStartPressed(){return mStartPressed;}
    void resetStart(){mStartPressed = false;}
    
    bool isSettingPressed(){return mSettingPressed;}
    void resetSetting(){mSettingPressed = false;}
};
