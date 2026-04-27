#include "DLLVisualizer.h"
#include "ThemeManager.h"
#include "tinyfiledialogs.h"
#include "Common.h"
#include <random>
#include <cmath>
#include <sstream>

DLLVisualizer::DLLVisualizer(sf::RenderWindow& window)
    : mWindow(&window),
      mHeaderText(mFontBold), mTitleNum(mFontBold),
      mTitleDoubly(mFontBold), mTitleLinked(mFontBold), mTitleList(mFontBold), mDescriptionText(mFontRegular),
      mCodeTitleText(mFontBold),
      mStepText(mFontRegular),
      mClearBtn(mFontRegular, "Clear list", 70.f, 176.f, 160.f, 45.f, 21.f, ThemeManager::current.secondary),
      mNewBtn(mFontRegular, "New list", 251.f, 176.f, 160.f, 45.f, 21.f, ThemeManager::current.secondary),
      mInsertBtn(mFontRegular, "Insert", 70.f, 230.f, 160.f, 45.f, 21.f, ThemeManager::current.secondary),
      mDeleteBtn(mFontRegular, "Delete", 70.f, 284.f, 160.f, 45.f, 21.f, ThemeManager::current.secondary),
      mSearchBtn(mFontRegular, "Search", 70.f, 338.f, 160.f, 45.f, 21.f, ThemeManager::current.secondary),
      mUpdateBtn(mFontRegular, "Update", 70.f, 392.f, 160.f, 45.f, 21.f, ThemeManager::current.secondary),
      mUndoBtn(mFontRegular, "Undo", 70.f, 176.f, 160.f, 45.f, 21.f, ThemeManager::current.secondary),
      mSpeedSlider(mFontRegular, 956.f, 921.f),
      mStepBackBtn(mFontRegular, "Step back", 326.f, 896.f, 165.f, 49.f, 24.5f, ThemeManager::current.bg),
      mStepForwardBtn(mFontRegular, "Step forward", 504.f, 896.f, 201.f, 49.f, 24.5f, ThemeManager::current.bg),
      
      mInsertHoverStroke(mFontRegular, "", 68.f, 228.f, 164.f, 49.f, 23.f, ThemeManager::current.primary),
      mInsertExpandedStroke(mFontRegular, "", 68.f, 228.f, 345.f, 49.f, 23.f, ThemeManager::current.primary),
      mInsertExpandedBg(mFontRegular, "", 70.f, 230.f, 341.f, 45.f, 21.f, ThemeManager::current.secondary),
      mConfirmAddBtn(mFontRegular, "Add", 254.f, 233.f, 154.f, 39.f, 19.5f, ThemeManager::current.bg),
      mInsertInputText(mFontRegular),

      mDeleteHoverStroke(mFontRegular, "", 68.f, 282.f, 164.f, 49.f, 23.f, ThemeManager::current.primary),
      mDeleteExpandedStroke(mFontRegular, "", 68.f, 282.f, 345.f, 49.f, 23.f, ThemeManager::current.primary),
      mDeleteExpandedBg(mFontRegular, "", 70.f, 284.f, 341.f, 45.f, 21.f, ThemeManager::current.secondary),
      mConfirmRemoveBtn(mFontRegular, "Remove", 254.f, 287.f, 154.f, 39.f, 19.5f, ThemeManager::current.bg),
      mDeleteInputText(mFontRegular),

      mSearchHoverStroke(mFontRegular, "", 68.f, 336.f, 164.f, 49.f, 23.f, ThemeManager::current.primary),
      mSearchExpandedStroke(mFontRegular, "", 68.f, 336.f, 345.f, 49.f, 23.f, ThemeManager::current.primary),
      mSearchExpandedBg(mFontRegular, "", 70.f, 338.f, 341.f, 45.f, 21.f, ThemeManager::current.secondary),
      mConfirmSearchBtn(mFontRegular, "Search", 254.f, 341.f, 154.f, 39.f, 19.5f, ThemeManager::current.bg),
      mSearchInputText(mFontRegular),

      mUpdateHoverStroke(mFontRegular, "", 68.f, 390.f, 164.f, 49.f, 23.f, ThemeManager::current.primary),
      mUpdateExpandedStroke(mFontRegular, "", 68.f, 390.f, 345.f, 49.f, 23.f, ThemeManager::current.primary),
      mUpdateExpandedBg(mFontRegular, "", 70.f, 392.f, 341.f, 45.f, 21.f, ThemeManager::current.secondary),
      mConfirmUpdateBtn(mFontRegular, "Update", 254.f, 395.f, 154.f, 39.f, 19.5f, ThemeManager::current.bg),
      mUpdateInputText(mFontRegular),
      mUpdateCursorLine(),

      mNewHoverStroke(mFontRegular, "", 249.f, 174.f, 164.f, 49.f, 23.f, ThemeManager::current.primary),
      mNewExpandedStroke(mFontRegular, "", 249.f, 174.f, 164.f, 157.f, 23.f, ThemeManager::current.primary),
      mNewExpandedBg(mFontRegular, "", 251.f, 176.f, 160.f, 153.f, 21.f, ThemeManager::current.secondary),
      mRandomBtn(mFontRegular, "Random", 254.f, 233.f, 154.f, 39.f, 19.5f, ThemeManager::current.bg),
      mUploadBtn(mFontRegular, "From File", 254.f, 287.f, 154.f, 39.f, 19.5f, ThemeManager::current.bg)
{
    initializeFonts();
    initializeTextures();
    initializeButtons();
    initializeInputForms();
    initializePseudoCodePanel();

    mBackground.setSize({1440.f, 960.f});
    mBackground.setFillColor(ThemeManager::current.bg);

    mFooter.setSize({1440.f, 78.f});
    mFooter.setPosition({0.f, 882.f});
    mFooter.setFillColor(ThemeManager::current.secondary);

    mHeaderText.setString("Data Structure Visualization");
    mHeaderText.setCharacterSize(30);
    mHeaderText.setFillColor(ThemeManager::current.textColor);
    mHeaderText.setPosition({130.f, 68.f});

    mTitleNum.setString("02");
    mTitleNum.setCharacterSize(30);
    mTitleNum.setFillColor(ThemeManager::current.textColor);
    mTitleNum.setPosition(sf::Vector2f(67.f, 663.f));

    mTitleDoubly.setString("Doubly");
    mTitleDoubly.setCharacterSize(70);
    mTitleDoubly.setFillColor(ThemeManager::current.primary);
    mTitleDoubly.setPosition(sf::Vector2f(67.f, 695.f));
    
    mTitleLinked.setString("Linked");
    mTitleLinked.setCharacterSize(70);
    mTitleLinked.setFillColor(ThemeManager::current.primaryLight);
    mTitleLinked.setPosition(sf::Vector2f(67.f + mTitleDoubly.getLocalBounds().size.x + 10.f, 695.f));

    mTitleList.setString("List");
    mTitleList.setCharacterSize(70);
    mTitleList.setFillColor(ThemeManager::current.secondary);
    mTitleList.setPosition(sf::Vector2f(67.f, 765.f));
    
    // Description box
    mDescriptionBox.setSize({400.f, 150.f});
    mDescriptionBox.setFillColor(ThemeManager::current.secondary);
    mDescriptionBox.setPosition({1018.f, 100.f});

    mDescriptionText.setString("Ready to visualize");
    mDescriptionText.setCharacterSize(16);
    mDescriptionText.setFillColor(ThemeManager::current.textColor);
    mDescriptionText.setPosition({1040.f, 120.f});

    // Step display background
    float bgX = 131.f, bgY = 890.f, bgW = 580.f, bgH = 61.f, bgR = 30.5f;
    sf::Color bgColor(196, 196, 196);
        
    mStepBgLeft.setRadius(bgR);
    mStepBgLeft.setPosition(sf::Vector2f(bgX, bgY));
    mStepBgLeft.setFillColor(ThemeManager::current.screenBg);
    
    mStepBgRight.setRadius(bgR);
    mStepBgRight.setPosition(sf::Vector2f(bgX + bgW - bgH, bgY));
    mStepBgRight.setFillColor(ThemeManager::current.screenBg);
    
    mStepBgCenter.setSize(sf::Vector2f(bgW - bgH, bgH));
    mStepBgCenter.setPosition(sf::Vector2f(bgX + bgR, bgY));
    mStepBgCenter.setFillColor(ThemeManager::current.screenBg);

    mStepText.setString("Step 0 / 0");
    mStepText.setCharacterSize(21);
    mStepText.setFillColor(sf::Color::Black);
    mStepText.setPosition(sf::Vector2f(192.f, 906.f));

    // Set up callbacks
    mClearBtn.setCallback([this]() {
        mList.backup();
        mList.initialize();
        mList.resetHistory("List cleared");
        mDescriptionText.setString("List cleared.");
        mShowUndoBtn = true;
    });
    
    mUndoBtn.setCallback([this]() {
        mList.restore();
        mDescriptionText.setString("List restored.");
        mShowUndoBtn = false;
    });
    
    mRandomBtn.setCallback([this]() {
        mList.backup();
        generateRandomList();
        mShowUndoBtn = false;
        mIsNewExpanded = false;
    });

    mUploadBtn.setCallback([this]() {
        mList.backup();
        loadListFromFile();
    });

    mStepBackBtn.setCallback([this]() {
        int cur = mList.getCurrentStep();
        if (cur > 0) {
            mList.setCurrentStep(cur - 1);
        }
    });

    mStepForwardBtn.setCallback([this]() {
        int cur = mList.getCurrentStep();
        auto history = mList.getStepHistory();
        if (cur < static_cast<int>(history.size()) - 1) {
            mList.setCurrentStep(cur + 1);
        }
    });

    mSkipBackBtn.setCallback([this]() {
        mList.setCurrentStep(0);
        mIsPaused = true;
    });

    mPauseBtn.setCallback([this]() {
        mIsPaused = true;
    });

    mStartBtn.setCallback([this]() {
        mIsPaused = false;
    });

    mConfirmAddBtn.setCallback([this]() {
        if (!mInputValue.empty()) {
            int val = stoi(mInputValue);
            mList.backup();
            mList.resetHistory("Insert " + to_string(val));
            mList.insertTail(val);
            mList.setCurrentStep(0);
            mIsInsertExpanded = false;
            mInputValue = "";
            mIsPaused = false;
            mShowUndoBtn = false;
            mAutoPlayClock.restart();
        }
    });

    mConfirmRemoveBtn.setCallback([this]() {
        if (!mInputValue.empty()) {
            int val = stoi(mInputValue);
            mList.backup();
            mList.resetHistory("Delete " + to_string(val));
            mList.deleteValue(val);
            mList.setCurrentStep(0);
            mIsDeleteExpanded = false;
            mInputValue = "";
            mIsPaused = false;
            mShowUndoBtn = false;
            mAutoPlayClock.restart();
        }
    });

    mConfirmSearchBtn.setCallback([this]() {
        if (!mInputValue.empty()) {
            int val = stoi(mInputValue);
            mList.resetHistory("Search for " + to_string(val));
            int idx = mList.search(val);
            if (idx != -1) {
                mDescriptionText.setString("Found at index: " + to_string(idx));
            } else {
                mDescriptionText.setString("Value not found");
            }
            mList.setCurrentStep(0);
            mIsSearchExpanded = false;
            mInputValue = "";
            mIsPaused = false;
            mAutoPlayClock.restart();
        }
    });

    mConfirmUpdateBtn.setCallback([this]() {
        if (!mInputValue.empty()) {
            if (!mIsUpdatePhaseTwo) {
                try {
                    mUpdateOldValue = stoi(mInputValue);
                    mInputValue.clear();
                    mIsUpdatePhaseTwo = true;
                    mDescriptionText.setString("Enter new value for " + to_string(mUpdateOldValue));
                } catch (...) {
                    mDescriptionText.setString("Enter a valid old value first.");
                }
            } else {
                try {
                    int newVal = stoi(mInputValue);
                    mList.backup();
                    if (mList.update(mUpdateOldValue, newVal)) {
                        mList.resetHistory("Update " + to_string(mUpdateOldValue) + " to " + to_string(newVal));
                        mList.setCurrentStep(0);
                        mDescriptionText.setString("Updated " + to_string(mUpdateOldValue) + " to " + to_string(newVal));
                        mShowUndoBtn = false;
                    } else {
                        mList.resetHistory("Value not found");
                        mDescriptionText.setString("Value not found");
                    }
                    mIsUpdateExpanded = false;
                    mIsUpdatePhaseTwo = false;
                    mInputValue.clear();
                    mIsPaused = false;
                    mAutoPlayClock.restart();
                } catch (...) {
                    mDescriptionText.setString("Enter a valid new value.");
                }
            }
        }
    });

    mClearBtn.refreshText();
    mNewBtn.refreshText();
    mInsertBtn.refreshText();
    mDeleteBtn.refreshText();
    mSearchBtn.refreshText();
    mUpdateBtn.refreshText();
    mStepBackBtn.refreshText();
    mStepForwardBtn.refreshText();
    mConfirmAddBtn.refreshText();
    mConfirmRemoveBtn.refreshText();
    mConfirmSearchBtn.refreshText();
    mConfirmUpdateBtn.refreshText();
    mUndoBtn.refreshText();
    mRandomBtn.refreshText();
    mUploadBtn.refreshText();
    mUpdateBtn.refreshText();
    mConfirmUpdateBtn.refreshText();

    mList.initialize();
    generateRandomList();
}

