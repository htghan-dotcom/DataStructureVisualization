#pragma once
#include <SFML/Graphics.hpp>
#include "GUI.h"
#include "DoublyLinkedList.h"
#include "DLLStepState.h"
#include <optional>
#include <vector>
#include <string>
#include <map>

class DLLVisualizer {
private:
    // Window and rendering
    sf::RenderWindow* mWindow;
    sf::Font mFontRegular;
    sf::Font mFontBold;
    sf::Font mFontMono;  // For pseudocode display
    
    // Data structure
    DoublyLinkedList mList;
    
    // Background and layout
    sf::RectangleShape mBackground;
    sf::RectangleShape mFooter;
    sf::RectangleShape mDescriptionBox;
    sf::RectangleShape mCodePanelBox;  // Panel for pseudocode display
    
    // Header and title elements
    sf::Text mHeaderText;
    sf::Text mTitleNum;
    sf::Text mTitleDoubly;
    sf::Text mTitleLinked;
    sf::Text mTitleList;
    sf::Text mDescriptionText;
    
    // Control button textures
    sf::Texture mHomeTex;
    sf::Texture mSkipBackTex;
    sf::Texture mPauseTex;
    sf::Texture mSkipForwardTex;
    sf::Texture mStartTex;
    sf::Texture mDiceTex;
    
    // Control buttons
    ImageButton mHomeBtn;
    ImageButton mSkipBackBtn;
    ImageButton mPauseBtn;
    ImageButton mSkipForwardBtn;
    ImageButton mStartBtn;
    
    // Action buttons
    RoundedButton mClearBtn;
    RoundedButton mUndoBtn;
    RoundedButton mNewBtn;
    RoundedButton mInsertBtn;
    RoundedButton mDeleteBtn;
    RoundedButton mSearchBtn;
    RoundedButton mUpdateBtn;
    
    // Step controls
    sf::CircleShape mStepBgLeft, mStepBgRight;
    sf::RectangleShape mStepBgCenter;
    sf::Text mStepText;
    RoundedButton mStepBackBtn;
    RoundedButton mStepForwardBtn;
    
    // Speed slider
    SpeedSlider mSpeedSlider;
    
    // Input form components - Insert
    RoundedButton mInsertHoverStroke;
    RoundedButton mInsertExpandedStroke;
    RoundedButton mInsertExpandedBg;
    RoundedButton mConfirmAddBtn;
    ImageButton mInsertDiceBtn;
    sf::Text mInsertInputText;
    sf::RectangleShape mInsertCursorLine;
    bool mIsInsertExpanded = false;
    
    // Input form components - Delete
    RoundedButton mDeleteHoverStroke;
    RoundedButton mDeleteExpandedStroke;
    RoundedButton mDeleteExpandedBg;
    RoundedButton mConfirmRemoveBtn;
    ImageButton mDeleteDiceBtn;
    sf::Text mDeleteInputText;
    sf::RectangleShape mDeleteCursorLine;
    bool mIsDeleteExpanded = false;
    
    // Input form components - Search
    RoundedButton mSearchHoverStroke;
    RoundedButton mSearchExpandedStroke;
    RoundedButton mSearchExpandedBg;
    RoundedButton mConfirmSearchBtn;
    ImageButton mSearchDiceBtn;
    sf::Text mSearchInputText;
    sf::RectangleShape mSearchCursorLine;
    bool mIsSearchExpanded = false;

    RoundedButton mUpdateHoverStroke;
    RoundedButton mUpdateExpandedStroke;
    RoundedButton mUpdateExpandedBg;
    RoundedButton mConfirmUpdateBtn;
    sf::Text mUpdateInputText;
    sf::RectangleShape mUpdateCursorLine;
    bool mIsUpdateExpanded = false;
    bool mIsUpdatePhaseTwo = false;
    int mUpdateOldValue = 0;
    
    // New list form components
    RoundedButton mNewHoverStroke;
    RoundedButton mNewExpandedStroke;
    RoundedButton mNewExpandedBg;
    RoundedButton mRandomBtn;
    RoundedButton mUploadBtn;
    bool mIsNewExpanded = false;
    
    // Pseudocode display components
    sf::Text mCodeTitleText;
    std::vector<sf::Text> mPseudoCodeLines;
    std::vector<sf::Text> mCppCodeLines;
    int mCurrentCodeStep = -1;
    
    // State variables
    std::string mInputValue = "";
    bool mShowCursor = true;
    sf::Clock mCursorClock;
    sf::Clock mAutoPlayClock;
    
    bool mIsPaused = false;
    bool mGoHome = false;
    bool mShowUndoBtn = false;
    
    float mStepAnimProgress = 1.0f;
    int mTargetStep = -1;
    float mTransitionProgress = 1.0f;
    
    // Helper methods
    void initializeFonts();
    void initializeTextures();
    void initializeButtons();
    void initializeInputForms();
    void initializePseudoCodePanel();
    void refreshTheme();
    void renderNodeVisualization(const DLLStepState& state);
    void renderDoubleArrows(const DLLStepState& state);
    void renderPseudoCodePanel(const DLLStepState& state);
    void updatePseudoCodeLines(const DLLStepState& state);
    void handleMouseInput(const sf::Vector2f& worldPos);
    void handleKeyInput(const sf::Event::KeyPressed& keyEvent);
    void loadListFromFile();
    
public:
    DLLVisualizer(sf::RenderWindow& window);
    ~DLLVisualizer() = default;
    
    void update(const std::optional<sf::Event>& event);
    void render(bool showUI = true);
    
    bool checkReturnHome();
    void generateRandomList();
    bool isEmpty();
    void setTransitionProgress(float p) { mTransitionProgress = p; }
};