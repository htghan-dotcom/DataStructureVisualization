#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <optional>
#include "GUI.h"

class SettingsScreen {
private:
    float mWindowWidth;
    float mWindowHeight;

    sf::Font mFontBold;
    sf::Font mFontRegular;
    
    sf::Text mTitleSettings;
    sf::Text mLabelAboutUs;
    sf::Text mLabelMusic;
    sf::Text mLabelThemes;

    struct TriButton {
        sf::ConvexShape outer;
        sf::ConvexShape middle;
        sf::ConvexShape inner;
        std::optional<sf::Text> label;
        std::optional<sf::Sprite> icon;
        sf::FloatRect bounds;
        
        TriButton() = default;
    };

    TriButton mBtnColor;
    TriButton mBtnFont;
    TriButton mBtnMusic;

    sf::Texture mMusicTex;
    sf::Texture mExitTex;
    
    sf::ConvexShape createRoundedRect(sf::Vector2f size, float radius);
    void setupButton(TriButton& btn, float x, float y);
    
    bool mShowThemePopup = false;
        
    sf::ConvexShape mPopupShadow;
    sf::ConvexShape mPopupBg;
    sf::ConvexShape mPopupInner;
    
    sf::Texture mTickTex;
    sf::Texture mCloseTex;
        
    sf::CircleShape mCloseBtnBg;
    std::optional<sf::Sprite> mCloseIcon;
    std::optional<sf::Sprite> mTickSprite;

    sf::Text mThemePopupTitle;
    
    struct ThemeColorBtn {
        TriButton btn;
        std::optional<sf::Text> label;
        sf::Color themeColor;
        bool isSelected = false;
    };
        
    std::vector<ThemeColorBtn> mThemeBtns;
    
    sf::Texture mLightTex;
    std::optional<sf::Sprite> mLightIcon;
    
    sf::Texture mDarkTex;
    std::optional<sf::Sprite> mDarkIcon;
    
    sf::ConvexShape mToggleOuter;
    sf::ConvexShape mToggleMiddle;
    sf::ConvexShape mToggleInner;
    sf::Text mToggleText;
    bool mIsLightMode = true;

public:
    ImageButton mExitBtn;
    bool mGoBack = false;
    
    void resetExit(){mGoBack = false;}
    
    SettingsScreen(float w, float h);
    void update(sf::Vector2i mousePos, const std::optional<sf::Event>& event);
    void draw(sf::RenderWindow& window);
    void toggleThemePopup(){mShowThemePopup = !mShowThemePopup;}
};