void DLLVisualizer::initializeFonts() {
    if (!mFontBold.openFromFile("assets/fonts/Inter-Bold.ttf")) {
        cerr << "Cannot load Inter-Bold.ttf" << endl;
    }
    if (!mFontRegular.openFromFile("assets/fonts/Inter-Regular.ttf")) {
        cerr << "Cannot load Inter-Regular.ttf" << endl;
    }
    if (!mFontMono.openFromFile("assets/fonts/Inter-Regular.ttf")) {
        mFontMono = mFontRegular; // Fallback if monospace font not available
    }
}

void DLLVisualizer::initializeTextures() {
    if (!mHomeTex.loadFromFile("assets/images/homeButton.png")) cerr << "Cannot load homeButton.png\n";
    if (!mSkipBackTex.loadFromFile("assets/images/skipbackButton.png")) cerr << "Cannot load skipbackButton.png\n";
    if (!mPauseTex.loadFromFile("assets/images/pauseButton.png")) cerr << "Cannot load pauseButton.png\n";
    if (!mSkipForwardTex.loadFromFile("assets/images/skipforwardButton.png")) cerr << "Cannot load skipforwardButton.png\n";
    if (!mStartTex.loadFromFile("assets/images/startButton.png")) cerr << "Cannot load startButton.png\n";
    if (!mDiceTex.loadFromFile("assets/images/randomButton.png")) cerr << "Cannot load randomButton.png\n";

    mHomeTex.setSmooth(true);
    mSkipBackTex.setSmooth(true);
    mPauseTex.setSmooth(true);
    mSkipForwardTex.setSmooth(true);
    mStartTex.setSmooth(true);
    mDiceTex.setSmooth(true);
}

