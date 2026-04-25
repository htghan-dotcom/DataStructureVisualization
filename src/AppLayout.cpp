#include "AppLayout.h"
#include "ThemeManager.h"
#include "config/Common.h"

using namespace std;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

sf::ConvexShape AppLayout::createRoundedRect(sf::Vector2f size, float radius){
    radius = std::min(radius, std::min(size.x / 2.0f, size.y / 2.0f));
    int pointsPerCorner = 15;
    sf::ConvexShape shape(pointsPerCorner * 4);
    int index = 0;
    for (int i = 0; i < pointsPerCorner; ++i){
        float angle = i * (M_PI / 2) / (pointsPerCorner - 1);
        shape.setPoint(index++, sf::Vector2f(size.x - radius + radius * sin(angle), radius - radius * cos(angle)));
    }
    for (int i = 0; i < pointsPerCorner; ++i){
        float angle = M_PI / 2 + i * (M_PI / 2) / (pointsPerCorner - 1);
        shape.setPoint(index++, sf::Vector2f(size.x - radius + radius * sin(angle), size.y - radius - radius * cos(angle)));
    }
    for (int i = 0; i < pointsPerCorner; ++i){
        float angle = M_PI + i * (M_PI / 2) / (pointsPerCorner - 1);
        shape.setPoint(index++, sf::Vector2f(radius + radius * sin(angle), size.y - radius - radius * cos(angle)));
    }
    for (int i = 0; i < pointsPerCorner; ++i){
        float angle = 3 * M_PI / 2 + i * (M_PI / 2) / (pointsPerCorner - 1);
        shape.setPoint(index++, sf::Vector2f(radius + radius * sin(angle), radius - radius * cos(angle)));
    }
    return shape;
}

AppLayout::AppLayout()
    : mHeaderText(mFontBold),
      mDescriptionText(mFontRegular),
      mStepText(mFontRegular),
      mSpeedSlider(mFontRegular, 956.f, 917.f),
      mStepBackBtn(mFontRegular, "Step back", 326.f, 896.f, 165.f, 49.f, 24.5f, sf::Color::White),
      mStepForwardBtn(mFontRegular, "Step forward", 504.f, 896.f, 201.f, 49.f, 24.5f, sf::Color::White)
{
    if (!mFontBold.openFromFile("assets/fonts/Inter-Bold.ttf") or !mFontRegular.openFromFile("assets/fonts/Inter-Regular.ttf")){
        cerr << "Cannot load font!" << endl;
    }
    if (!mHomeTex.loadFromFile("assets/images/homeButton.png")){cerr << "Cannot load homeButton.png" << endl;}
    mHomeTex.setSmooth(true); 
    (void)mHomeTex.generateMipmap();

    mFooter.setSize({1440.f, 78.f});
    mFooter.setPosition({0.f, 882.f});

    mStepBgCenter = createRoundedRect(sf::Vector2f(580.f, 61.f), 50.f);
    mStepBgCenter.setPosition(sf::Vector2f(131.f, 890.f));

    mHeaderText.setFont(mFontBold);
    mHeaderText.setString("Data Structure Visualization");
    mHeaderText.setCharacterSize(30);
    mHeaderText.setPosition({130.f, 68.f});

    mHomeBtn.setup(mHomeTex, 67.f, 60.f, 55.f, 55.f);
    mHomeBtn.setCallback([this]() { mGoHome = true; });
    
    mDescriptionBox.setSize({400.f, 150.f});
    mDescriptionBox.setPosition({1018.f, 165.f});
    
    mDescriptionText.setFont(mFontRegular);
    mDescriptionText.setCharacterSize(20);
    mDescriptionText.setPosition({1040.f, 185.f});
    
    if (!mSkipBackTex.loadFromFile("assets/images/skipbackButton.png")){cerr << "Check lai anh skipback" << endl;}
    if (!mPauseTex.loadFromFile("assets/images/pauseButton.png")){cerr << "Check lai anh pause" << endl;}
    if (!mSkipForwardTex.loadFromFile("assets/images/skipforwardButton.png")){cerr << "Check lai anh skipforward" << endl;}
    if (!mStartTex.loadFromFile("assets/images/startButton.png")){cerr << "Check lai anh start" << endl;}

    mSkipBackTex.setSmooth(true); 
    (void)mSkipBackTex.generateMipmap();
    mPauseTex.setSmooth(true); 
    (void)mPauseTex.generateMipmap();
    mSkipForwardTex.setSmooth(true); 
    (void)mSkipForwardTex.generateMipmap();
    mStartTex.setSmooth(true); 
    (void)mStartTex.generateMipmap();

    mSkipBackBtn.setup(mSkipBackTex, 333.f, 897.f, 48.f, 48.f);
    mPauseBtn.setup(mPauseTex, 397.f, 897.f, 48.f, 48.f);
    mSkipForwardBtn.setup(mSkipForwardTex, 461.f, 897.f, 48.f, 48.f);
    mStartBtn.setup(mStartTex, 134.f, 897.f, 48.f, 48.f);

    mStepText.setCharacterSize(24);
    mStepText.setString("Step 0 / 0");
    mStepText.setOrigin(sf::Vector2f(0.f, 0.f));
    mStepText.setPosition(sf::Vector2f(190.f, 904.f));

    mPauseBtn.setCallback([this](){
        mIsPaused = true;
        mSkipBackBtn.setup(mSkipBackTex, 67.f, 897.f, 48.f, 48.f);
        mSkipForwardBtn.setup(mSkipForwardTex, 735.f, 897.f, 48.f, 48.f);
        if (mPlayPauseCb) mPlayPauseCb();
    });

    mStartBtn.setCallback([this](){
        mIsPaused = false;
        mSkipBackBtn.setup(mSkipBackTex, 333.f, 897.f, 48.f, 48.f);
        mSkipForwardBtn.setup(mSkipForwardTex, 461.f, 897.f, 48.f, 48.f);
        if (mPlayPauseCb) mPlayPauseCb();
    });
    
    mSkipBackBtn.setCallback([this](){ if(mSkipBackCb) mSkipBackCb(); });
    mSkipForwardBtn.setCallback([this](){ if(mSkipForwardCb) mSkipForwardCb(); });
    mStepBackBtn.setCallback([this](){ if(mStepBackCb) mStepBackCb(); });
    mStepForwardBtn.setCallback([this](){ if(mStepForwardCb) mStepForwardCb(); });
    
    mStepBackBtn.setCharacterSize(24);
    mStepForwardBtn.setCharacterSize(24);
}

