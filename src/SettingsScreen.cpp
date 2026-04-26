#include "SettingsScreen.h"
#include "ThemeManager.h"
#include "Common.h"

void SettingsScreen::setupButton(TriButton& btn, float x, float y){
    btn.outer = createRoundedRect({145.f, 145.f}, 50.f);
    btn.outer.setPosition(sf::Vector2f(x, y));
    btn.outer.setFillColor(sf::Color(217, 217, 217));
    
    btn.middle = createRoundedRect({115.f, 115.f}, 35.f);
    btn.middle.setPosition(sf::Vector2f(x + 15.f, y + 15.f));
    btn.middle.setFillColor(sf::Color(90, 150, 44));
    
    btn.inner = createRoundedRect({105.f, 105.f}, 30.f);
    btn.inner.setPosition(sf::Vector2f(x + 20.f, y + 20.f));
    btn.inner.setFillColor(sf::Color(245, 245, 240));
    
    btn.bounds = sf::FloatRect(sf::Vector2f(x, y), sf::Vector2f(145.f, 145.f));
    btn.label.reset();
}

SettingsScreen::SettingsScreen(float w, float h)
    : mWindowWidth(w), mWindowHeight(h),
      mTitleSettings(mFontBold),
      mLabelAboutUs(mFontBold),
      mLabelMusic(mFontBold),
      mLabelThemes(mFontBold),
      mThemePopupTitle(mFontBold),
      mToggleText(mFontBold)
{
    if (!mFontBold.openFromFile("assets/fonts/Inter-Bold.ttf") or !mFontRegular.openFromFile("assets/fonts/Inter-Regular.ttf")){
        cerr << "Cannot load fonts for SettingsScreen!" << endl;
    }

    mTitleSettings.setFont(mFontBold);
    mTitleSettings.setString("SETTINGS");
    mTitleSettings.setCharacterSize(90);
    mTitleSettings.setFillColor(ThemeManager::current.textColor);
    
    sf::FloatRect sBounds = mTitleSettings.getLocalBounds();
    mTitleSettings.setOrigin(sf::Vector2f(sBounds.position.x + sBounds.size.x / 2.0f, 0.0f));
    mTitleSettings.setPosition(sf::Vector2f(mWindowWidth / 2.0f, 100.f));

    mLabelAboutUs.setFont(mFontBold);
    mLabelAboutUs.setString("About Us");
    mLabelAboutUs.setCharacterSize(65);
    mLabelAboutUs.setPosition(sf::Vector2f(233.f, 251.f));
    mLabelAboutUs.setFillColor(ThemeManager::current.textColor);

    mLabelMusic.setFont(mFontBold);
    mLabelMusic.setString("Music");
    mLabelMusic.setCharacterSize(50);
    mLabelMusic.setPosition(sf::Vector2f(954.f, 265.f));
    mLabelMusic.setFillColor(ThemeManager::current.textColor);

    mLabelThemes.setFont(mFontBold);
    mLabelThemes.setString("Themes");
    mLabelThemes.setCharacterSize(50);
    mLabelThemes.setPosition(sf::Vector2f(233.f, 757.f));
    mLabelThemes.setFillColor(ThemeManager::current.textColor);

    setupButton(mBtnColor, 494.f, 715.f);
    setupButton(mBtnFont, 676.f, 715.f);
    setupButton(mBtnMusic, 858.f, 715.f);
    
    mBtnFont.label.emplace(mFontRegular);
    mBtnFont.label->setString("Inter");
    mBtnFont.label->setCharacterSize(40);
    mBtnFont.label->setFillColor(ThemeManager::current.textColor);
    
    sf::FloatRect textBounds = mBtnFont.label->getLocalBounds();
    mBtnFont.label->setOrigin(sf::Vector2f(textBounds.position.x + textBounds.size.x / 2.0f, textBounds.position.y + textBounds.size.y / 2.0f));
    mBtnFont.label->setPosition(sf::Vector2f(676.f + 72.5f, 715.f + 72.5f));
    
    if (!mMusicTex.loadFromFile("assets/images/musicButton.png")){
        cerr << "Cannot load musicButton.png" << endl;
    } else {
        mMusicTex.setSmooth(true);
        mMusicTex.generateMipmap();
        mBtnMusic.icon.emplace(mMusicTex);
        
        sf::FloatRect iconBounds = mBtnMusic.icon->getLocalBounds();
        float scale = 76.f / iconBounds.size.x;
        mBtnMusic.icon->setScale(sf::Vector2f(scale, scale));
        
        mBtnMusic.icon->setOrigin(sf::Vector2f(iconBounds.size.x / 2.0f, iconBounds.size.y / 2.0f));
        mBtnMusic.icon->setPosition(sf::Vector2f(858.f + 72.5f, 715.f + 72.5f));
    }

    if (!mExitTex.loadFromFile("assets/images/exitButton.png")){cerr << "Cannot load exitButton.png" << endl;}
    mExitTex.setSmooth(true);
    mExitTex.generateMipmap();

    mExitBtn.setup(mExitTex, 49.f, 62.f, 48.f, 48.f);
    mExitBtn.setCallback([this](){
        mGoBack = true;
    });
    
    mPopupShadow = createRoundedRect(sf::Vector2f(1077.f, 699.f), 50.f);
    mPopupShadow.setPosition(sf::Vector2f(181.f, 127.f));
    mPopupShadow.setFillColor(sf::Color(90, 150, 44));
        
    mPopupBg = createRoundedRect(sf::Vector2f(1077.f, 699.f), 50.f);
    mPopupBg.setPosition(sf::Vector2f(181.f, 118.f));
    mPopupBg.setFillColor(sf::Color(217, 217, 217));
        
    mCloseBtnBg.setRadius(35.f);
    mCloseBtnBg.setPosition(sf::Vector2f(181.f + 1077.f - 45.f, 118.f + 45.f - 70.f));
    mCloseBtnBg.setFillColor(sf::Color(217, 217, 217));
    if (mCloseTex.loadFromFile("assets/images/closeButton.png")){
        mCloseTex.setSmooth(true);
        mCloseTex.generateMipmap();
        mCloseIcon = sf::Sprite(mCloseTex);
        sf::FloatRect cb = mCloseIcon->getLocalBounds();
        mCloseIcon->setOrigin(sf::Vector2f(cb.size.x/2.f, cb.size.y/2.f));
        mCloseIcon->setPosition(sf::Vector2f(mCloseBtnBg.getPosition().x + 35.f, mCloseBtnBg.getPosition().y + 35.f));
        mCloseIcon->setScale(sf::Vector2f(42.f/cb.size.x, 42.f/cb.size.y));
    }

    mPopupInner = createRoundedRect(sf::Vector2f(1033.f, 655.f), 28.f);
    mPopupInner.setPosition(sf::Vector2f(203.f, 140.f));
    mPopupInner.setFillColor(sf::Color::White);
    
    mThemePopupTitle.setFont(mFontBold);
    mThemePopupTitle.setString("THEME COLOR");
    mThemePopupTitle.setCharacterSize(80);
    mThemePopupTitle.setFillColor(ThemeManager::current.textColor);
    mThemePopupTitle.setPosition(sf::Vector2f(428.f, 160.f));
    
    if (mTickTex.loadFromFile("assets/images/tickIcon.png")){
        mTickTex.setSmooth(true);
        mTickSprite = sf::Sprite(mTickTex);
        mTickSprite->setScale(sf::Vector2f(0.5f, 0.5f));
        sf::FloatRect ts = mTickSprite->getLocalBounds();
        mTickSprite->setScale(sf::Vector2f(80.f/ts.size.x, 80.f/ts.size.y));
    }

    vector<string> colorNames = {"Default", "Pinky", "Navy", "Golden"};
    vector<sf::Color> middleColors = { sf::Color(90, 150, 44), sf::Color(232, 62, 140), sf::Color(30, 58, 138), sf::Color(245, 158, 11) };
    vector<sf::Color> innerColors = { sf::Color(245, 245, 240), sf::Color(255, 245, 248), sf::Color(248, 250, 252), sf::Color(255,253, 240) };
        
        float startX = 305.f;
        for (int i = 0; i < 4; ++i){
            ThemeColorBtn tb;
            float curX = startX + i * (145.f + 83.f);
            setupButton(tb.btn, curX, 291.f);
            tb.btn.middle.setFillColor(middleColors[i]);
            tb.btn.inner.setFillColor(innerColors[i]);
            tb.themeColor = middleColors[i];
            
            tb.label.emplace(mFontBold);
            tb.label->setString(colorNames[i]);
            tb.label->setCharacterSize(45);
            tb.label->setFillColor(ThemeManager::current.textColor);
            sf::FloatRect lb = tb.label->getLocalBounds();
            tb.label->setOrigin(sf::Vector2f(lb.position.x + lb.size.x/2.f, 0.f));
            tb.label->setPosition(sf::Vector2f(curX + 72.5f, 441.f));
            
            if (i == 0) tb.isSelected = true;
            mThemeBtns.push_back(tb);
        }

        float toggleX = 720.f - (373.f / 2.f);
        float toggleY = 600.f;
        
        mToggleOuter = createRoundedRect(sf::Vector2f(373.f, 145.f), 50.f);
        mToggleOuter.setPosition(sf::Vector2f(toggleX, toggleY));
        mToggleOuter.setFillColor(sf::Color(217, 217, 217));
        
        mToggleMiddle = createRoundedRect(sf::Vector2f(115.f, 115.f), 35.f);
        mToggleMiddle.setPosition(sf::Vector2f(toggleX + 15.f, toggleY + 15.f));
        mToggleMiddle.setFillColor(middleColors[0]);
        
        mToggleInner = createRoundedRect(sf::Vector2f(105.f, 105.f), 30.f);
        mToggleInner.setPosition(sf::Vector2f(toggleX + 20.f, toggleY + 20.f));
        mToggleInner.setFillColor(innerColors[0]);
        
        mToggleText.setFont(mFontBold);
        mToggleText.setString("Light");
        mToggleText.setCharacterSize(45);
        mToggleText.setFillColor(ThemeManager::current.textColor);
        sf::FloatRect tb = mToggleText.getLocalBounds();
        mToggleText.setOrigin(sf::Vector2f(tb.position.x + tb.size.x/2.f, tb.position.y + tb.size.y/2.f));
        mToggleText.setPosition(sf::Vector2f(toggleX + 145.f + 114.f, toggleY + 72.5f));
    
    if (mLightTex.loadFromFile("assets/images/lightIcon.png")){
        mLightTex.setSmooth(true);
        mLightTex.generateMipmap();
        mLightIcon.emplace(mLightTex);
        
        sf::FloatRect lib = mLightIcon->getLocalBounds();
        mLightIcon->setOrigin(sf::Vector2f(lib.size.x / 2.f, lib.size.y / 2.f));

        float iconScale = 80.f / lib.size.x;
        mLightIcon->setScale(sf::Vector2f(iconScale, iconScale));
    }
    
    if (mDarkTex.loadFromFile("assets/images/darkIcon.png")){
        mDarkTex.setSmooth(true);
        mDarkTex.generateMipmap();
        mDarkIcon.emplace(mDarkTex);
        sf::FloatRect dib = mDarkIcon->getLocalBounds();
        mDarkIcon->setOrigin(sf::Vector2f(dib.size.x / 2.f, dib.size.y / 2.f));
        float iconScale = 80.f / dib.size.x;
        mDarkIcon->setScale(sf::Vector2f(iconScale, iconScale));
    }
}