void DLLVisualizer::initializeButtons() {
    mHomeBtn.setup(mHomeTex, 67.f, 62.f, 48.f, 48.f);
    mSkipBackBtn.setup(mSkipBackTex, 333.f, 897.f, 48.f, 48.f);
    mPauseBtn.setup(mPauseTex, 397.f, 897.f, 48.f, 48.f);
    mSkipForwardBtn.setup(mSkipForwardTex, 461.f, 897.f, 48.f, 48.f);
    mStartBtn.setup(mStartTex, 134.f, 897.f, 48.f, 48.f);

    mHomeBtn.setCallback([this]() { mGoHome = true; });
}

void DLLVisualizer::initializeInputForms() {
    mInsertInputText.setFont(mFontRegular);
    mInsertInputText.setCharacterSize(22);
    mInsertInputText.setFillColor(ThemeManager::current.textColor);
    mInsertInputText.setPosition(sf::Vector2f(85.f, 238.f));
    
    mDeleteInputText.setFont(mFontRegular);
    mDeleteInputText.setCharacterSize(22);
    mDeleteInputText.setFillColor(ThemeManager::current.textColor);
    mDeleteInputText.setPosition(sf::Vector2f(85.f, 292.f));
    
    mSearchInputText.setFont(mFontRegular);
    mSearchInputText.setCharacterSize(22);
    mSearchInputText.setFillColor(ThemeManager::current.textColor);
    mSearchInputText.setPosition(sf::Vector2f(85.f, 346.f));
    
    mUpdateInputText.setFont(mFontRegular);
    mUpdateInputText.setCharacterSize(22);
    mUpdateInputText.setFillColor(ThemeManager::current.textColor);
    mUpdateInputText.setPosition(sf::Vector2f(85.f, 400.f));

    mInsertCursorLine.setSize(sf::Vector2f(2.f, 24.f));
    mInsertCursorLine.setFillColor(ThemeManager::current.textColor);
    
    mDeleteCursorLine.setSize(sf::Vector2f(2.f, 24.f));
    mDeleteCursorLine.setFillColor(ThemeManager::current.textColor);
    
    mSearchCursorLine.setSize(sf::Vector2f(2.f, 24.f));
    mSearchCursorLine.setFillColor(ThemeManager::current.textColor);

    mUpdateCursorLine.setSize(sf::Vector2f(2.f, 24.f));
    mUpdateCursorLine.setFillColor(ThemeManager::current.textColor);

    mInsertDiceBtn.setup(mDiceTex, 205.f, 237.f, 30.f, 30.f);
    mInsertDiceBtn.setCallback([this]() {
        mInputValue = to_string(rand() % 99 + 1);
    });
    
    mDeleteDiceBtn.setup(mDiceTex, 205.f, 291.f, 30.f, 30.f);
    mDeleteDiceBtn.setCallback([this]() {
        mInputValue = to_string(rand() % 99 + 1);
    });
    
    mSearchDiceBtn.setup(mDiceTex, 205.f, 345.f, 30.f, 30.f);
    mSearchDiceBtn.setCallback([this]() {
        mInputValue = to_string(rand() % 99 + 1);
    });
}

void DLLVisualizer::initializePseudoCodePanel() {
    mCodePanelBox.setSize({380.f, 200.f});
    mCodePanelBox.setFillColor(ThemeManager::current.screenBg);
    mCodePanelBox.setOutlineThickness(2.f);
    mCodePanelBox.setOutlineColor(ThemeManager::current.textColor);
    mCodePanelBox.setPosition({1020.f, 260.f});

    mCodeTitleText.setFont(mFontBold);
    mCodeTitleText.setString("Step Code");
    mCodeTitleText.setCharacterSize(14);
    mCodeTitleText.setFillColor(ThemeManager::current.textColor);
    mCodeTitleText.setPosition({1030.f, 270.f});
}

