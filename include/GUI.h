#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <functional>
#include <sstream>
#include <iomanip>
#include <optional>
#include "ThemeManager.h"

class RoundedButton {
private:
    sf::CircleShape mTopLeft, mTopRight, mBottomLeft, mBottomRight;
    sf::RectangleShape mHorizRect, mVertRect;
    sf::Text mText;
    sf::Color mNormalColor;
    sf::Color mHoverColor;
    std::function<void()> mCallback;
    bool mIsHovered = false;

public:
    RoundedButton(const sf::Font& font, const std::string& str, float x, float y, float w, float h, float radius, sf::Color color)
        : mText(font),
          mNormalColor(color)
    {
        mHoverColor = sf::Color(std::max(0, color.r - 20), std::max(0, color.g - 20), std::max(0, color.b - 20));

        mTopLeft.setRadius(radius); mTopLeft.setPosition(sf::Vector2f(x, y));
        mTopRight.setRadius(radius); mTopRight.setPosition(sf::Vector2f(x + w - radius * 2, y));
        mBottomLeft.setRadius(radius); mBottomLeft.setPosition(sf::Vector2f(x, y + h - radius * 2));
        mBottomRight.setRadius(radius); mBottomRight.setPosition(sf::Vector2f(x + w - radius * 2, y + h - radius * 2));

        mHorizRect.setSize(sf::Vector2f(w - radius * 2, h)); mHorizRect.setPosition(sf::Vector2f(x + radius, y));
        mVertRect.setSize(sf::Vector2f(w, h - radius * 2)); mVertRect.setPosition(sf::Vector2f(x, y + radius));

        setColor(mNormalColor);

        mText.setString(str);
        mText.setCharacterSize(21);
        mText.setFillColor(sf::Color::Black);
        
        sf::FloatRect bounds = mText.getLocalBounds();
        
        mText.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2.0f, bounds.position.y + bounds.size.y / 2.0f));
        mText.setPosition(sf::Vector2f(x + w / 2.0f, y + h / 2.0f));
    }

    void setColor(sf::Color c){
        mTopLeft.setFillColor(c); mTopRight.setFillColor(c);
        mBottomLeft.setFillColor(c); mBottomRight.setFillColor(c);
        mHorizRect.setFillColor(c); mVertRect.setFillColor(c);
    }
    
    void setThemeColor(sf::Color color){
        mNormalColor = color;
        mHoverColor = sf::Color(std::max(0, color.r - 20), std::max(0, color.g - 20), std::max(0, color.b - 20));
            
        if (!mIsHovered){
            setColor(mNormalColor);
        } else {
            setColor(mHoverColor);
        }
    }

    void update(const sf::Vector2i& mousePos){
        sf::FloatRect bounds(sf::Vector2f(mVertRect.getPosition().x, mHorizRect.getPosition().y), sf::Vector2f(mVertRect.getSize().x, mHorizRect.getSize().y));
        if (bounds.contains({static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)})){
            if (!mIsHovered){setColor(mHoverColor);}
            mIsHovered = true;
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) and mCallback) {mCallback();}
        } else {
            if (mIsHovered){setColor(mNormalColor);}
            
            mIsHovered = false;
        }
    }

    void draw(sf::RenderTarget& target){
        mText.setFillColor(ThemeManager::current.textColor);
        
        target.draw(mTopLeft); target.draw(mTopRight);
        target.draw(mBottomLeft); target.draw(mBottomRight);
        target.draw(mHorizRect); target.draw(mVertRect);
        target.draw(mText);
    }

    void setCallback(std::function<void()> cb){mCallback = cb;}
    
    void refreshText(){
        sf::FloatRect bounds = mText.getLocalBounds();
        mText.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2.0f, bounds.position.y + bounds.size.y / 2.0f));
    }
    
    void setCharacterSize(unsigned int size){
        mText.setCharacterSize(size);
        refreshText();
    }
};

