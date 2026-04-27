#include "AppLayout.h"
#include "ThemeManager.h"
#include "Common.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Helper function to support automatic line breaks
static std::string wrapText(const std::string& text, float maxWidth, const sf::Font& font, unsigned int charSize) {
    std::istringstream words(text);
    std::string word;
    std::string wrappedText = "";
    std::string currentLine = "";
    sf::Text measureText(font, "", charSize);

    while (words >> word) {
        std::string testLine = currentLine.empty() ? word : currentLine + " " + word;
        measureText.setString(testLine);
        
        // If adding new word exceeds the allowed width -> Wrap to the next line
        if (measureText.getLocalBounds().size.x > maxWidth) {
            wrappedText += currentLine + "\n";
            currentLine = word;
        } else {
            currentLine = testLine;
        }
    }
    wrappedText += currentLine;
    return wrappedText;
}

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
      mStepForwardBtn(mFontRegular, "Step forward", 504.f, 896.f, 201.f, 49.f, 24.5f, sf::Color::White),
      mHideDescBtn(mFontBold, ">", 1028.f, 170.f, 45.f, 80.f, 22.5f, sf::Color::White),
      mShowDescBtn(mFontBold, "<", 1375.f, 170.f, 45.f, 80.f, 22.5f, sf::Color::White),
      mHidePseudoBtn(mFontBold, ">", 1028.f, 475.f, 45.f, 80.f, 22.5f, sf::Color::White),
      mShowPseudoBtn(mFontBold, "<", 1375.f, 475.f, 45.f, 80.f, 22.5f, sf::Color::White)
{
    if (!mFontBold.openFromFile("assets/fonts/Inter-Bold.ttf") or !mFontRegular.openFromFile("assets/fonts/Inter-Regular.ttf")){
        cerr << "Cannot load font!" << endl;
    }
    if (!mHomeTex.loadFromFile("assets/images/homeButton.png")){cerr << "Cannot load homeButton.png" << endl;}
    mHomeTex.setSmooth(true); mHomeTex.generateMipmap();

    mFooter.setSize({1440.f, 78.f});
    mFooter.setPosition({0.f, 882.f});

    mStepBgCenter = createRoundedRect(sf::Vector2f(580.f, 61.f), 50.f);
    mStepBgCenter.setPosition(sf::Vector2f(131.f, 890.f));

    mHeaderText.setFont(mFontBold);
    mHeaderText.setString("Data Structure Visualization");
    mHeaderText.setCharacterSize(30);
    mHeaderText.setPosition({95.f, 68.f});

    mHomeBtn.setup(mHomeTex, 32.f, 60.f, 55.f, 55.f);
    mHomeBtn.setCallback([this](){ mGoHome = true; });
    
    mDescriptionBox = createRoundedRect(sf::Vector2f(330.f, 120.f), 20.f);
    mDescriptionBox.setPosition(sf::Vector2f(1080.f, 150.f));
    mDescriptionBox.setFillColor(ThemeManager::current.secondary);
    
    mDescriptionText.setFont(mFontRegular);
    mDescriptionText.setFillColor(ThemeManager::current.primary);
    mDescriptionText.setCharacterSize(20);
    mDescriptionText.setPosition(sf::Vector2f(1100.f, 165.f));

    mPseudoBox = createRoundedRect(sf::Vector2f(330.f, 460.f), 20.f);
    mPseudoBox.setPosition(sf::Vector2f(1080.f, 285.f));
    mPseudoBox.setFillColor(ThemeManager::current.secondary);

    mHideDescBtn.setCallback([this](){ mIsDescVisible = false; });
    mShowDescBtn.setCallback([this](){ mIsDescVisible = true; });
    mHidePseudoBtn.setCallback([this](){ mIsPseudoVisible = false; });
    mShowPseudoBtn.setCallback([this](){ mIsPseudoVisible = true; });
    
    if (!mSkipBackTex.loadFromFile("assets/images/skipbackButton.png")){cerr << "Check lai anh skipback" << endl;}
    if (!mPauseTex.loadFromFile("assets/images/pauseButton.png")){cerr << "Check lai anh pause" << endl;}
    if (!mSkipForwardTex.loadFromFile("assets/images/skipforwardButton.png")){cerr << "Check lai anh skipforward" << endl;}
    if (!mStartTex.loadFromFile("assets/images/startButton.png")){cerr << "Check lai anh start" << endl;}

    mSkipBackTex.setSmooth(true); mSkipBackTex.generateMipmap();
    mPauseTex.setSmooth(true); mPauseTex.generateMipmap();
    mSkipForwardTex.setSmooth(true); mSkipForwardTex.generateMipmap();
    mStartTex.setSmooth(true); mStartTex.generateMipmap();

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
    for (auto* b : {&mStepBackBtn, &mStepForwardBtn, &mHideDescBtn, &mShowDescBtn, &mHidePseudoBtn, &mShowPseudoBtn}) {
        b->refreshText();
    }
}