void DLLVisualizer::refreshTheme() {
    mBackground.setFillColor(ThemeManager::current.bg);
    mFooter.setFillColor(ThemeManager::current.secondary);
    mDescriptionBox.setFillColor(ThemeManager::current.secondary);
    mDescriptionText.setFillColor(ThemeManager::current.textColor);
    mHeaderText.setFillColor(ThemeManager::current.textColor);
    mTitleNum.setFillColor(ThemeManager::current.textColor);
    mTitleDoubly.setFillColor(ThemeManager::current.primary);
    mTitleLinked.setFillColor(ThemeManager::current.primaryLight);
    mTitleList.setFillColor(ThemeManager::current.secondary);
    mCodePanelBox.setFillColor(ThemeManager::current.screenBg);
    mCodePanelBox.setOutlineColor(ThemeManager::current.textColor);
    mCodeTitleText.setFillColor(ThemeManager::current.textColor);
    mStepBgLeft.setFillColor(ThemeManager::current.screenBg);
    mStepBgRight.setFillColor(ThemeManager::current.screenBg);
    mStepBgCenter.setFillColor(ThemeManager::current.screenBg);
    mStepText.setFillColor(ThemeManager::current.textColor);

    mInsertInputText.setFillColor(ThemeManager::current.textColor);
    mDeleteInputText.setFillColor(ThemeManager::current.textColor);
    mSearchInputText.setFillColor(ThemeManager::current.textColor);
    mUpdateInputText.setFillColor(ThemeManager::current.textColor);
    mInsertCursorLine.setFillColor(ThemeManager::current.textColor);
    mDeleteCursorLine.setFillColor(ThemeManager::current.textColor);
    mSearchCursorLine.setFillColor(ThemeManager::current.textColor);
    mUpdateCursorLine.setFillColor(ThemeManager::current.textColor);

    mClearBtn.setThemeColor(ThemeManager::current.secondary);
    mNewBtn.setThemeColor(ThemeManager::current.secondary);
    mInsertBtn.setThemeColor(ThemeManager::current.secondary);
    mDeleteBtn.setThemeColor(ThemeManager::current.secondary);
    mSearchBtn.setThemeColor(ThemeManager::current.secondary);
    mUpdateBtn.setThemeColor(ThemeManager::current.secondary);
    mUndoBtn.setThemeColor(ThemeManager::current.secondary);
    mStepBackBtn.setThemeColor(ThemeManager::current.bg);
    mStepForwardBtn.setThemeColor(ThemeManager::current.bg);

    mInsertHoverStroke.setThemeColor(ThemeManager::current.primary);
    mInsertExpandedStroke.setThemeColor(ThemeManager::current.primary);
    mDeleteHoverStroke.setThemeColor(ThemeManager::current.primary);
    mDeleteExpandedStroke.setThemeColor(ThemeManager::current.primary);
    mSearchHoverStroke.setThemeColor(ThemeManager::current.primary);
    mSearchExpandedStroke.setThemeColor(ThemeManager::current.primary);
    mUpdateHoverStroke.setThemeColor(ThemeManager::current.primary);
    mUpdateExpandedStroke.setThemeColor(ThemeManager::current.primary);
    mNewHoverStroke.setThemeColor(ThemeManager::current.primary);
    mNewExpandedStroke.setThemeColor(ThemeManager::current.primary);

    mInsertExpandedBg.setThemeColor(ThemeManager::current.secondary);
    mDeleteExpandedBg.setThemeColor(ThemeManager::current.secondary);
    mSearchExpandedBg.setThemeColor(ThemeManager::current.secondary);
    mUpdateExpandedBg.setThemeColor(ThemeManager::current.secondary);
    mNewExpandedBg.setThemeColor(ThemeManager::current.secondary);

    mConfirmAddBtn.setThemeColor(ThemeManager::current.bg);
    mConfirmRemoveBtn.setThemeColor(ThemeManager::current.bg);
    mConfirmSearchBtn.setThemeColor(ThemeManager::current.bg);
    mConfirmUpdateBtn.setThemeColor(ThemeManager::current.bg);
    mRandomBtn.setThemeColor(ThemeManager::current.bg);
    mUploadBtn.setThemeColor(ThemeManager::current.bg);

    mHomeBtn.setThemeColor(sf::Color(230, 57, 70));
    mPauseBtn.setThemeColor(sf::Color::Black);
    mStartBtn.setThemeColor(sf::Color::Black);
    mSkipBackBtn.setThemeColor(sf::Color::Black);
    mSkipForwardBtn.setThemeColor(sf::Color::Black);
}

void DLLVisualizer::update(const optional<sf::Event>& event) {
    sf::Vector2f worldPos = mWindow->mapPixelToCoords(sf::Mouse::getPosition(*mWindow));
    sf::Vector2i mousePos(static_cast<int>(worldPos.x), static_cast<int>(worldPos.y));
    
    // Update buttons
    if (mShowUndoBtn) {
        mUndoBtn.update(mousePos);
    } else {
        mClearBtn.update(mousePos);
    }
    
    mNewBtn.update(mousePos);
    if (mIsNewExpanded) {
        mRandomBtn.update(mousePos);
        mUploadBtn.update(mousePos);
    }
    mInsertBtn.update(mousePos);
    mDeleteBtn.update(mousePos);
    mSearchBtn.update(mousePos);
    mUpdateBtn.update(mousePos);
    
    // Update confirmation buttons when forms are expanded
    if (mIsInsertExpanded) {
        mConfirmAddBtn.update(mousePos);
        mInsertDiceBtn.update(mousePos);
    }
    if (mIsDeleteExpanded) {
        mConfirmRemoveBtn.update(mousePos);
        mDeleteDiceBtn.update(mousePos);
    }
    if (mIsSearchExpanded) {
        mConfirmSearchBtn.update(mousePos);
        mSearchDiceBtn.update(mousePos);
    }
    if (mIsUpdateExpanded) {
        mConfirmUpdateBtn.update(mousePos);
    }
    
    mSpeedSlider.update(mousePos);
    
    float speedMult = mSpeedSlider.getSpeed();
    if (speedMult > 0.0f) {
        int speedMs = static_cast<int>(500.f / speedMult);
        mList.setVisualizationSpeed(speedMs);
    }
    
    mHomeBtn.update(mousePos);
    mSkipBackBtn.update(mousePos);
    mSkipForwardBtn.update(mousePos);
    
    if (mIsPaused) {
        mStartBtn.update(mousePos);
        mStepBackBtn.update(mousePos);
        mStepForwardBtn.update(mousePos);
    } else {
        mPauseBtn.update(mousePos);
    }
    
    // Handle events
    if (event) {
        if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mouseEvent->button == sf::Mouse::Button::Left) {
                handleMouseInput(worldPos);
            }
        } else if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
            handleKeyInput(*keyEvent);
        } else if (const auto* textEvent = event->getIf<sf::Event::TextEntered>()) {
            if (mIsInsertExpanded || mIsDeleteExpanded || mIsSearchExpanded || mIsUpdateExpanded) {
                char c = static_cast<char>(textEvent->unicode);
                if ((c >= '0' && c <= '9') && mInputValue.length() < 7) {
                    mInputValue += c;
                } else if (mIsUpdateExpanded && c == ' ' && !mInputValue.empty() && mInputValue.find(' ') == std::string::npos) {
                    mInputValue += c;
                }
            }
        }
    }

    // Auto-play logic
    if (!mIsPaused && mAutoPlayClock.getElapsedTime().asMilliseconds() > 500) {
        int cur = mList.getCurrentStep();
        auto history = mList.getStepHistory();
        if (cur < static_cast<int>(history.size()) - 1) {
            mList.setCurrentStep(cur + 1);
        } else {
            mIsPaused = true;
        }
        mAutoPlayClock.restart();
    }
}

