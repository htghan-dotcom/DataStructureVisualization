#pragma once
#include <SFML/Graphics.hpp>
#include "AppLayout.h"
#include "RedBlackTRee/RedBlackTree.h"
#include <optional>
#include <map>
#include <string>

class RBTVisualizer {
private:
    sf::RenderWindow* mWindow;
    sf::Font mFontRegular;
    sf::Font mFontBold;
    RedBlackTree mTree;
    
    AppLayout mLayout;

    sf::Text mTitleNum;
    sf::Text mTitleRed;
    sf::Text mTitleBlack;
    sf::Text mTitleTree;
    
    sf::Clock mAutoPlayClock;

    RoundedButton mClearBtn, mUndoBtn, mNewBtn, mInsertBtn, mDeleteBtn, mSearchBtn;
    
    bool mGoHome = false;
    float mTransitionProgress = 1.0f;
    
    bool mShowUndoBtn = false;
    
    bool mIsInsertExpanded = false;
    bool mIsDeleteExpanded = false;
    bool mIsSearchExpanded = false;
    bool mIsNewExpanded = false;
    
    std::string mInputValue = "";
    sf::Clock mCursorClock;
    float mStepAnimProgress = 1.0f;
    int mTargetStep = -1;
    bool mShowCursor = true;

    sf::Texture mDiceTex;

    RoundedButton mInsertHoverStroke;
    RoundedButton mInsertExpandedStroke;
    RoundedButton mInsertExpandedBg;
    RoundedButton mConfirmAddBtn;
    ImageButton mInsertDiceBtn;
    sf::Text mInsertInputText;
    sf::RectangleShape mInsertCursorLine;

    RoundedButton mDeleteHoverStroke;
    RoundedButton mDeleteExpandedStroke;
    RoundedButton mDeleteExpandedBg;
    RoundedButton mConfirmRemoveBtn;
    ImageButton mDeleteDiceBtn;
    sf::Text mDeleteInputText;
    sf::RectangleShape mDeleteCursorLine;
    
    RoundedButton mSearchHoverStroke;
    RoundedButton mSearchExpandedStroke;
    RoundedButton mSearchExpandedBg;
    RoundedButton mConfirmSearchBtn;
    ImageButton mSearchDiceBtn;
    sf::Text mSearchInputText;
    sf::RectangleShape mSearchCursorLine;
    
    RoundedButton mNewHoverStroke;
    RoundedButton mNewExpandedStroke;
    RoundedButton mNewExpandedBg;
    RoundedButton mRandomBtn;
    RoundedButton mUploadBtn;

    void drawAnimatedTree(const StepState& stepA, const StepState& stepB, float animP);
    std::map<int, sf::Vector2f> computeLayout(const std::vector<NodeSnapshot>& nodes);

public:
    RBTVisualizer(sf::RenderWindow& window);
    void update(const std::optional<sf::Event>& event);
    void render(bool showUI = true);
    bool checkReturnHome();
    void generateRandomTree();
    bool isEmpty();
    void setTransitionProgress(float p){mTransitionProgress = p;}
    void resetPlayUI();
};