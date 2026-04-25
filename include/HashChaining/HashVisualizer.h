#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <optional>

#include "AppLayout.h"
#include "GUI.h"
#include "HashChaining/HashChaining.h"

class HashVisualizer {
private:
    sf::RenderWindow* mWindow;
    sf::Font mFontRegular;
    sf::Font mFontBold;

    HashChaining mHash;
    AppLayout mLayout;

    // ── Title labels (bottom-left, mirroring RBT "03 / Red-Black / Tree") ──
    sf::Text mTitleNum;
    sf::Text mTitleHash;
    sf::Text mTitleChain;

    // ── Playback image buttons ──
    sf::Texture mSkipBackTex;
    sf::Texture mPauseTex;
    sf::Texture mSkipForwardTex;
    sf::Texture mStartTex;

    ImageButton mSkipBackBtn;
    ImageButton mPauseBtn;
    ImageButton mSkipForwardBtn;
    ImageButton mStartBtn;

    sf::Clock mAutoPlayClock;
    bool mIsPaused = false;

    // ── Step counter pill ──
    sf::CircleShape    mStepBgLeft;
    sf::CircleShape    mStepBgRight;
    sf::RectangleShape mStepBgCenter;
    sf::Text           mStepText;

    RoundedButton mStepBackBtn;
    RoundedButton mStepForwardBtn;

    // ── Sidebar action buttons ──
    RoundedButton mClearBtn;
    RoundedButton mNewBtn;
    RoundedButton mInsertBtn;
    RoundedButton mDeleteBtn;
    RoundedButton mSearchBtn;
    RoundedButton mUndoBtn;

    SpeedSlider mSpeedSlider;

    bool mGoHome     = false;
    bool mShowUndoBtn = false;

    // ── Expand states (only one open at a time) ──
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

    // ── New expand widgets (Random / From File) ──
    RoundedButton mNewHoverStroke;
    RoundedButton mNewExpandedStroke;
    RoundedButton mNewExpandedBg;
    RoundedButton mRandomBtn;
    RoundedButton mUploadBtn;

    // ── Animation / step state ──
    int   mCurrentStep = 0;   // index into mHash.getSteps()
    float mStepAnimProgress = 1.0f;
    int   mTargetStep = -1;

    // ── Bucket / node rendering helpers ──
    // (kept from original HashVisualizer)
    float mBucketStartX = 0.f;
    float mBucketRowY   = 0.f;
    float mBucketW      = 80.f;
    float mBucketH      = 58.f;
    float mBucketGapX   = 88.f;

    // ── Private helpers ──
    void resetPlayUI();
    void runAction(int action, int value, int oldValue = -1);

    void drawRoundedRect(sf::RenderWindow& window, const sf::FloatRect& rect,
                         float radius, const sf::Color& fill,
                         const sf::Color& border, float borderThick = 2.f);
    void drawArrowUp(sf::RenderWindow& window, sf::Vector2f from,
                     sf::Vector2f to, const sf::Color& color);

    void renderBuckets(sf::RenderWindow& window);
    void renderNodes  (sf::RenderWindow& window);

public:
    HashVisualizer(sf::RenderWindow& window);
    void update(const std::optional<sf::Event>& event);
    void render(bool showUI = true);
    bool checkReturnHome();
    void setTransitionProgress(float p) { /* unused but keeps call-site compatible */ }
};