void DLLVisualizer::handleMouseInput(const sf::Vector2f& worldPos) {
    bool clickedInsideAnyForm = false;
    
    // Handle New button
    if (!mIsNewExpanded && sf::FloatRect(sf::Vector2f(251.f, 176.f), sf::Vector2f(160.f, 45.f)).contains(worldPos)) {
        mIsNewExpanded = true;
        mIsInsertExpanded = false;
        mIsDeleteExpanded = false;
        mIsSearchExpanded = false;
        clickedInsideAnyForm = true;
    } else if (mIsNewExpanded && sf::FloatRect(sf::Vector2f(251.f, 176.f), sf::Vector2f(160.f, 153.f)).contains(worldPos)) {
        clickedInsideAnyForm = true;
    }
    
    // Handle Insert button
    if (!mIsInsertExpanded && sf::FloatRect(sf::Vector2f(70.f, 230.f), sf::Vector2f(160.f, 45.f)).contains(worldPos)) {
        mIsInsertExpanded = true;
        mIsDeleteExpanded = false;
        mIsSearchExpanded = false;
        mIsNewExpanded = false;
        mInputValue = "";
        clickedInsideAnyForm = true;
    } else if (mIsInsertExpanded && sf::FloatRect(sf::Vector2f(70.f, 230.f), sf::Vector2f(341.f, 45.f)).contains(worldPos)) {
        clickedInsideAnyForm = true;
    }

    // Handle Delete button
    if (!mIsDeleteExpanded && sf::FloatRect(sf::Vector2f(70.f, 284.f), sf::Vector2f(160.f, 45.f)).contains(worldPos)) {
        mIsDeleteExpanded = true;
        mIsInsertExpanded = false;
        mIsSearchExpanded = false;
        mIsNewExpanded = false;
        mInputValue = "";
        clickedInsideAnyForm = true;
    } else if (mIsDeleteExpanded && sf::FloatRect(sf::Vector2f(70.f, 284.f), sf::Vector2f(341.f, 45.f)).contains(worldPos)) {
        clickedInsideAnyForm = true;
    }

    // Handle Search button
    if (!mIsSearchExpanded && sf::FloatRect(sf::Vector2f(70.f, 338.f), sf::Vector2f(160.f, 45.f)).contains(worldPos)) {
        mIsSearchExpanded = true;
        mIsInsertExpanded = false;
        mIsDeleteExpanded = false;
        mIsNewExpanded = false;
        mIsUpdateExpanded = false;
        mInputValue = "";
        clickedInsideAnyForm = true;
    } else if (mIsSearchExpanded && sf::FloatRect(sf::Vector2f(70.f, 338.f), sf::Vector2f(341.f, 45.f)).contains(worldPos)) {
        clickedInsideAnyForm = true;
    }

    // Handle Update button
    if (!mIsUpdateExpanded && sf::FloatRect(sf::Vector2f(70.f, 392.f), sf::Vector2f(160.f, 45.f)).contains(worldPos)) {
        mIsUpdateExpanded = true;
        mIsInsertExpanded = false;
        mIsDeleteExpanded = false;
        mIsSearchExpanded = false;
        mIsNewExpanded = false;
        mIsUpdatePhaseTwo = false;
        mInputValue = "";
        clickedInsideAnyForm = true;
    } else if (mIsUpdateExpanded && sf::FloatRect(sf::Vector2f(70.f, 392.f), sf::Vector2f(341.f, 45.f)).contains(worldPos)) {
        clickedInsideAnyForm = true;
    }

    if (!clickedInsideAnyForm) {
        mIsInsertExpanded = false;
        mIsDeleteExpanded = false;
        mIsSearchExpanded = false;
        mIsUpdateExpanded = false;
        mIsUpdatePhaseTwo = false;
        mIsNewExpanded = false;
    }
}