void SettingsScreen::update(sf::Vector2i mousePos, const std::optional<sf::Event>& event){
    mExitBtn.update(mousePos);
    
    if (mShowThemePopup){
        if (event){
            if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()){
                if (mouseEvent->button == sf::Mouse::Button::Left){
                    sf::Vector2f mPosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
                    
                    for (int i = 0; i < mThemeBtns.size(); ++i){
                        if (mThemeBtns[i].btn.bounds.contains(mPosF)){
                            ThemeManager::setTheme(static_cast<ThemeType>(i));
                            
                            for (auto& b : mThemeBtns){
                                b.isSelected = false;
                            }
                            mThemeBtns[i].isSelected = true;
                            
                            mPopupShadow.setFillColor(ThemeManager::current.primary);
                        }
                    }
                    
                    sf::FloatRect toggleBounds(sf::Vector2f(720.f - (373.f / 2.f), 600.f), sf::Vector2f(373.f, 145.f));
                    if (toggleBounds.contains(mPosF)){
                        ThemeManager::isDark = !ThemeManager::isDark;
                        ThemeManager::setTheme(ThemeManager::currentType);
                    }
                    
                    sf::FloatRect closeBnd = mCloseBtnBg.getGlobalBounds();
                    if (closeBnd.contains(mPosF)){
                        mShowThemePopup = false;
                    }
                }
            }
        }
    }
}

