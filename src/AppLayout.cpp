#include "AppLayout.h"
#include "ThemeManager.h"
#include "Common.h"

AppLayout::AppLayout()
    : mHeaderText(mFontBold),
      mDescriptionText(mFontBold),
      mStepText(mFontRegular),
      mSpeedSlider(mFontRegular, 32.f, 917.f),
      mStepBackBtn(mFontRegular, "Step back", 288.f, 818.f, 165.f, 49.f, 24.5f, sf::Color::White),
      mStepForwardBtn(mFontRegular, "Step forward", 466.f, 818.f, 201.f, 49.f, 24.5f, sf::Color::White)
{
    if (!mFontBold.openFromFile("assets/fonts/Inter-Bold.ttf") or !mFontRegular.openFromFile("assets/fonts/Inter-Regular.ttf")){
        cerr << "Cannot load font!" << endl;
    }
    if (!mHomeTex.loadFromFile("assets/images/homeButton.png")){cerr << "Cannot load homeButton.png" << endl;}
    mHomeTex.setSmooth(true); mHomeTex.generateMipmap();

    mFooter.setSize({1440.f, 156.f});
    mFooter.setPosition({0.f, 804.f});

    mStepBgCenter = createRoundedRect(sf::Vector2f(580.f, 61.f), 50.f);
    mStepBgCenter.setPosition(sf::Vector2f(93.f, 812.f));

    mHeaderText.setFont(mFontBold);
    mHeaderText.setString("Data Structure Visualization");
    mHeaderText.setCharacterSize(30);
    mHeaderText.setPosition({95.f, 68.f});

    mHomeBtn.setup(mHomeTex, 32.f, 60.f, 55.f, 55.f);
    mHomeBtn.setCallback([this](){mGoHome = true;});
    
    mDescriptionBox = createRoundedRect(sf::Vector2f(720.f, 394.f), 25.f);
    mDescriptionBox.setPosition(sf::Vector2f(745.f, 616.f));
    mDescriptionBox.setFillColor(ThemeManager::current.secondary);
    
    mDescriptionText.setFont(mFontBold);
    mDescriptionText.setFillColor(ThemeManager::current.primary);
    mDescriptionText.setCharacterSize(25);
    mDescriptionText.setPosition(sf::Vector2f(765.f, 636.f));
    
    if (!mSkipBackTex.loadFromFile("assets/images/skipbackButton.png")){cerr << "Cannot load skipbackButton.png" << endl;}
    if (!mPauseTex.loadFromFile("assets/images/pauseButton.png")){cerr << "Cannot load pauseButton.png " << endl;}
    if (!mSkipForwardTex.loadFromFile("assets/images/skipforwardButton.png")){cerr << "Cannot load skipforwardButton.png" << endl;}
    if (!mStartTex.loadFromFile("assets/images/startButton.png")){cerr << "Cannot load startButton.png" << endl;}

    mSkipBackTex.setSmooth(true); mSkipBackTex.generateMipmap();
    mPauseTex.setSmooth(true); mPauseTex.generateMipmap();
    mSkipForwardTex.setSmooth(true); mSkipForwardTex.generateMipmap();
    mStartTex.setSmooth(true); mStartTex.generateMipmap();

    mSkipBackBtn.setup(mSkipBackTex, 32.f, 819.f, 48.f, 48.f);
    mPauseBtn.setup(mPauseTex, 96.f, 819.f, 48.f, 48.f);
    mSkipForwardBtn.setup(mSkipForwardTex, 160.f, 819.f, 48.f, 48.f);
    mStartBtn.setup(mStartTex, 96.f, 819.f, 48.f, 48.f);

    mStepText.setCharacterSize(24);
    mStepText.setString("Step 0 / 0");
    mStepText.setOrigin(sf::Vector2f(0.f, 0.f));
    mStepText.setPosition(sf::Vector2f(152.f, 826.f));

    mPauseBtn.setCallback([this](){
        mIsPaused = true;
        mSkipBackBtn.setup(mSkipBackTex, 32.f, 819.f, 48.f, 48.f);
        mSkipForwardBtn.setup(mSkipForwardTex, 686.f, 819.f, 48.f, 48.f);
        if (mPlayPauseCb) mPlayPauseCb();
    });

    mStartBtn.setCallback([this](){
        mIsPaused = false;
        mSkipBackBtn.setup(mSkipBackTex, 32.f, 819.f, 48.f, 48.f);
        mSkipForwardBtn.setup(mSkipForwardTex, 160.f, 819.f, 48.f, 48.f);
        if (mPlayPauseCb) mPlayPauseCb();
    });
    
    mSkipBackBtn.setCallback([this](){if(mSkipBackCb) mSkipBackCb();});
    mSkipForwardBtn.setCallback([this](){if(mSkipForwardCb) mSkipForwardCb();});
    mStepBackBtn.setCallback([this](){if(mStepBackCb) mStepBackCb();});
    mStepForwardBtn.setCallback([this](){if(mStepForwardCb) mStepForwardCb();});
    
    mStepBackBtn.setCharacterSize(24);
    mStepForwardBtn.setCharacterSize(24);
}