void DLLVisualizer::handleKeyInput(const sf::Event::KeyPressed& keyEvent) {
    // Undo
    if ((keyEvent.system || keyEvent.control) && keyEvent.code == sf::Keyboard::Key::Z) {
        mList.restore();
        mDescriptionText.setString("Undo successfully.");
        mShowUndoBtn = false;
        mIsPaused = true;
        mIsInsertExpanded = false;
        mIsDeleteExpanded = false;
        mIsSearchExpanded = false;
        mIsNewExpanded = false;
        mInputValue = "";
        return;
    }
     
    if (!mIsInsertExpanded && !mIsDeleteExpanded && !mIsSearchExpanded && !mIsUpdateExpanded) {
        if (keyEvent.code == sf::Keyboard::Key::I) {
            mIsInsertExpanded = true;
            mIsNewExpanded = false;
            mIsDeleteExpanded = false;
            mIsSearchExpanded = false;
            mIsUpdateExpanded = false;
        }
        else if (keyEvent.code == sf::Keyboard::Key::D) {
            mIsDeleteExpanded = true;
            mIsNewExpanded = false;
            mIsInsertExpanded = false;
            mIsSearchExpanded = false;
            mIsUpdateExpanded = false;
        }
        else if (keyEvent.code == sf::Keyboard::Key::S) {
            mIsSearchExpanded = true;
            mIsNewExpanded = false;
            mIsInsertExpanded = false;
            mIsDeleteExpanded = false;
            mIsUpdateExpanded = false;
        }
        else if (keyEvent.code == sf::Keyboard::Key::U) {
            mIsUpdateExpanded = true;
            mIsNewExpanded = false;
            mIsInsertExpanded = false;
            mIsDeleteExpanded = false;
            mIsSearchExpanded = false;
        }
        else if (keyEvent.code == sf::Keyboard::Key::N) {
            mIsNewExpanded = !mIsNewExpanded;
            mIsInsertExpanded = false;
            mIsDeleteExpanded = false;
            mIsSearchExpanded = false;
            mIsUpdateExpanded = false;
        }
        else if (keyEvent.code == sf::Keyboard::Key::R) {
            mList.backup();
            generateRandomList();
            mShowUndoBtn = false;
            mIsNewExpanded = false;
        }
        else if (keyEvent.code == sf::Keyboard::Key::Space) {
            mIsPaused = !mIsPaused;
        }
    }
    else if (mIsInsertExpanded || mIsDeleteExpanded || mIsSearchExpanded || mIsUpdateExpanded) {
        if (keyEvent.code == sf::Keyboard::Key::Backspace && !mInputValue.empty()) {
            mInputValue.pop_back();
        }
        else if (keyEvent.code == sf::Keyboard::Key::Enter && !mInputValue.empty()) {
            if (mIsInsertExpanded) {
                int val = stoi(mInputValue);
                mList.backup();
                mList.resetHistory("Insert " + to_string(val));
                mList.insertTail(val);
                mList.setCurrentStep(0);
                mIsInsertExpanded = false;
                mInputValue = "";
                mIsPaused = false;
                mShowUndoBtn = false;
                mAutoPlayClock.restart();
            }
            else if (mIsDeleteExpanded) {
                int val = stoi(mInputValue);
                mList.backup();
                mList.resetHistory("Delete " + to_string(val));
                mList.deleteValue(val);
                mList.setCurrentStep(0);
                mIsDeleteExpanded = false;
                mInputValue = "";
                mIsPaused = false;
                mShowUndoBtn = false;
                mAutoPlayClock.restart();
            }
            else if (mIsSearchExpanded) {
                int val = stoi(mInputValue);
                mList.resetHistory("Search for " + to_string(val));
                int idx = mList.search(val);
                if (idx != -1) {
                    mDescriptionText.setString("Found at index: " + to_string(idx));
                } else {
                    mDescriptionText.setString("Value not found");
                }
                mList.setCurrentStep(0);
                mIsSearchExpanded = false;
                mInputValue = "";
                mIsPaused = false;
                mShowUndoBtn = false;
                mAutoPlayClock.restart();
            }
            else if (mIsUpdateExpanded) {
                if (!mIsUpdatePhaseTwo) {
                    try {
                        mUpdateOldValue = stoi(mInputValue);
                        mInputValue.clear();
                        mIsUpdatePhaseTwo = true;
                        mDescriptionText.setString("Enter new value for " + to_string(mUpdateOldValue));
                    } catch (...) {
                        mDescriptionText.setString("Enter a valid old value first.");
                    }
                } else {
                    try {
                        int newVal = stoi(mInputValue);
                        mList.backup();
                        if (mList.update(mUpdateOldValue, newVal)) {
                            mList.resetHistory("Update " + to_string(mUpdateOldValue) + " to " + to_string(newVal));
                            mList.setCurrentStep(0);
                            mDescriptionText.setString("Updated " + to_string(mUpdateOldValue) + " to " + to_string(newVal));
                            mShowUndoBtn = false;
                        } else {
                            mList.resetHistory("Value not found");
                            mDescriptionText.setString("Value not found");
                        }
                        mIsUpdateExpanded = false;
                        mIsUpdatePhaseTwo = false;
                        mInputValue.clear();
                        mIsPaused = false;
                        mAutoPlayClock.restart();
                    } catch (...) {
                        mDescriptionText.setString("Enter a valid new value.");
                    }
                }
            }
        }
        else if (keyEvent.code == sf::Keyboard::Key::R) {
            mInputValue = to_string(rand() % 99 + 1);
        }
    }
}