class SpeedSlider {
private:
    sf::RectangleShape mTrack;
    sf::RectangleShape mFill;
    sf::CircleShape mThumb;
    sf::Text mLabelText;
    sf::Text mValText;
    float mMinVal, mMaxVal, mCurrentVal;
    bool mIsDragging = false;

public:
    SpeedSlider(const sf::Font& font, float x, float y)
        : mLabelText(font),
          mValText(font),
          mMinVal(0.5f),
          mMaxVal(3.0f),
          mCurrentVal(1.5f)
    {
        mLabelText.setString("Speed:");
        mLabelText.setCharacterSize(24);
        mLabelText.setFillColor(sf::Color::Black);
        sf::FloatRect labelBounds = mLabelText.getLocalBounds();

        mLabelText.setOrigin(sf::Vector2f(0.f, labelBounds.position.y + labelBounds.size.y / 2.0f));
        mLabelText.setPosition(sf::Vector2f(x, y + 10.f));

        mValText.setCharacterSize(24);
        mValText.setFillColor(sf::Color::Black);
        mValText.setString("1.5x");
        sf::FloatRect valBounds = mValText.getLocalBounds();
        mValText.setOrigin(sf::Vector2f(0.f, valBounds.position.y + valBounds.size.y / 2.0f));
        mValText.setPosition(sf::Vector2f(x + 95.f, y + 10.f));

        mTrack.setSize(sf::Vector2f(250.f, 8.f));
        mTrack.setOrigin(sf::Vector2f(0.f, 4.f));
        mTrack.setFillColor(sf::Color(200, 200, 200));
        mTrack.setPosition(sf::Vector2f(x + 165.f, y));

        mFill.setSize(sf::Vector2f(125.f, 8.f));
        mFill.setOrigin(sf::Vector2f(0.f, 4.f));
        mFill.setFillColor(sf::Color(90, 150, 44));
        mFill.setPosition(sf::Vector2f(x + 165.f, y));

        mThumb.setRadius(10.f);
        mThumb.setOrigin(sf::Vector2f(10.f, 10.f));
        mThumb.setFillColor(sf::Color(90, 150, 44));
        updateThumbPos();
    }

    void updateThumbPos(){
        float percent = (mCurrentVal - mMinVal) / (mMaxVal - mMinVal);
        float thumbX = mTrack.getPosition().x + percent * mTrack.getSize().x;

        mThumb.setPosition(sf::Vector2f(thumbX, mTrack.getPosition().y));
        mFill.setSize(sf::Vector2f(percent * mTrack.getSize().x, 8.f));

        std::stringstream stream;
        stream << std::fixed << std::setprecision(1) << mCurrentVal << "x";
        mValText.setString(stream.str());
    }

    void update(const sf::Vector2i& mousePos){
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
            sf::FloatRect trackBounds(sf::Vector2f(mTrack.getPosition().x - 10.f, mTrack.getPosition().y - 15.f), sf::Vector2f(mTrack.getSize().x + 20.f, 30.f));
            if (trackBounds.contains(sf::Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) or mIsDragging){
                mIsDragging = true;
                float newX = std::max(mTrack.getPosition().x, std::min(static_cast<float>(mousePos.x), mTrack.getPosition().x + mTrack.getSize().x));
                float percent = (newX - mTrack.getPosition().x) / mTrack.getSize().x;
                mCurrentVal = mMinVal + percent * (mMaxVal - mMinVal);
                updateThumbPos();
            }
        } else {
            mIsDragging = false;
        }
    }

    void draw(sf::RenderTarget& target){
        mFill.setFillColor(ThemeManager::current.primary);
        mThumb.setFillColor(ThemeManager::current.primary);
        
        mTrack.setFillColor(ThemeManager::current.screenBg);
        
        mLabelText.setFillColor(ThemeManager::current.textColor);
        mValText.setFillColor(ThemeManager::current.textColor);
            
        target.draw(mLabelText);
        target.draw(mValText);
        target.draw(mTrack);
        target.draw(mFill);
        target.draw(mThumb);
    }
    
    float getSpeed() const {return mCurrentVal;}
};

class ImageButton {
private:
    std::function<void()> mCallback;
    bool mIsHovered = false;
    sf::Color mNormalColor = sf::Color::White;
    sf::Color mHoverColor = sf::Color(200, 200, 200);

public:
    std::optional<sf::Sprite> mSprite;
    
    ImageButton() = default;

    void setup(const sf::Texture& texture, float x, float y, float targetW, float targetH){
        mSprite.emplace(texture);
        sf::FloatRect bounds = mSprite->getLocalBounds();
        mSprite->setScale(sf::Vector2f(targetW / bounds.size.x, targetH / bounds.size.y));
        mSprite->setPosition(sf::Vector2f(x, y));
        mSprite->setColor(mNormalColor);
    }

    void update(const sf::Vector2i& mousePos){
        if (!mSprite){return;}
        
        sf::FloatRect bounds = mSprite->getGlobalBounds();
        if (bounds.contains(sf::Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))){
            if (!mIsHovered) mSprite->setColor(mHoverColor);
            mIsHovered = true;
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) and mCallback){
                mCallback();
            }
        } else {
            if (mIsHovered) mSprite->setColor(mNormalColor);
            mIsHovered = false;
        }
    }

    void draw(sf::RenderTarget& target) const {
        if (mSprite) target.draw(*mSprite);
    }

    void setThemeColor(sf::Color color){
        mNormalColor = color;
        mHoverColor = sf::Color(std::max(0, color.r - 20), std::max(0, color.g - 20), std::max(0, color.b - 20));
        if (mSprite){
            mSprite->setColor(mIsHovered ? mHoverColor : mNormalColor);
        }
    }

    void setCallback(std::function<void()> cb){mCallback = cb;}
};
