#include "AppLayout.h"
#include "Common.h"

AppLayout::AppLayout()
    : mHeaderText(mFontBold),
      mDescriptionText(mFontRegular)
{
    if (!mFontBold.openFromFile("assets/fonts/Inter-Bold.ttf") or !mFontRegular.openFromFile("assets/fonts/Inter-Regular.ttf")){
        cerr << "Cannot load font!" << endl;
    }
    if (!mHomeTex.loadFromFile("assets/images/homeButton.png")){
        cerr << "Cannot load homeButton.png" << endl;
    }
    mHomeTex.setSmooth(true);
    mHomeTex.generateMipmap();

    mFooter.setSize({1440.f, 78.f});
    mFooter.setPosition({0.f, 882.f});
    mFooter.setFillColor(sf::Color(217, 217, 217));

    mHeaderText.setFont(mFontBold);
    mHeaderText.setString("Data Structure Visualization");
    mHeaderText.setCharacterSize(30);
    mHeaderText.setFillColor(sf::Color::Black);
    mHeaderText.setPosition({130.f, 68.f});

    mHomeBtn.setup(mHomeTex, 67.f, 62.f, 48.f, 48.f);
    mHomeBtn.setCallback([this]() {
        mGoHome = true;
    });
    
    mDescriptionBox.setSize({400.f, 150.f});
    mDescriptionBox.setFillColor(sf::Color(217, 217, 217));
    mDescriptionBox.setPosition({1018.f, 165.f});
    
    mDescriptionText.setFont(mFontRegular);
    mDescriptionText.setCharacterSize(20);
    mDescriptionText.setFillColor(sf::Color::Black);
    mDescriptionText.setPosition({1040.f, 185.f});
}

void AppLayout::update(sf::Vector2i mousePos){
    mHomeBtn.update(mousePos);
}

void AppLayout::draw(sf::RenderWindow& window){
    window.draw(mFooter);
    window.draw(mHeaderText);
    window.draw(mDescriptionBox);
    window.draw(mDescriptionText);
    mHomeBtn.draw(window);
}

void AppLayout::setDescription(const string& text){
    mDescriptionText.setString(text);
}