void DLLVisualizer::render(bool showUI) {
    refreshTheme();
    if (showUI) {
        mWindow->draw(mBackground);
        mWindow->draw(mHeaderText);
        mWindow->draw(mFooter);
        mWindow->draw(mDescriptionBox);
        mWindow->draw(mDescriptionText);
    }

    auto history = mList.getStepHistory();
    int cur = mList.getCurrentStep();

    if (cur >= 0 && cur < static_cast<int>(history.size())) {
        const DLLStepState& state = history[cur];
        
        renderNodeVisualization(state);
        renderDoubleArrows(state);
        if (showUI) {
            renderPseudoCodePanel(state);
        }

        if (!mIsInsertExpanded && !mIsDeleteExpanded && !mIsSearchExpanded && !mIsUpdateExpanded) {
            mDescriptionText.setString(state.description);
        }
    }

    if (showUI) {
        mWindow->draw(mFooter);

        mWindow->draw(mTitleNum);
        mWindow->draw(mTitleDoubly);
        mWindow->draw(mTitleLinked);
        mWindow->draw(mTitleList);

        if (mShowUndoBtn) {
            mUndoBtn.draw(*mWindow);
        } else {
            mClearBtn.draw(*mWindow);
        }

        mNewBtn.draw(*mWindow);
        mInsertBtn.draw(*mWindow);
        mDeleteBtn.draw(*mWindow);
        mSearchBtn.draw(*mWindow);

        // New button expanded state
        if (mIsNewExpanded) {
            mNewExpandedStroke.draw(*mWindow);
            mNewExpandedBg.draw(*mWindow);
            mRandomBtn.draw(*mWindow);
            mUploadBtn.draw(*mWindow);
        } else {
            if (sf::FloatRect(sf::Vector2f(251.f, 176.f), sf::Vector2f(160.f, 45.f)).contains(mWindow->mapPixelToCoords(sf::Mouse::getPosition(*mWindow)))) {
                mNewHoverStroke.draw(*mWindow);
            }
            mNewBtn.draw(*mWindow);
        }

        // Insert form
        if (mIsInsertExpanded) {
            mInsertExpandedStroke.draw(*mWindow);
            mInsertExpandedBg.draw(*mWindow);
            mConfirmAddBtn.draw(*mWindow);
            mInsertDiceBtn.draw(*mWindow);
            
            mInsertInputText.setString(mInputValue);
            mWindow->draw(mInsertInputText);
                    
            if (mCursorClock.getElapsedTime().asSeconds() >= 0.5f) {
                mShowCursor = !mShowCursor;
                mCursorClock.restart();
            }
                
            if (mShowCursor) {
                float textWidth = mInsertInputText.getLocalBounds().size.x;
                float cursorX = mInputValue.empty() ? 85.f : 85.f + textWidth + 2.f;
                mInsertCursorLine.setPosition(sf::Vector2f(cursorX, 240.f));
                mWindow->draw(mInsertCursorLine);
            }
        } else {
            sf::Vector2f worldPos = mWindow->mapPixelToCoords(sf::Mouse::getPosition(*mWindow));
            if (sf::FloatRect(sf::Vector2f(70.f, 230.f), sf::Vector2f(160.f, 45.f)).contains(worldPos)) {
                mInsertHoverStroke.draw(*mWindow);
            }
            mInsertBtn.draw(*mWindow);
        }
        
        // Delete form
        if (mIsDeleteExpanded) {
            mDeleteExpandedStroke.draw(*mWindow);
            mDeleteExpandedBg.draw(*mWindow);
            mConfirmRemoveBtn.draw(*mWindow);
            mDeleteDiceBtn.draw(*mWindow);
            
            mDeleteInputText.setString(mInputValue);
            mWindow->draw(mDeleteInputText);
                    
            if (mCursorClock.getElapsedTime().asSeconds() >= 0.5f) {
                mShowCursor = !mShowCursor;
                mCursorClock.restart();
            }
                
            if (mShowCursor) {
                float textWidth = mDeleteInputText.getLocalBounds().size.x;
                float cursorX = mInputValue.empty() ? 85.f : 85.f + textWidth + 2.f;
                mDeleteCursorLine.setPosition(sf::Vector2f(cursorX, 294.f));
                mWindow->draw(mDeleteCursorLine);
            }
        } else {
            sf::Vector2f worldPos = mWindow->mapPixelToCoords(sf::Mouse::getPosition(*mWindow));
            if (sf::FloatRect(sf::Vector2f(70.f, 284.f), sf::Vector2f(160.f, 45.f)).contains(worldPos)) {
                mDeleteHoverStroke.draw(*mWindow);
            }
            mDeleteBtn.draw(*mWindow);
        }

        // Search form
        if (mIsSearchExpanded) {
            mSearchExpandedStroke.draw(*mWindow);
            mSearchExpandedBg.draw(*mWindow);
            mConfirmSearchBtn.draw(*mWindow);
            mSearchDiceBtn.draw(*mWindow);
                    
            mSearchInputText.setString(mInputValue);
            mWindow->draw(mSearchInputText);
                    
            if (mCursorClock.getElapsedTime().asSeconds() >= 0.5f) {
                mShowCursor = !mShowCursor;
                mCursorClock.restart();
            }
            
            if (mShowCursor) {
                float textWidth = mSearchInputText.getLocalBounds().size.x;
                float cursorX = mInputValue.empty() ? 85.f : 85.f + textWidth + 2.f;
                mSearchCursorLine.setPosition(sf::Vector2f(cursorX, 348.f));
                mWindow->draw(mSearchCursorLine);
            }
        } else {
            sf::Vector2f worldPos = mWindow->mapPixelToCoords(sf::Mouse::getPosition(*mWindow));
            if (sf::FloatRect(sf::Vector2f(70.f, 338.f), sf::Vector2f(160.f, 45.f)).contains(worldPos)) {
                mSearchHoverStroke.draw(*mWindow);
            }
            mSearchBtn.draw(*mWindow);
        }

        // Update form
        if (mIsUpdateExpanded) {
            mUpdateExpandedStroke.draw(*mWindow);
            mUpdateExpandedBg.draw(*mWindow);
            mConfirmUpdateBtn.draw(*mWindow);
                    
            mUpdateInputText.setString(mInputValue);
            mWindow->draw(mUpdateInputText);
                    
            if (mCursorClock.getElapsedTime().asSeconds() >= 0.5f) {
                mShowCursor = !mShowCursor;
                mCursorClock.restart();
            }
            
            if (mShowCursor) {
                float textWidth = mUpdateInputText.getLocalBounds().size.x;
                float cursorX = mInputValue.empty() ? 85.f : 85.f + textWidth + 2.f;
                mUpdateCursorLine.setPosition(sf::Vector2f(cursorX, 404.f));
                mWindow->draw(mUpdateCursorLine);
            }
        } else {
            sf::Vector2f worldPos = mWindow->mapPixelToCoords(sf::Mouse::getPosition(*mWindow));
            if (sf::FloatRect(sf::Vector2f(70.f, 392.f), sf::Vector2f(160.f, 45.f)).contains(worldPos)) {
                mUpdateHoverStroke.draw(*mWindow);
            }
            mUpdateBtn.draw(*mWindow);
        }
        
        mSpeedSlider.draw(*mWindow);
        mHomeBtn.draw(*mWindow);
        mSkipBackBtn.draw(*mWindow);
        mSkipForwardBtn.draw(*mWindow);
        
        if (mIsPaused) {
            mWindow->draw(mStepBgLeft);
            mWindow->draw(mStepBgRight);
            mWindow->draw(mStepBgCenter);
            
            int total = static_cast<int>(history.size());
            if (total == 0) {
                mStepText.setString("Step 0 / 0");
            } else {
                mStepText.setString("Step " + to_string(cur + 1) + " / " + to_string(total));
            }
                        
            mWindow->draw(mStepText);
            mStartBtn.draw(*mWindow);
            mStepBackBtn.draw(*mWindow);
            mStepForwardBtn.draw(*mWindow);
        } else {
            mPauseBtn.draw(*mWindow);
        }
    }
}

