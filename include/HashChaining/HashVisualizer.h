#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <optional>
#include <unordered_map>

#include "AppLayout.h"
#include "GUI.h"
#include "ThemeManager.h"
#include "HashChaining/HashChaining.h"

class HashVisualizer {
private:
    sf::RenderWindow* mWindow;
    sf::Font mFontRegular;
    sf::Font mFontBold;
    sf::Font mFontMedium;

    HashChaining mHash;
    AppLayout    mLayout;

    // ── Title labels ──
    sf::Text mTitleNum;
    sf::Text mTitleHash;
    sf::Text mTitleChain;

    // ── Sidebar action buttons ──
    RoundedButton mClearBtn;
    RoundedButton mNewBtn;
    RoundedButton mInsertBtn;
    RoundedButton mDeleteBtn;
    RoundedButton mSearchBtn;
    RoundedButton mUndoBtn;
    RoundedButton mUpdateBtn;

    // Hide/Show Sidebar button
    RoundedButton mHideMenuBtn;
    RoundedButton mShowMenuBtn;
    bool mIsSidebarVisible = true;

    bool mGoHome      = false;
    bool mShowUndoBtn = false;

    // ── Expand states ──
    bool mIsInsertExpanded = false;
    bool mIsDeleteExpanded = false;
    bool mIsSearchExpanded = false;
    bool mIsNewExpanded    = false;

    // ── Shared text input ──
    std::string mInputValue;
    sf::Clock   mCursorClock;
    bool        mShowCursor = true;

    // ── Insert expand widgets ──
    sf::Texture   mDiceTex;
    RoundedButton mInsertHoverStroke;
    RoundedButton mInsertExpandedStroke;
    RoundedButton mInsertExpandedBg;
    RoundedButton mConfirmAddBtn;
    ImageButton   mInsertDiceBtn;
    sf::Text      mInsertInputText;
    sf::RectangleShape mInsertCursorLine;

    // ── Delete expand widgets ──
    RoundedButton mDeleteHoverStroke;
    RoundedButton mDeleteExpandedStroke;
    RoundedButton mDeleteExpandedBg;
    RoundedButton mConfirmRemoveBtn;
    ImageButton   mDeleteDiceBtn;
    sf::Text      mDeleteInputText;
    sf::RectangleShape mDeleteCursorLine;

    // ── Search expand widgets ──
    RoundedButton mSearchHoverStroke;
    RoundedButton mSearchExpandedStroke;
    RoundedButton mSearchExpandedBg;
    RoundedButton mConfirmSearchBtn;
    ImageButton   mSearchDiceBtn;
    sf::Text      mSearchInputText;
    sf::RectangleShape mSearchCursorLine;

    
    // ── Update expand widgets ──
    RoundedButton mUpdateHoverStroke;
    RoundedButton mUpdateExpandedStroke;
    RoundedButton mUpdateExpandedBg;
    RoundedButton mConfirmUpdateBtn;
    sf::Text      mUpdateInputText; 
    sf::RectangleShape mUpdateCursorLine;

    bool mIsUpdateExpanded = false;
    std::string mInputOld = "";
    std::string mInputNew = "";
    bool mEditingOld = true;

    // ── New expand widgets ──
    RoundedButton mNewHoverStroke;
    RoundedButton mNewExpandedStroke;
    RoundedButton mNewExpandedBg;
    RoundedButton mRandomBtn;
    RoundedButton mUploadBtn;

    // ── Step / autoplay state ──
    int       mCurrentStep      = 0;
    float     mStepAnimProgress = 1.0f;
    int       mTargetStep       = -1;
    sf::Clock mAutoPlayClock;

    // ── Bucket layout cache ──
    float mBucketStartX = 0.f;
    float mBucketRowY   = 0.f;
    float mBucketW      = 80.f;
    float mBucketH      = 58.f;
    float mBucketGapX   = 88.f;

    // ── Color cache ──
    std::unordered_map<int,int> mColorIdx;

    // ── Helpers ──
    void runAction(int action, int value, int oldValue = -1);
    void doRandom();

    void drawRoundedRect(sf::RenderWindow& window, const sf::FloatRect& rect,
                         float radius, const sf::Color& fill,
                         const sf::Color& border, float borderThick = 2.f);
    void drawArrowUp(sf::RenderWindow& window, sf::Vector2f from,
                     sf::Vector2f to, const sf::Color& color);

    void renderBuckets(sf::RenderWindow& window);
    void renderNodes  (sf::RenderWindow& window);

    // --- Color palette for hash table ---
    struct HashColorTheme {
        sf::Color bucketFill;
        sf::Color bucketBorder;
        sf::Color nodeFill;
        sf::Color nodeBorder;
        sf::Color arrow;

        sf::Color bucketHighlightFill;
        sf::Color bucketHighlightBorder;
        sf::Color nodeHighlightFill;
        sf::Color nodeHighlightBorder;
        
        sf::Color highlightText;
    };

    HashColorTheme getHashTheme();
public:
    HashVisualizer(sf::RenderWindow& window);
    void update(const std::optional<sf::Event>& event);
    void render(bool showUI = true);
    bool checkReturnHome();
    void setTransitionProgress(float) {}
};