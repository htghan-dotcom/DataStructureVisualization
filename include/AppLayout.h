#pragma once
#include <SFML/Graphics.hpp>
#include "GUI.h"
#include "ThemeManager.h"
#include <iostream>

class AppLayout {
private:
    sf::RectangleShape mFooter;
    sf::Text mHeaderText;
    sf::ConvexShape mDescriptionBox;
    sf::Text mDescriptionText;
    sf::Font mFontBold;
    sf::Font mFontRegular;
    sf::Texture mHomeTex;
    
    std::vector<std::string> mCodeLines;
    int mActiveCodeLine = -1;
    
    sf::Texture mSkipBackTex, mPauseTex, mSkipForwardTex, mStartTex;
    ImageButton mSkipBackBtn, mPauseBtn, mSkipForwardBtn, mStartBtn;
        
    sf::CircleShape mStepBgLeft, mStepBgRight;
    sf::ConvexShape mStepBgCenter;
    sf::Text mStepText;
        
    RoundedButton mStepBackBtn, mStepForwardBtn;
    SpeedSlider mSpeedSlider;

    sf::ConvexShape mPseudoBox;
    sf::ConvexShape createRoundedRect(sf::Vector2f size, float radius);

    RoundedButton mHideDescBtn;
    RoundedButton mShowDescBtn;
    RoundedButton mHidePseudoBtn;
    RoundedButton mShowPseudoBtn;

    bool mIsDescVisible = true;
    bool mIsPseudoVisible = true;
    bool mIsPaused = false;
    
public:
    ImageButton mHomeBtn;
    bool mGoHome = false;

    AppLayout();
    void update(sf::Vector2i mousePos);
    void draw(sf::RenderWindow& window);
    void setDescription(const std::string& text);
    
    void setPseudoCode(const std::vector<std::string>& codeLines);
    void setActiveCodeLine(int lineIndex);
    
    bool isPaused() const {return mIsPaused;}
    void setPaused(bool paused);
    float getSpeed() const {return mSpeedSlider.getSpeed();}
    void setStepText(const std::string& text){mStepText.setString(text);}
    

    std::function<void()> mPlayPauseCb;
    std::function<void()> mSkipBackCb;
    std::function<void()> mSkipForwardCb;
    std::function<void()> mStepBackCb;
    std::function<void()> mStepForwardCb;
    
    void setPlayPauseCallback(std::function<void()> cb){mPlayPauseCb = cb;}
    void setSkipBackCallback(std::function<void()> cb){mSkipBackCb = cb;}
    void setSkipForwardCallback(std::function<void()> cb){mSkipForwardCb = cb;}
    void setStepBackCallback(std::function<void()> cb){mStepBackCb = cb;}
    void setStepForwardCallback(std::function<void()> cb){mStepForwardCb = cb;}

    std::vector<int> mActiveCodeLines;
    void setActiveCodeLines(const std::vector<int>& lines) { mActiveCodeLines = lines; }
};