void AppLayout::update(sf::Vector2i mousePos){
    mHomeBtn.update(mousePos);
    mSpeedSlider.update(mousePos);
    mSkipBackBtn.update(mousePos);
    mSkipForwardBtn.update(mousePos);
    
    if (mIsPaused){
        mStartBtn.update(mousePos);
        mStepBackBtn.update(mousePos);
        mStepForwardBtn.update(mousePos);
    } else {
        mPauseBtn.update(mousePos);
    }
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
    
    float boxX = mDescriptionBox.getPosition().x;
    float boxY = mDescriptionBox.getPosition().y;
    float boxW = 695.f;
    
    sf::RectangleShape pseudoFrame(sf::Vector2f(boxW, 252.f));
    pseudoFrame.setPosition(sf::Vector2f(boxX, boxY + 92.f));
    pseudoFrame.setFillColor(ThemeManager::isDark ? ThemeManager::current.screenBg : sf::Color(196, 196, 196));
    window.draw(pseudoFrame);
    
    float startX = boxX + 20.f;
    float currentY = boxY + 95.f;
    
    for (int i = 0; i < mCodeLines.size(); ++i){
        sf::Text lineText(i == mActiveCodeLine ? mFontBold : mFontRegular, mCodeLines[i], 25);
        
        lineText.setPosition(sf::Vector2f(startX, currentY));
        
        if (i == mActiveCodeLine){
            sf::FloatRect textBounds = lineText.getGlobalBounds();
            
            sf::RectangleShape hgBg(sf::Vector2f(boxW, textBounds.size.y + 20.f));
            hgBg.setPosition(sf::Vector2f(boxX, textBounds.position.y - 10.f));
            hgBg.setFillColor(ThemeManager::current.bg);
            window.draw(hgBg);
            
            lineText.setFillColor(ThemeManager::current.primary);
        } else {
            lineText.setFillColor(ThemeManager::current.textColor);
        }

        lineText.setPosition(sf::Vector2f(startX, currentY));
        window.draw(lineText);
        
        currentY += lineText.getLocalBounds().size.y + 20.f;
    }
    
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
        mSkipBackBtn.setup(mSkipBackTex, 32.f, 819.f, 48.f, 48.f);
        mSkipForwardBtn.setup(mSkipForwardTex, 686.f, 819.f, 48.f, 48.f);
    } else {
        mSkipBackBtn.setup(mSkipBackTex, 32.f, 819.f, 48.f, 48.f);
        mSkipForwardBtn.setup(mSkipForwardTex, 160.f, 819.f, 48.f, 48.f);
    }
}

void AppLayout::setPseudoCode(const std::vector<std::string>& codeLines){
    mCodeLines = codeLines;
}

void AppLayout::setActiveCodeLine(int lineIndex){
    mActiveCodeLine = lineIndex;
}