void AppLayout::update(sf::Vector2i mousePos){
    mHomeBtn.update(mousePos);
    mSpeedSlider.update(mousePos);
    mSkipBackBtn.update(mousePos);
    mPauseBtn.update(mousePos);
    mSkipForwardBtn.update(mousePos);
    mStartBtn.update(mousePos);
    mStepBackBtn.update(mousePos);
    mStepForwardBtn.update(mousePos);
}

void AppLayout::draw(sf::RenderWindow& window){
    mFooter.setFillColor(ThemeManager::current.secondary);
    mDescriptionBox.setFillColor(ThemeManager::current.secondary);
    mHeaderText.setFillColor(ThemeManager::current.textColor);
    mDescriptionText.setFillColor(ThemeManager::current.textColor);

    window.draw(mFooter);
    window.draw(mHeaderText);
    window.draw(mDescriptionBox);
    window.draw(mDescriptionText);

    sf::Color iconCol = ThemeManager::current.textColor;
    if(mSkipBackBtn.mSprite) mSkipBackBtn.mSprite->setColor(iconCol);
    if(mSkipForwardBtn.mSprite) mSkipForwardBtn.mSprite->setColor(iconCol);
    if(mPauseBtn.mSprite) mPauseBtn.mSprite->setColor(iconCol);
    if(mStartBtn.mSprite) mStartBtn.mSprite->setColor(iconCol);
    if(mHomeBtn.mSprite) mHomeBtn.mSprite->setColor(ThemeManager::current.primary);

    mHomeBtn.draw(window);
    mSpeedSlider.draw(window);
    mSkipBackBtn.draw(window);
    mSkipForwardBtn.draw(window);
    
    if (mIsPaused){
        mStepBgCenter.setFillColor(ThemeManager::isDark ? ThemeManager::current.screenBg : sf::Color(196, 196, 196));
        window.draw(mStepBgCenter);
        
        mStartBtn.draw(window);
        
        mStepBackBtn.setThemeColor(ThemeManager::current.bg);
        mStepForwardBtn.setThemeColor(ThemeManager::current.bg);
        
        mStepBackBtn.draw(window);
        mStepForwardBtn.draw(window);
        
        mStepText.setFillColor(ThemeManager::current.textColor);
        window.draw(mStepText);
    } else {
        mPauseBtn.draw(window);
    }
}

void AppLayout::setDescription(const std::string& text){
    mDescriptionText.setString(text);
}

void AppLayout::setPaused(bool paused){
    mIsPaused = paused;
    if (mIsPaused){
        mSkipBackBtn.setup(mSkipBackTex, 67.f, 897.f, 48.f, 48.f);
        mSkipForwardBtn.setup(mSkipForwardTex, 735.f, 897.f, 48.f, 48.f);
    } else {
        mSkipBackBtn.setup(mSkipBackTex, 333.f, 897.f, 48.f, 48.f);
        mSkipForwardBtn.setup(mSkipForwardTex, 461.f, 897.f, 48.f, 48.f);
    }
}