void AppLayout::update(sf::Vector2i mousePos){
    mHomeBtn.update(mousePos);
    mSpeedSlider.update(mousePos);
    mSkipBackBtn.update(mousePos);
    mPauseBtn.update(mousePos);
    mSkipForwardBtn.update(mousePos);
    if (mIsDescVisible) mHideDescBtn.update(mousePos);
    else mShowDescBtn.update(mousePos);
    if (mIsPseudoVisible) mHidePseudoBtn.update(mousePos);
    else mShowPseudoBtn.update(mousePos);
    mStartBtn.update(mousePos);
    mStepBackBtn.update(mousePos);
    mStepForwardBtn.update(mousePos);
}

void AppLayout::draw(sf::RenderWindow& window){
    mFooter.setFillColor(ThemeManager::current.secondary);
    mHeaderText.setFillColor(ThemeManager::current.textColor);

    window.draw(mFooter);
    window.draw(mHeaderText);

    mHideDescBtn.setThemeColor(ThemeManager::current.secondary);
    mShowDescBtn.setThemeColor(ThemeManager::current.secondary);
    mHidePseudoBtn.setThemeColor(ThemeManager::current.secondary);
    mShowPseudoBtn.setThemeColor(ThemeManager::current.secondary);

    if (mIsDescVisible) {
        mDescriptionBox.setFillColor(ThemeManager::current.secondary);
        mDescriptionText.setFillColor(ThemeManager::current.textColor);
        
        window.draw(mDescriptionBox);
        window.draw(mDescriptionText);
        mHideDescBtn.draw(window);
    } else {
        mShowDescBtn.draw(window);
    }

   if (mIsPseudoVisible) {
        mPseudoBox.setFillColor(ThemeManager::current.secondary);
        window.draw(mPseudoBox);
        
        float boxX = mPseudoBox.getPosition().x;
        float boxY = mPseudoBox.getPosition().y;
        float boxW = 330.f;
        
        float startX = boxX + 20.f;
        float currentY = boxY + 20.f; 
        
        for (int i = 0; i < (int)mCodeLines.size(); ++i){
            bool isActive = (std::find(mActiveCodeLines.begin(), mActiveCodeLines.end(), i) != mActiveCodeLines.end());
            std::string wrappedLine = wrapText(mCodeLines[i], 290.f, (isActive ? mFontBold : mFontRegular), 20);
            
            sf::Text lineText(isActive ? mFontBold : mFontRegular, wrappedLine, 20);
            lineText.setPosition(sf::Vector2f(startX, currentY)); 
            if (isActive){
                sf::FloatRect textBounds = lineText.getGlobalBounds();
                
                sf::RectangleShape hgBg(sf::Vector2f(boxW, textBounds.size.y + 14.f));
                hgBg.setPosition(sf::Vector2f(boxX, textBounds.position.y - 7.f));
                hgBg.setFillColor(ThemeManager::current.bg);
                window.draw(hgBg);
                
                lineText.setFillColor(ThemeManager::current.primary);
            } else {
                lineText.setFillColor(ThemeManager::current.textColor);
            }
            window.draw(lineText);
            currentY += lineText.getGlobalBounds().size.y + 15.f;
        }
        
        mHidePseudoBtn.draw(window);
    } else {
        mShowPseudoBtn.draw(window);
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
    std::string wrapped = wrapText(text, 290.f, mFontRegular, 20); 
    mDescriptionText.setString(wrapped);
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
