#pragma once
#include <SFML/Graphics.hpp>
#include "GUI.h"
#include "DoublyLinkedList/DoublyLinkedList.h"
#include "DoublyLinkedList/DLLStepState.h"
#include "AppLayout.h"
#include <optional>
#include <vector>
#include <string>
#include <map>

class DLLVisualizer {
private:
    sf::RenderWindow* mWindow;
    sf::Font mFontRegular;
    sf::Font mFontBold;
    
    DoublyLinkedList mList;
    AppLayout mLayout;
    
    sf::Text mTitleNum;
    sf::Text mTitleDoubly;
    sf::Text mTitleLinked;
    sf::Text mTitleList;
    
    sf::Clock mAutoPlayClock;

    RoundedButton mClearBtn, mUndoBtn, mNewBtn, mInsertBtn, mDeleteBtn, mSearchBtn, mUpdateBtn;
    
    bool mGoHome = false;
    float mTransitionProgress = 0.f;
    
    struct DLLLayout {
        float spacing;
        float xOffset;
        float y;
        float nodeRadius;
    };
    DLLLayout computeLayout(int nodeCount);
    
    bool mShowUndoBtn = false;
    
    bool mIsInsertExpanded = false;
    bool mIsDeleteExpanded = false;
    bool mIsSearchExpanded = false;
    bool mIsUpdateExpanded = false;
    bool mIsNewExpanded = false;
    
    bool mEditingOld = true;
    std::string mInputOld = "";
    std::string mInputNew = "";
    
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

    RoundedButton mUpdateHoverStroke;
    RoundedButton mUpdateExpandedStroke;
    RoundedButton mUpdateExpandedBg;
    RoundedButton mConfirmUpdateBtn;
    sf::Text mUpdateInputText;
    sf::RectangleShape mUpdateCursorLine;
    
    RoundedButton mNewHoverStroke;
    RoundedButton mNewExpandedStroke;
    RoundedButton mNewExpandedBg;
    RoundedButton mRandomBtn;
    RoundedButton mUploadBtn;
    
    // BIẾN TÀNG HÌNH MENU
    bool mIsControlsVisible = true;
    RoundedButton mHideControlsBtn;
    RoundedButton mShowControlsBtn;

    void renderNodeVisualization(const DLLStepState& state);
    void renderDoubleArrows(const DLLStepState& state);

public:
    DLLVisualizer(sf::RenderWindow& window);
    void update(const std::optional<sf::Event>& event);
    void render(bool showUI = true);
    bool checkReturnHome();
    void generateRandomList();
    bool isEmpty();
    void setTransitionProgress(float p){mTransitionProgress = p;}
    void resetPlayUI();
    void loadListFromFile();
};