void DLLVisualizer::renderNodeVisualization(const DLLStepState& state) {
    float nodeRadius = 30.f;
    float spacing = 120.f;
    sf::Vector2f startPos(200.f, 550.f);

    for (size_t i = 0; i < state.nodes.size(); ++i) {
        sf::Vector2f pos = startPos + sf::Vector2f(i * spacing, 0.f);

        // Draw node circle
        sf::CircleShape circle(nodeRadius);
        circle.setOrigin({nodeRadius, nodeRadius});
        circle.setPosition(pos);
        circle.setOutlineThickness(3.f);
        circle.setOutlineColor(state.nodes[i].isHighlighted ? sf::Color::Yellow : sf::Color::Black);
        circle.setFillColor(sf::Color(52, 152, 219));
        mWindow->draw(circle);

        // Draw value
        sf::Text text(mFontRegular, to_string(state.nodes[i].val), 20);
        auto bounds = text.getLocalBounds();
        text.setPosition(sf::Vector2f(pos.x - bounds.size.x / 2.f, pos.y - bounds.size.y / 2.f));
        text.setFillColor(sf::Color::White);
        mWindow->draw(text);
    }
}

void DLLVisualizer::renderDoubleArrows(const DLLStepState& state) {
    if (state.nodes.size() < 2) return;
    
    float nodeRadius = 30.f;
    float spacing = 120.f;
    sf::Vector2f startPos(200.f, 550.f);

    for (size_t i = 0; i < state.nodes.size() - 1; ++i) {
        sf::Vector2f pos1 = startPos + sf::Vector2f(i * spacing, 0.f);
        sf::Vector2f pos2 = startPos + sf::Vector2f((i + 1) * spacing, 0.f);
        
        sf::Vector2f p1 = pos1 + sf::Vector2f(nodeRadius + 5.f, 0.f);
        sf::Vector2f p2 = pos2 - sf::Vector2f(nodeRadius + 5.f, 0.f);
        
        // Forward arrow (next pointer) - green
        sf::Vertex forwardLine[] = {sf::Vertex{p1, sf::Color(89, 149, 43)}, sf::Vertex{p2, sf::Color(89, 149, 43)}};
        mWindow->draw(forwardLine, 2, sf::PrimitiveType::Lines);
        
        sf::Vector2f dir = p2 - p1;
        float angle = atan2(dir.y, dir.x);
        sf::CircleShape arrowHead(5.f, 3);
        arrowHead.setOrigin({5.f, 5.f});
        arrowHead.setRotation(sf::radians(angle + 3.14159f / 2.f));
        arrowHead.setPosition(p2);
        arrowHead.setFillColor(sf::Color(89, 149, 43));
        mWindow->draw(arrowHead);

        // Backward arrow (prev pointer) - red, offset vertically
        sf::Vertex backwardLine[] = {sf::Vertex{p2 - sf::Vector2f(0.f, 15.f), sf::Color(231, 76, 60)}, 
                                     sf::Vertex{p1 - sf::Vector2f(0.f, 15.f), sf::Color(231, 76, 60)}};
        mWindow->draw(backwardLine, 2, sf::PrimitiveType::Lines);
        
        sf::Vector2f backDir = p1 - p2;
        float backAngle = atan2(backDir.y, backDir.x);
        sf::CircleShape backArrowHead(5.f, 3);
        backArrowHead.setOrigin({5.f, 5.f});
        backArrowHead.setRotation(sf::radians(backAngle + 3.14159f / 2.f));
        backArrowHead.setPosition(p1 - sf::Vector2f(0.f, 15.f));
        backArrowHead.setFillColor(sf::Color(231, 76, 60));
        mWindow->draw(backArrowHead);
    }
}

void DLLVisualizer::renderPseudoCodePanel(const DLLStepState& state) {
    mWindow->draw(mCodePanelBox);
    mWindow->draw(mCodeTitleText);
    
    // Draw pseudocode lines
    float lineHeight = 14.f;
    float startY = 285.f;  // Adjusted for new panel position
    int maxLines = 10;
    
    // Split pseudocode by lines
    std::istringstream iss(state.pseudoCode);
    std::string line;
    int lineNum = 0;
    
    while (std::getline(iss, line) && lineNum < maxLines) {
        sf::Text codeText(mFontMono, line, 12);
        if (ThemeManager::isDark) {
            codeText.setFillColor(sf::Color::Red);
        } else {
            codeText.setFillColor(state.activeLine == lineNum ? sf::Color::Red : sf::Color::Black);
        }
        codeText.setPosition({1030.f, startY + lineNum * lineHeight});
        mWindow->draw(codeText);
        lineNum++;
    }
}

void DLLVisualizer::updatePseudoCodeLines(const DLLStepState& state) {
    mPseudoCodeLines.clear();
    mCppCodeLines.clear();
    
    // Parse pseudocode
    std::istringstream pseudoStream(state.pseudoCode);
    std::string line;
    while (std::getline(pseudoStream, line)) {
        sf::Text text(mFontMono, line, 12);
        text.setFillColor(sf::Color::Black);
        mPseudoCodeLines.push_back(text);
    }
    
    // Parse C++ code
    std::istringstream cppStream(state.cppCode);
    while (std::getline(cppStream, line)) {
        sf::Text text(mFontMono, line, 12);
        text.setFillColor(sf::Color::Black);
        mCppCodeLines.push_back(text);
    }
}

bool DLLVisualizer::checkReturnHome() {
    if (mGoHome) {
        mGoHome = false;
        return true;
    }
    return false;
}

void DLLVisualizer::generateRandomList() {
    mList.initialize();
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 999);
    for (int i = 0; i < 5; ++i) {
        mList.insertTail(dis(gen));
    }
    mList.setCurrentStep(0);
    mIsPaused = false;
    mAutoPlayClock.restart();
    mDescriptionText.setString("Random list generated.");
}

void DLLVisualizer::loadListFromFile() {
    const char* filterPatterns[1] = {"*.txt"};
    const char* filePath = tinyfd_openFileDialog("Choose Doubly Linked List data file", "", 1, filterPatterns, "Text Files (*.txt)", 0);
    if (filePath != nullptr) {
        if (mList.initFromFile(filePath)) {
            mList.setCurrentStep(0);
            mIsPaused = false;
            mAutoPlayClock.restart();
            mDescriptionText.setString("List loaded from file.");
            mShowUndoBtn = false;
        } else {
            mDescriptionText.setString("Failed to load list from file.");
        }
    } else {
        mDescriptionText.setString("File selection canceled.");
    }
    mIsNewExpanded = false;
}

bool DLLVisualizer::isEmpty() {
    auto history = mList.getStepHistory();
    int cur = mList.getCurrentStep();
    if (cur >= 0 && cur < static_cast<int>(history.size())) {
        return history[cur].nodes.empty();
    }
    return true;
}