void SettingsScreen::draw(sf::RenderWindow& window){
    auto syncBtnTheme = [](TriButton& b){
        b.outer.setFillColor(ThemeManager::current.secondary);
        b.middle.setFillColor(ThemeManager::current.primary);
        b.inner.setFillColor(ThemeManager::current.bg);
    };
    syncBtnTheme(mBtnColor);
    syncBtnTheme(mBtnFont);
    syncBtnTheme(mBtnMusic);
    
    mTitleSettings.setFillColor(ThemeManager::current.textColor);
    mLabelAboutUs.setFillColor(ThemeManager::current.textColor);
    mLabelMusic.setFillColor(ThemeManager::current.textColor);
    mLabelThemes.setFillColor(ThemeManager::current.textColor);
    
    if (mBtnFont.label.has_value()){
        mBtnFont.label->setFillColor(ThemeManager::current.textColor);
    }
    
    window.draw(mTitleSettings);
    window.draw(mLabelAboutUs);
    window.draw(mLabelMusic);
    window.draw(mLabelThemes);

    window.draw(mBtnColor.outer); window.draw(mBtnColor.middle); window.draw(mBtnColor.inner);
    if (mBtnColor.label.has_value()){window.draw(mBtnColor.label.value());}
    
    window.draw(mBtnFont.outer); window.draw(mBtnFont.middle); window.draw(mBtnFont.inner);
    if (mBtnFont.label.has_value()){window.draw(mBtnFont.label.value());}
    
    window.draw(mBtnMusic.outer); window.draw(mBtnMusic.middle); window.draw(mBtnMusic.inner);
    if (mBtnMusic.label.has_value()){window.draw(mBtnMusic.label.value());}
    if (mBtnMusic.icon.has_value()){
        mBtnMusic.icon->setColor(ThemeManager::current.textColor);
        window.draw(mBtnMusic.icon.value());
    }

    mExitBtn.mSprite->setColor(ThemeManager::current.primary);
    mExitBtn.draw(window);
    
    if (mShowThemePopup){
        sf::RectangleShape dimmingOverlay(sf::Vector2f(mWindowWidth, mWindowHeight));
        dimmingOverlay.setFillColor(sf::Color(0, 0, 0, 180));
        window.draw(dimmingOverlay);
        
        mPopupBg.setFillColor(ThemeManager::current.secondary);
        mPopupShadow.setFillColor(ThemeManager::current.primary);
        mPopupInner.setFillColor(ThemeManager::current.screenBg);
        mThemePopupTitle.setFillColor(ThemeManager::current.textColor);
        
        mCloseBtnBg.setFillColor(ThemeManager::current.secondary);
        if (mCloseIcon.has_value()){
            mCloseIcon->setColor(ThemeManager::current.textColor);
        }
        
        window.draw(mPopupShadow);
        window.draw(mPopupBg);
        window.draw(mPopupInner);
        window.draw(mCloseBtnBg);
        if (mCloseIcon.has_value()) window.draw(*mCloseIcon);
        window.draw(mThemePopupTitle);
        
        
        vector<sf::Color> lightMiddle = {
            sf::Color(90, 150, 44),
            sf::Color(232, 62, 140),
            sf::Color(30, 58, 138),
            sf::Color(245, 158, 11)
        };
        
        vector<sf::Color> lightInner = {
            sf::Color(245, 245, 240),
            sf::Color(255, 245, 248),
            sf::Color(248, 250, 252),
            sf::Color(255, 253, 240)
        };
        
        vector<sf::Color> darkMiddle = {
            sf::Color(110, 176, 58),
            sf::Color(213, 63, 140),
            sf::Color(59, 130, 246),
            sf::Color(249, 115, 22)
        };
        
        vector<sf::Color> darkInner = {
            sf::Color(32, 39, 34),
            sf::Color(45, 27, 38),
            sf::Color(30, 41, 59),
            sf::Color(44, 36, 27)
        };
        
        for (int i = 0; i < 4; ++i){
            mThemeBtns[i].btn.outer.setFillColor(ThemeManager::current.secondary);
            
            if (ThemeManager::isDark){
                mThemeBtns[i].btn.middle.setFillColor(darkMiddle[i]);
                mThemeBtns[i].btn.inner.setFillColor(darkInner[i]);
                mThemeBtns[i].themeColor = darkMiddle[i];
            } else {
                mThemeBtns[i].btn.middle.setFillColor(lightMiddle[i]);
                mThemeBtns[i].btn.inner.setFillColor(lightInner[i]);
                mThemeBtns[i].themeColor = lightMiddle[i];
            }
        }
        
        for (auto& tb : mThemeBtns){
            window.draw(tb.btn.outer);
            window.draw(tb.btn.middle);
            window.draw(tb.btn.inner);
            
            if (tb.label.has_value()){
                tb.label->setFillColor(ThemeManager::current.textColor);
                window.draw(tb.label.value());
            }
            
            if (tb.isSelected){
                if (mTickSprite.has_value()) {
                    mTickSprite->setColor(tb.themeColor);
                    
                    sf::FloatRect tickBnd = mTickSprite->getLocalBounds();
                    mTickSprite->setOrigin(sf::Vector2f(tickBnd.size.x/2.f, tickBnd.size.y/2.f));
                    mTickSprite->setPosition(sf::Vector2f(tb.btn.bounds.position.x + 72.5f, tb.btn.bounds.position.y + 72.5f));
                    window.draw(*mTickSprite);
                }
            }
        }
        
        sf::Color currentThemeColor = sf::Color(90, 150, 44);
        for (auto& tb : mThemeBtns){
            if (tb.isSelected) {
                currentThemeColor = tb.themeColor;
                break;
            }
        }
        
        mToggleOuter.setFillColor(ThemeManager::current.secondary);
        mToggleMiddle.setFillColor(currentThemeColor);
        
        float toggleX = 720.f - (373.f / 2.f);
        float toggleY = 600.f;
        
        window.draw(mToggleOuter);
        
        if (ThemeManager::isDark){
            mToggleMiddle.setPosition(sf::Vector2f(toggleX + 373.f - 115.f - 15.f, toggleY + 15.f));
            mToggleInner.setPosition(sf::Vector2f(toggleX + 373.f - 105.f - 20.f, toggleY + 20.f));
            mToggleInner.setFillColor(ThemeManager::current.bg);
            
            window.draw(mToggleMiddle);
            window.draw(mToggleInner);
            
            if (mDarkIcon.has_value()){
                mDarkIcon->setPosition(sf::Vector2f(toggleX + 373.f - 105.f/2.f - 20.f, toggleY + 72.5f));
                mDarkIcon->setColor(currentThemeColor);
                window.draw(*mDarkIcon);
            }
            
            mToggleText.setString("Dark");
            mToggleText.setFillColor(ThemeManager::current.textColor);
            sf::FloatRect tb = mToggleText.getLocalBounds();
            mToggleText.setOrigin(sf::Vector2f(tb.position.x + tb.size.x/2.f, tb.position.y + tb.size.y/2.f));
            mToggleText.setPosition(sf::Vector2f(toggleX + 114.f, toggleY + 72.5f));
            window.draw(mToggleText);
            
        } else {
            mToggleMiddle.setPosition(sf::Vector2f(toggleX + 15.f, toggleY + 15.f));
            mToggleInner.setPosition(sf::Vector2f(toggleX + 20.f, toggleY + 20.f));
            mToggleInner.setFillColor(ThemeManager::current.bg);
            
            window.draw(mToggleMiddle);
            window.draw(mToggleInner);
            
            if (mLightIcon.has_value()){
                mLightIcon->setPosition(sf::Vector2f(toggleX + 72.5f, toggleY + 72.5f));
                mLightIcon->setColor(currentThemeColor);
                window.draw(*mLightIcon);
            }
            
            mToggleText.setString("Light");
            mToggleText.setFillColor(ThemeManager::current.textColor);
            sf::FloatRect tb = mToggleText.getLocalBounds();
            mToggleText.setOrigin(sf::Vector2f(tb.position.x + tb.size.x/2.f, tb.position.y + tb.size.y/2.f));
            mToggleText.setPosition(sf::Vector2f(toggleX + 373.f - 114.f, toggleY + 72.5f));
            window.draw(mToggleText);
        }
    }
}
