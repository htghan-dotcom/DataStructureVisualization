#include "DoublyLinkedList/DLLVisualizer.h"
#include "ThemeManager.h"
#include "tinyfiledialogs.h"
#include "Common.h"

DLLVisualizer::DLLVisualizer(sf::RenderWindow& window)
    : mWindow(&window),
      mTitleNum(mFontBold), mTitleDoubly(mFontBold), mTitleLinked(mFontBold), mTitleList(mFontBold),
      
      mClearBtn(mFontRegular, "Clear list", 32.f, 150.f, 160.f, 45.f, 22.5f, ThemeManager::current.secondary),
      mNewBtn(mFontRegular, "New list", 213.f, 150.f, 160.f, 45.f, 22.5f, ThemeManager::current.secondary),
      mInsertBtn(mFontRegular, "Insert", 32.f, 204.f, 160.f, 45.f, 22.5f, ThemeManager::current.secondary),
      mDeleteBtn(mFontRegular, "Delete", 32.f, 258.f, 160.f, 45.f, 22.5f, ThemeManager::current.secondary),
      mSearchBtn(mFontRegular, "Search", 32.f, 312.f, 160.f, 45.f, 22.5f, ThemeManager::current.secondary),
      mUpdateBtn(mFontRegular, "Update", 32.f, 366.f, 160.f, 45.f, 22.5f, ThemeManager::current.secondary),
      mUndoBtn(mFontRegular, "Undo", 32.f, 150.f, 160.f, 45.f, 22.5f, ThemeManager::current.secondary),
      
      mNewHoverStroke(mFontRegular, "", 211.f, 148.f, 164.f, 49.f, 24.5f, ThemeManager::current.primary),
      mNewExpandedStroke(mFontRegular, "", 211.f, 148.f, 164.f, 157.f, 24.5f, ThemeManager::current.primary),
      mNewExpandedBg(mFontRegular, "", 213.f, 150.f, 160.f, 153.f, 22.5f, ThemeManager::current.secondary),
      mRandomBtn(mFontRegular, "Random", 216.f, 207.f, 154.f, 39.f, 19.5f, ThemeManager::current.bg),
      mUploadBtn(mFontRegular, "From File", 216.f, 261.f, 154.f, 39.f, 19.5f, ThemeManager::current.bg),
      
      mInsertHoverStroke(mFontRegular, "", 30.f, 202.f, 164.f, 49.f, 24.5f, ThemeManager::current.primary),
      mInsertExpandedStroke(mFontRegular, "", 30.f, 202.f, 345.f, 49.f, 24.5f, ThemeManager::current.primary),
      mInsertExpandedBg(mFontRegular, "", 32.f, 204.f, 341.f, 45.f, 22.5f, ThemeManager::current.secondary),
      mConfirmAddBtn(mFontRegular, "Add", 216.f, 207.f, 154.f, 39.f, 19.5f, ThemeManager::current.bg),
      mInsertInputText(mFontRegular),

      mDeleteHoverStroke(mFontRegular, "", 30.f, 256.f, 164.f, 49.f, 24.5f, ThemeManager::current.primary),
      mDeleteExpandedStroke(mFontRegular, "", 30.f, 256.f, 345.f, 49.f, 24.5f, ThemeManager::current.primary),
      mDeleteExpandedBg(mFontRegular, "", 32.f, 258.f, 341.f, 45.f, 22.5f, ThemeManager::current.secondary),
      mConfirmRemoveBtn(mFontRegular, "Remove", 216.f, 261.f, 154.f, 39.f, 19.5f, ThemeManager::current.bg),
      mDeleteInputText(mFontRegular),

      mSearchHoverStroke(mFontRegular, "", 30.f, 310.f, 164.f, 49.f, 24.5f, ThemeManager::current.primary),
      mSearchExpandedStroke(mFontRegular, "", 30.f, 310.f, 345.f, 49.f, 24.5f, ThemeManager::current.primary),
      mSearchExpandedBg(mFontRegular, "", 32.f, 312.f, 341.f, 45.f, 22.5f, ThemeManager::current.secondary),
      mConfirmSearchBtn(mFontRegular, "Search", 216.f, 315.f, 154.f, 39.f, 19.5f, ThemeManager::current.bg),
      mSearchInputText(mFontRegular),

      mUpdateHoverStroke(mFontRegular, "", 30.f, 364.f, 164.f, 49.f, 24.5f, ThemeManager::current.primary),
      mUpdateExpandedStroke(mFontRegular, "", 30.f, 364.f, 345.f, 49.f, 24.5f, ThemeManager::current.primary),
      mUpdateExpandedBg(mFontRegular, "", 32.f, 366.f, 341.f, 45.f, 22.5f, ThemeManager::current.secondary),
      mConfirmUpdateBtn(mFontRegular, "Update", 216.f, 369.f, 154.f, 39.f, 19.5f, ThemeManager::current.bg),
      mUpdateInputText(mFontRegular),

      mHideControlsBtn(mFontBold, "<", 382.f, 150.f, 45.f, 45.f, 22.5f, ThemeManager::current.secondary),
      mShowControlsBtn(mFontBold, ">", 32.f, 150.f, 45.f, 45.f, 22.5f, ThemeManager::current.secondary)
{
    if (!mFontBold.openFromFile("assets/fonts/Inter-Bold.ttf") || !mFontRegular.openFromFile("assets/fonts/Inter-Regular.ttf")) {
        cerr << "Cannot load fonts" << endl;
    }

    mClearBtn.refreshText(); mNewBtn.refreshText(); mInsertBtn.refreshText();
    mDeleteBtn.refreshText(); mSearchBtn.refreshText(); mUpdateBtn.refreshText();
    mConfirmAddBtn.refreshText(); mConfirmRemoveBtn.refreshText();
    mConfirmSearchBtn.refreshText(); mConfirmUpdateBtn.refreshText();
    mUndoBtn.refreshText(); mRandomBtn.refreshText(); mUploadBtn.refreshText();
    mHideControlsBtn.refreshText(); mShowControlsBtn.refreshText();

    if (!mDiceTex.loadFromFile("assets/images/randomButton.png")) cerr << "Cannot load randomButton.png\n";
    mDiceTex.setSmooth(true); (void)mDiceTex.generateMipmap();

    mInsertDiceBtn.setup(mDiceTex, 167.f, 211.f, 30.f, 30.f);
    mDeleteDiceBtn.setup(mDiceTex, 167.f, 265.f, 30.f, 30.f);
    mSearchDiceBtn.setup(mDiceTex, 167.f, 319.f, 30.f, 30.f);

    auto setupInput = [&](sf::Text& t, float y){
        t.setFont(mFontRegular);
        t.setCharacterSize(25);
        t.setFillColor(ThemeManager::current.textColor);
        t.setPosition(sf::Vector2f(47.f, y));
    };
    
    setupInput(mInsertInputText, 212.f);
    setupInput(mDeleteInputText, 266.f);
    setupInput(mSearchInputText, 320.f);
    setupInput(mUpdateInputText, 374.f);

    mInsertCursorLine.setSize(sf::Vector2f(2.f, 25.f));
    mDeleteCursorLine.setSize(sf::Vector2f(2.f, 25.f));
    mSearchCursorLine.setSize(sf::Vector2f(2.f, 25.f));
    mUpdateCursorLine.setSize(sf::Vector2f(2.f, 25.f));

    mTitleNum.setString("01");
    mTitleNum.setCharacterSize(30);
    mTitleNum.setPosition(sf::Vector2f(32.f, 663.f));

    mTitleDoubly.setString("Doubly");
    mTitleDoubly.setCharacterSize(70);
    mTitleDoubly.setPosition(sf::Vector2f(32.f, 695.f));
    
    mTitleLinked.setString("Linked");
    mTitleLinked.setCharacterSize(70);
    mTitleLinked.setPosition(sf::Vector2f(32.f + mTitleDoubly.getLocalBounds().size.x + 10.f, 695.f));

    mTitleList.setString("List");
    mTitleList.setCharacterSize(70);
    mTitleList.setPosition(sf::Vector2f(32.f, 765.f));

    // CONNECT APPLAYOUT CALLBACKS
    mLayout.setSkipBackCallback([this](){ mList.setCurrentStep(0); mLayout.setPaused(true); });
    mLayout.setStepBackCallback([this](){
        int cur = mList.getCurrentStep();
        if(cur > 0) mList.setCurrentStep(cur - 1);
        mLayout.setPaused(true);
    });
    mLayout.setStepForwardCallback([this](){
        int cur = mList.getCurrentStep();
        if(cur < (int)mList.getStepHistory().size() - 1) mList.setCurrentStep(cur + 1);
        mLayout.setPaused(true);
    });
    mLayout.setSkipForwardCallback([this](){
        int total = (int)mList.getStepHistory().size();
        if(total > 0) mList.setCurrentStep(total - 1);
        mLayout.setPaused(true);
    });
    mLayout.setPlayPauseCallback([this](){
        if(!mLayout.isPaused()) mAutoPlayClock.restart();
    });

    mClearBtn.setCallback([this]() {
        mList.backup();
        mList.initialize();
        mList.resetHistory("List cleared");
        mLayout.setDescription("List cleared.");
        resetPlayUI();
        mShowUndoBtn = true;
    });
    
    mUndoBtn.setCallback([this]() {
        mList.restore();
        mLayout.setDescription("Undo successfully.");
        mShowUndoBtn = false;
        mLayout.setPaused(true);
        mIsNewExpanded = mIsInsertExpanded = mIsDeleteExpanded = mIsSearchExpanded = mIsUpdateExpanded = false;
        mInputValue = "";
    });

    mHideControlsBtn.setCallback([this](){
        mIsControlsVisible = false;
        mIsNewExpanded = mIsInsertExpanded = mIsDeleteExpanded = mIsSearchExpanded = mIsUpdateExpanded = false;
    });
    
    mShowControlsBtn.setCallback([this](){ mIsControlsVisible = true; });

    mList.initialize();
    generateRandomList();
}

void DLLVisualizer::resetPlayUI(){
    mLayout.setPaused(false);
    mList.setCurrentStep(0);
    mShowUndoBtn = false;
    mAutoPlayClock.restart();
}

void DLLVisualizer::update(const std::optional<sf::Event>& event) {
    sf::Vector2f worldPos = mWindow->mapPixelToCoords(sf::Mouse::getPosition(*mWindow));
    sf::Vector2i mousePos(static_cast<int>(worldPos.x), static_cast<int>(worldPos.y));
    
    mLayout.update(mousePos);
    if (mLayout.mGoHome) { mGoHome = true; mLayout.mGoHome = false; }
    
    if (mIsControlsVisible) {
        mHideControlsBtn.update(mousePos);
        if (mShowUndoBtn) mUndoBtn.update(mousePos); else mClearBtn.update(mousePos);
        mNewBtn.update(mousePos);
        
        if (mIsInsertExpanded) { mConfirmAddBtn.update(mousePos); mInsertDiceBtn.update(mousePos); } else mInsertBtn.update(mousePos);
        if (mIsDeleteExpanded) { mConfirmRemoveBtn.update(mousePos); mDeleteDiceBtn.update(mousePos); } else mDeleteBtn.update(mousePos);
        if (mIsSearchExpanded) { mConfirmSearchBtn.update(mousePos); mSearchDiceBtn.update(mousePos); } else mSearchBtn.update(mousePos);
        if (mIsUpdateExpanded) { mConfirmUpdateBtn.update(mousePos); } else mUpdateBtn.update(mousePos);
    } else {
        mShowControlsBtn.update(mousePos);
    }
    
    float speedMult = mLayout.getSpeed();
    if (speedMult > 0.0f) mList.setVisualizationSpeed(static_cast<int>(500.f / speedMult));
    
    if (!mLayout.isPaused() && mAutoPlayClock.getElapsedTime().asMilliseconds() > mList.getVisualizationSpeed()) {
        int cur = mList.getCurrentStep();
        if (cur < static_cast<int>(mList.getStepHistory().size()) - 1) {
            mList.setCurrentStep(cur + 1);
        } else {
            mLayout.setPaused(true);
        }
        mAutoPlayClock.restart();
    }
    
    if (event) {
        if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mouseEvent->button == sf::Mouse::Button::Left) {
                bool clickedInsideAnyForm = false;
                
                if (mIsControlsVisible) {
                    if (!mIsNewExpanded && sf::FloatRect(sf::Vector2f(213.f, 150.f), sf::Vector2f(160.f, 45.f)).contains(worldPos)) {
                        mIsNewExpanded = true; mShowUndoBtn = false; mIsInsertExpanded = mIsDeleteExpanded = mIsSearchExpanded = mIsUpdateExpanded = false; clickedInsideAnyForm = true;
                    }
                    else if (mIsNewExpanded && sf::FloatRect(sf::Vector2f(213.f, 150.f), sf::Vector2f(160.f, 161.f)).contains(worldPos)) {
                        clickedInsideAnyForm = true;
                        if (sf::FloatRect(sf::Vector2f(216.f, 207.f), sf::Vector2f(154.f, 39.f)).contains(worldPos)) {
                            mList.backup(); generateRandomList(); mIsNewExpanded = false; mShowUndoBtn = false;
                        } else if (sf::FloatRect(sf::Vector2f(216.f, 261.f), sf::Vector2f(154.f, 39.f)).contains(worldPos)) {
                            mList.backup(); loadListFromFile(); mIsNewExpanded = false; mShowUndoBtn = false;
                        }
                    }
                    else if (!mIsInsertExpanded && sf::FloatRect(sf::Vector2f(32.f, 204.f), sf::Vector2f(160.f, 45.f)).contains(worldPos)) {
                        mIsInsertExpanded = true; mIsNewExpanded = mIsDeleteExpanded = mIsSearchExpanded = mIsUpdateExpanded = false; mInputValue = ""; clickedInsideAnyForm = true; mShowUndoBtn = false;
                    }
                    else if (mIsInsertExpanded && sf::FloatRect(sf::Vector2f(32.f, 204.f), sf::Vector2f(341.f, 45.f)).contains(worldPos)) {
                        clickedInsideAnyForm = true;
                        if (sf::FloatRect(sf::Vector2f(216.f, 207.f), sf::Vector2f(154.f, 39.f)).contains(worldPos) && !mInputValue.empty()) {
                            int val = stoi(mInputValue); mList.backup(); mList.resetHistory("Insert " + to_string(val)); mList.insertTail(val); mIsInsertExpanded = false; mInputValue = ""; resetPlayUI();
                        } else if (sf::FloatRect(sf::Vector2f(167.f, 211.f), sf::Vector2f(30.f, 30.f)).contains(worldPos)) {
                            mInputValue = to_string(rand() % 99 + 1);
                        }
                    }
                    else if (!mIsDeleteExpanded && sf::FloatRect(sf::Vector2f(32.f, 258.f), sf::Vector2f(160.f, 45.f)).contains(worldPos)) {
                        mIsDeleteExpanded = true; mIsNewExpanded = mIsInsertExpanded = mIsSearchExpanded = mIsUpdateExpanded = false; mInputValue = ""; clickedInsideAnyForm = true; mShowUndoBtn = false;
                    }
                    else if (mIsDeleteExpanded && sf::FloatRect(sf::Vector2f(32.f, 258.f), sf::Vector2f(341.f, 45.f)).contains(worldPos)) {
                        clickedInsideAnyForm = true;
                        if (sf::FloatRect(sf::Vector2f(216.f, 261.f), sf::Vector2f(154.f, 39.f)).contains(worldPos) && !mInputValue.empty()) {
                            int val = stoi(mInputValue); mList.backup(); mList.resetHistory("Delete " + to_string(val)); mList.deleteValue(val); mIsDeleteExpanded = false; mInputValue = ""; resetPlayUI();
                        } else if (sf::FloatRect(sf::Vector2f(167.f, 265.f), sf::Vector2f(30.f, 30.f)).contains(worldPos)) {
                            mInputValue = to_string(rand() % 99 + 1);
                        }
                    }
                    else if (!mIsSearchExpanded && sf::FloatRect(sf::Vector2f(32.f, 312.f), sf::Vector2f(160.f, 45.f)).contains(worldPos)) {
                        mIsSearchExpanded = true; mIsNewExpanded = mIsInsertExpanded = mIsDeleteExpanded = mIsUpdateExpanded = false; mInputValue = ""; clickedInsideAnyForm = true; mShowUndoBtn = false;
                    }
                    else if (mIsSearchExpanded && sf::FloatRect(sf::Vector2f(32.f, 312.f), sf::Vector2f(341.f, 45.f)).contains(worldPos)) {
                        clickedInsideAnyForm = true;
                        if (sf::FloatRect(sf::Vector2f(216.f, 315.f), sf::Vector2f(154.f, 39.f)).contains(worldPos) && !mInputValue.empty()) {
                            int val = stoi(mInputValue); mList.resetHistory("Search " + to_string(val)); mList.search(val); mIsSearchExpanded = false; mInputValue = ""; resetPlayUI();
                        } else if (sf::FloatRect(sf::Vector2f(167.f, 319.f), sf::Vector2f(30.f, 30.f)).contains(worldPos)) {
                            mInputValue = to_string(rand() % 99 + 1);
                        }
                    }
                    else if (!mIsUpdateExpanded && sf::FloatRect(sf::Vector2f(32.f, 366.f), sf::Vector2f(160.f, 45.f)).contains(worldPos)) {
                        mIsUpdateExpanded = true; mIsNewExpanded = mIsInsertExpanded = mIsDeleteExpanded = mIsSearchExpanded = false;
                        mEditingOld = true; mInputOld = ""; mInputNew = ""; clickedInsideAnyForm = true; mShowUndoBtn = false;
                    }
                    else if (mIsUpdateExpanded && sf::FloatRect(sf::Vector2f(32.f, 366.f), sf::Vector2f(341.f, 45.f)).contains(worldPos)) {
                        clickedInsideAnyForm = true;
                        if (sf::FloatRect(sf::Vector2f(216.f, 369.f), sf::Vector2f(154.f, 39.f)).contains(worldPos) && !mInputOld.empty() && !mInputNew.empty()) {
                            int oldVal = stoi(mInputOld); int newVal = stoi(mInputNew);
                            mList.backup(); mList.update(oldVal, newVal); mIsUpdateExpanded = false;
                            mInputOld = ""; mInputNew = ""; resetPlayUI();
                        }
                        else if (sf::FloatRect(sf::Vector2f(35.f, 369.f), sf::Vector2f(70.f, 39.f)).contains(worldPos)) { mEditingOld = true; }
                        else if (sf::FloatRect(sf::Vector2f(135.f, 369.f), sf::Vector2f(70.f, 39.f)).contains(worldPos)) { mEditingOld = false; }
                    }
                }
                    
                if (!clickedInsideAnyForm) {
                    mIsInsertExpanded = mIsDeleteExpanded = mIsSearchExpanded = mIsUpdateExpanded = mIsNewExpanded = false;
                }
            }
        }
        else if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()){
            if ((keyEvent->system || keyEvent->control) && keyEvent->code == sf::Keyboard::Key::Z) {
                mList.restore(); mShowUndoBtn = false; mLayout.setPaused(true);
                mIsInsertExpanded = mIsDeleteExpanded = mIsSearchExpanded = mIsNewExpanded = mIsUpdateExpanded = false; mInputValue = ""; mInputOld = ""; mInputNew = "";
            }
            else if (keyEvent->code == sf::Keyboard::Key::I) {
                mIsInsertExpanded = true; mIsDeleteExpanded = mIsSearchExpanded = mIsUpdateExpanded = mIsNewExpanded = false; mInputValue = "";
            }
            else if (keyEvent->code == sf::Keyboard::Key::D) {
                mIsDeleteExpanded = true; mIsInsertExpanded = mIsSearchExpanded = mIsUpdateExpanded = mIsNewExpanded = false; mInputValue = "";
            }
            else if (keyEvent->code == sf::Keyboard::Key::S) {
                mIsSearchExpanded = true; mIsInsertExpanded = mIsDeleteExpanded = mIsUpdateExpanded = mIsNewExpanded = false; mInputValue = "";
            }
            else if (keyEvent->code == sf::Keyboard::Key::U) {
                mIsUpdateExpanded = true; mIsInsertExpanded = mIsDeleteExpanded = mIsSearchExpanded = mIsNewExpanded = false; mEditingOld = true; mInputOld = ""; mInputNew = "";
            }
            else if (keyEvent->code == sf::Keyboard::Key::N) {
                mIsNewExpanded = true; mIsInsertExpanded = mIsDeleteExpanded = mIsSearchExpanded = mIsUpdateExpanded = false; mInputValue = "";
            }
            else if (keyEvent->code == sf::Keyboard::Key::Space) {
                mLayout.setPaused(!mLayout.isPaused());
            }
            else if (keyEvent->code == sf::Keyboard::Key::Left) {
                int cur = mList.getCurrentStep();
                if (cur > 0) mList.setCurrentStep(cur - 1);
                mLayout.setPaused(true);
            }
            else if (keyEvent->code == sf::Keyboard::Key::Right) {
                int cur = mList.getCurrentStep();
                int total = static_cast<int>(mList.getStepHistory().size());
                if (cur < total - 1) mList.setCurrentStep(cur + 1);
                mLayout.setPaused(true);
            }
            else if (keyEvent->code == sf::Keyboard::Key::R) {
                if (mIsInsertExpanded || mIsDeleteExpanded || mIsSearchExpanded) {
                    mInputValue = std::to_string(rand() % 99 + 1);
                } else if (mIsUpdateExpanded) {
                } else {
                    mList.backup(); generateRandomList(); mShowUndoBtn = false; mIsNewExpanded = false;
                }
            }
            else if (keyEvent->code == sf::Keyboard::Key::Backspace) {
                if (mIsInsertExpanded || mIsDeleteExpanded || mIsSearchExpanded) {
                    if (!mInputValue.empty()) mInputValue.pop_back();
                } else if (mIsUpdateExpanded) {
                    if (mEditingOld && !mInputOld.empty()) mInputOld.pop_back();
                    else if (!mEditingOld && !mInputNew.empty()) mInputNew.pop_back();
                }
            }
            else if (keyEvent->code == sf::Keyboard::Key::Enter) {
                if (mIsInsertExpanded && !mInputValue.empty()) {
                    int val = stoi(mInputValue); mList.backup(); mList.resetHistory("Insert " + std::to_string(val)); mList.insertTail(val); mIsInsertExpanded = false; mInputValue = ""; resetPlayUI();
                }
                else if (mIsDeleteExpanded && !mInputValue.empty()) {
                    int val = stoi(mInputValue); mList.backup(); mList.resetHistory("Delete " + std::to_string(val)); mList.deleteValue(val); mIsDeleteExpanded = false; mInputValue = ""; resetPlayUI();
                }
                else if (mIsSearchExpanded && !mInputValue.empty()) {
                    int val = stoi(mInputValue); mList.resetHistory("Search for " + std::to_string(val)); mList.search(val); mIsSearchExpanded = false; mInputValue = ""; resetPlayUI();
                }
                else if (mIsUpdateExpanded && !mInputOld.empty() && !mInputNew.empty()) {
                    int oldVal = stoi(mInputOld); int newVal = stoi(mInputNew); mList.backup(); mList.update(oldVal, newVal); mIsUpdateExpanded = false; mInputOld = ""; mInputNew = ""; resetPlayUI();
                }
            }
            else if (keyEvent->code == sf::Keyboard::Key::Tab) {
                if (mIsUpdateExpanded) mEditingOld = !mEditingOld;
            }
            else {
                int digit = -1;
                if (keyEvent->code >= sf::Keyboard::Key::Num0 && keyEvent->code <= sf::Keyboard::Key::Num9)
                    digit = (int)keyEvent->code - (int)sf::Keyboard::Key::Num0;
                else if (keyEvent->code >= sf::Keyboard::Key::Numpad0 && keyEvent->code <= sf::Keyboard::Key::Numpad9)
                    digit = (int)keyEvent->code - (int)sf::Keyboard::Key::Numpad0;
                
                if (digit != -1) {
                    if (mIsInsertExpanded || mIsDeleteExpanded || mIsSearchExpanded) {
                        if (mInputValue.size() < 4) mInputValue += std::to_string(digit);
                    } else if (mIsUpdateExpanded) {
                        if (mEditingOld && mInputOld.size() < 3) mInputOld += std::to_string(digit);
                        else if (!mEditingOld && mInputNew.size() < 3) mInputNew += std::to_string(digit);
                    }
                }
            }
        }
    }
}

void DLLVisualizer::renderNodeVisualization(const DLLStepState& state) {
    int nodeCount = static_cast<int>(state.nodes.size());
    if (nodeCount == 0) return;

    auto layout = computeLayout(nodeCount);
    float r = layout.nodeRadius;
    unsigned int fontSize = static_cast<unsigned int>(20.f * (r / 30.f));

    for (int i = 0; i < nodeCount; ++i) {
        sf::Vector2f pos(layout.xOffset + i * layout.spacing, layout.y);

        sf::CircleShape circle(r);
        circle.setOrigin(sf::Vector2f(r, r));
        circle.setPosition(pos);
        circle.setOutlineThickness(3.f);
        circle.setOutlineColor(state.nodes[i].isHighlighted
                                   ? sf::Color::Yellow
                                   : ThemeManager::current.textColor);
        circle.setFillColor(ThemeManager::current.primaryLight);
        mWindow->draw(circle);

        sf::Text text(mFontRegular, to_string(state.nodes[i].val), fontSize);
        auto bounds = text.getLocalBounds();
        text.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2.f,
                                    bounds.position.y + bounds.size.y / 2.f));
        text.setPosition(pos);
        text.setFillColor(sf::Color(41, 42, 49));
        mWindow->draw(text);
    }
}

void DLLVisualizer::renderDoubleArrows(const DLLStepState& state) {
    int nodeCount = static_cast<int>(state.nodes.size());
    if (nodeCount < 2) return;

    auto layout = computeLayout(nodeCount);
    float r = layout.nodeRadius;
    float currentScale = r / 30.f;
    
    float arrowHeadSize = 6.f * currentScale;
    
    float yOffset = 8.f * currentScale;
    float xOffset = std::sqrt(r * r - yOffset * yOffset);

    for (int i = 0; i < nodeCount - 1; ++i) {
        sf::Vector2f pos1(layout.xOffset + i * layout.spacing,       layout.y);
        sf::Vector2f pos2(layout.xOffset + (i + 1) * layout.spacing, layout.y);

        sf::Vector2f fwdP1 = pos1 + sf::Vector2f(xOffset, -yOffset);
        sf::Vector2f fwdP2 = pos2 + sf::Vector2f(-xOffset, -yOffset);
        
        sf::Vertex fwdLine[] = {
            sf::Vertex{fwdP1, ThemeManager::current.primary},
            sf::Vertex{fwdP2 - sf::Vector2f(arrowHeadSize, 0.f), ThemeManager::current.primary}
        };
        mWindow->draw(fwdLine, 2, sf::PrimitiveType::Lines);

        sf::CircleShape fwdHead(arrowHeadSize, 3);
        fwdHead.setOrigin(sf::Vector2f(arrowHeadSize, arrowHeadSize));
        fwdHead.setRotation(sf::degrees(90.f));
        fwdHead.setPosition(fwdP2);
        fwdHead.setFillColor(ThemeManager::current.primary);
        mWindow->draw(fwdHead);

        sf::Vector2f bwdP1 = pos2 + sf::Vector2f(-xOffset, yOffset);
        sf::Vector2f bwdP2 = pos1 + sf::Vector2f(xOffset, yOffset);
        
        sf::Vertex bwdLine[] = {
            sf::Vertex{bwdP1, sf::Color(230, 57, 70)},
            sf::Vertex{bwdP2 + sf::Vector2f(arrowHeadSize, 0.f), sf::Color(230, 57, 70)}
        };
        mWindow->draw(bwdLine, 2, sf::PrimitiveType::Lines);
        
        sf::CircleShape bwdHead(arrowHeadSize, 3);
        bwdHead.setOrigin(sf::Vector2f(arrowHeadSize, arrowHeadSize));
        bwdHead.setRotation(sf::degrees(-90.f));
        bwdHead.setPosition(bwdP2);
        bwdHead.setFillColor(sf::Color(230, 57, 70));
        mWindow->draw(bwdHead);
    }
}

bool DLLVisualizer::checkReturnHome() {
    if (mGoHome) {
        mGoHome = false;
        return true;
    }
    return false;
}

void DLLVisualizer::render(bool showUI) {
    if (showUI) {
        setTransitionProgress(1.0f);
    } else if (mTransitionProgress == 1.0f) {
        setTransitionProgress(0.0f);
    }
    
    auto history = mList.getStepHistory();
    int cur = mList.getCurrentStep();
    int total = static_cast<int>(history.size());

    if (cur >= 0 && cur < total) {
        const DLLStepState& state = history[cur];
        
        mLayout.setDescription(state.description);
        
        std::vector<std::string> pseudoLines;
        std::istringstream iss(state.pseudoCode);
        std::string line;
        while (std::getline(iss, line)) { pseudoLines.push_back(line); }
        mLayout.setPseudoCode(pseudoLines);
        mLayout.setActiveCodeLine(state.activeLine);
        
        renderNodeVisualization(state);
        renderDoubleArrows(state);
    }

    if (showUI) {
        mNewHoverStroke.setThemeColor(ThemeManager::current.primary);
        mNewExpandedStroke.setThemeColor(ThemeManager::current.primary);
        mInsertHoverStroke.setThemeColor(ThemeManager::current.primary);
        mInsertExpandedStroke.setThemeColor(ThemeManager::current.primary);
        mDeleteHoverStroke.setThemeColor(ThemeManager::current.primary);
        mDeleteExpandedStroke.setThemeColor(ThemeManager::current.primary);
        mSearchHoverStroke.setThemeColor(ThemeManager::current.primary);
        mSearchExpandedStroke.setThemeColor(ThemeManager::current.primary);
        mUpdateHoverStroke.setThemeColor(ThemeManager::current.primary);
        mUpdateExpandedStroke.setThemeColor(ThemeManager::current.primary);
        
        if (mInsertDiceBtn.mSprite) mInsertDiceBtn.mSprite->setColor(ThemeManager::current.textColor);
        if (mDeleteDiceBtn.mSprite) mDeleteDiceBtn.mSprite->setColor(ThemeManager::current.textColor);
        if (mSearchDiceBtn.mSprite) mSearchDiceBtn.mSprite->setColor(ThemeManager::current.textColor);
        
        mTitleNum.setFillColor(ThemeManager::current.textColor);
        mTitleDoubly.setFillColor(ThemeManager::current.primary);
        mTitleLinked.setFillColor(ThemeManager::current.primaryLight);
        mTitleList.setFillColor(ThemeManager::current.textColor);

        mClearBtn.setThemeColor(ThemeManager::current.secondary);
        mNewBtn.setThemeColor(ThemeManager::current.secondary);
        mInsertBtn.setThemeColor(ThemeManager::current.secondary);
        mDeleteBtn.setThemeColor(ThemeManager::current.secondary);
        mSearchBtn.setThemeColor(ThemeManager::current.secondary);
        mUpdateBtn.setThemeColor(ThemeManager::current.secondary);
        mUndoBtn.setThemeColor(ThemeManager::current.secondary);

        mUploadBtn.setThemeColor(ThemeManager::current.screenBg);
        mRandomBtn.setThemeColor(ThemeManager::current.screenBg);
        mConfirmAddBtn.setThemeColor(ThemeManager::current.screenBg);
        mConfirmRemoveBtn.setThemeColor(ThemeManager::current.screenBg);
        mConfirmSearchBtn.setThemeColor(ThemeManager::current.screenBg);
        mConfirmUpdateBtn.setThemeColor(ThemeManager::current.screenBg);
        
        mInsertExpandedBg.setThemeColor(ThemeManager::current.secondary);
        mDeleteExpandedBg.setThemeColor(ThemeManager::current.secondary);
        mSearchExpandedBg.setThemeColor(ThemeManager::current.secondary);
        mUpdateExpandedBg.setThemeColor(ThemeManager::current.secondary);
        mNewExpandedBg.setThemeColor(ThemeManager::current.secondary);

        mInsertInputText.setFillColor(ThemeManager::current.textColor);
        mDeleteInputText.setFillColor(ThemeManager::current.textColor);
        mSearchInputText.setFillColor(ThemeManager::current.textColor);
        mUpdateInputText.setFillColor(ThemeManager::current.textColor);

        mInsertCursorLine.setFillColor(ThemeManager::current.textColor);
        mDeleteCursorLine.setFillColor(ThemeManager::current.textColor);
        mSearchCursorLine.setFillColor(ThemeManager::current.textColor);
        mUpdateCursorLine.setFillColor(ThemeManager::current.textColor);

        mLayout.setStepText("Step " + to_string(total == 0 ? 0 : cur + 1) + " / " + to_string(total));
        mLayout.draw(*mWindow);

        mWindow->draw(mTitleNum);
        mWindow->draw(mTitleDoubly);
        mWindow->draw(mTitleLinked);
        mWindow->draw(mTitleList);

        sf::Vector2f worldPos = mWindow->mapPixelToCoords(sf::Mouse::getPosition(*mWindow));

        if (mIsControlsVisible) {
            mHideControlsBtn.setThemeColor(ThemeManager::current.secondary);
            mHideControlsBtn.draw(*mWindow);
            
            if (mShowUndoBtn) mUndoBtn.draw(*mWindow); else mClearBtn.draw(*mWindow);
            
            if (mIsNewExpanded) {
                mNewExpandedStroke.draw(*mWindow); mNewExpandedBg.draw(*mWindow); mNewBtn.draw(*mWindow); mRandomBtn.draw(*mWindow); mUploadBtn.draw(*mWindow);
            } else {
                if (sf::FloatRect(sf::Vector2f(213.f, 150.f), sf::Vector2f(160.f, 45.f)).contains(worldPos)) mNewHoverStroke.draw(*mWindow);
                mNewBtn.draw(*mWindow);
            }
            
            if (mIsInsertExpanded) {
                mInsertExpandedStroke.draw(*mWindow); mInsertExpandedBg.draw(*mWindow); mConfirmAddBtn.draw(*mWindow); mInsertDiceBtn.draw(*mWindow);
                mInsertInputText.setString(mInputValue); mWindow->draw(mInsertInputText);
                if (mCursorClock.getElapsedTime().asSeconds() >= 0.5f) { mShowCursor = !mShowCursor; mCursorClock.restart(); }
                if (mShowCursor) {
                    float textWidth = mInsertInputText.getLocalBounds().size.x;
                    mInsertCursorLine.setPosition(sf::Vector2f(mInputValue.empty() ? 47.f : 47.f + textWidth + 2.f, 214.f));
                    mWindow->draw(mInsertCursorLine);
                }
            } else {
                if (sf::FloatRect(sf::Vector2f(32.f, 204.f), sf::Vector2f(160.f, 45.f)).contains(worldPos)) mInsertHoverStroke.draw(*mWindow);
                mInsertBtn.draw(*mWindow);
            }
            
            if (mIsDeleteExpanded) {
                mDeleteExpandedStroke.draw(*mWindow); mDeleteExpandedBg.draw(*mWindow); mConfirmRemoveBtn.draw(*mWindow); mDeleteDiceBtn.draw(*mWindow);
                mDeleteInputText.setString(mInputValue); mWindow->draw(mDeleteInputText);
                if (mCursorClock.getElapsedTime().asSeconds() >= 0.5f) { mShowCursor = !mShowCursor; mCursorClock.restart(); }
                if (mShowCursor) {
                    float textWidth = mDeleteInputText.getLocalBounds().size.x;
                    mDeleteCursorLine.setPosition(sf::Vector2f(mInputValue.empty() ? 47.f : 47.f + textWidth + 2.f, 268.f));
                    mWindow->draw(mDeleteCursorLine);
                }
            } else {
                if (sf::FloatRect(sf::Vector2f(32.f, 258.f), sf::Vector2f(160.f, 45.f)).contains(worldPos)) mDeleteHoverStroke.draw(*mWindow);
                mDeleteBtn.draw(*mWindow);
            }
            
            if (mIsSearchExpanded) {
                mSearchExpandedStroke.draw(*mWindow); mSearchExpandedBg.draw(*mWindow); mConfirmSearchBtn.draw(*mWindow); mSearchDiceBtn.draw(*mWindow);
                mSearchInputText.setString(mInputValue); mWindow->draw(mSearchInputText);
                if (mCursorClock.getElapsedTime().asSeconds() >= 0.5f) { mShowCursor = !mShowCursor; mCursorClock.restart(); }
                if (mShowCursor) {
                    float textWidth = mSearchInputText.getLocalBounds().size.x;
                    mSearchCursorLine.setPosition(sf::Vector2f(mInputValue.empty() ? 47.f : 47.f + textWidth + 2.f, 322.f));
                    mWindow->draw(mSearchCursorLine);
                }
            } else {
                if (sf::FloatRect(sf::Vector2f(32.f, 312.f), sf::Vector2f(160.f, 45.f)).contains(worldPos)) mSearchHoverStroke.draw(*mWindow);
                mSearchBtn.draw(*mWindow);
            }

            if (mIsUpdateExpanded) {
                mUpdateExpandedStroke.draw(*mWindow); mUpdateExpandedBg.draw(*mWindow); mConfirmUpdateBtn.draw(*mWindow);
                mUpdateInputText.setString(mInputValue); mWindow->draw(mUpdateInputText);
                if (mCursorClock.getElapsedTime().asSeconds() >= 0.5f) { mShowCursor = !mShowCursor; mCursorClock.restart(); }
                if (mShowCursor) {
                    float textWidth = mUpdateInputText.getLocalBounds().size.x;
                    mUpdateCursorLine.setPosition(sf::Vector2f(mInputValue.empty() ? 47.f : 47.f + textWidth + 2.f, 376.f));
                    mWindow->draw(mUpdateCursorLine);
                }
            } else {
                if (sf::FloatRect(sf::Vector2f(32.f, 366.f), sf::Vector2f(160.f, 45.f)).contains(worldPos)) mUpdateHoverStroke.draw(*mWindow);
                mUpdateBtn.draw(*mWindow);
            }
            
            if (mIsUpdateExpanded) {
                mUpdateExpandedStroke.draw(*mWindow);
                mUpdateExpandedBg.draw(*mWindow);
                mConfirmUpdateBtn.draw(*mWindow);

                float baseY = 366.f;
                sf::Color boxFill = ThemeManager::current.bg;

                sf::ConvexShape oldBox = createRoundedRect(sf::Vector2f(70.f, 39.f), 19.5f);
                oldBox.setPosition(sf::Vector2f(35.f, baseY + 3.f));
                oldBox.setFillColor(boxFill);
                oldBox.setOutlineThickness(2.f);
                oldBox.setOutlineColor(mEditingOld ? ThemeManager::current.primary : sf::Color(200,200,200));
                mWindow->draw(oldBox);
                      
                sf::ConvexShape newBox = createRoundedRect(sf::Vector2f(70.f, 39.f), 19.5f);
                newBox.setPosition(sf::Vector2f(135.f, baseY + 3.f));
                newBox.setFillColor(boxFill);
                newBox.setOutlineThickness(2.f);
                newBox.setOutlineColor(!mEditingOld ? ThemeManager::current.primary : sf::Color(200,200,200));
                mWindow->draw(newBox);
                            
                sf::Text arrowText(mFontBold, "->", 20);
                arrowText.setFillColor(ThemeManager::current.textColor);
                arrowText.setPosition(sf::Vector2f(108.f, baseY + 9.f));
                mWindow->draw(arrowText);

                mUpdateInputText.setCharacterSize(18);
                mUpdateInputText.setFillColor(ThemeManager::current.textColor);

                mUpdateInputText.setString(mInputOld);
                sf::FloatRect oldBounds = mUpdateInputText.getLocalBounds();
                mUpdateInputText.setPosition(sf::Vector2f(35.f + (70.f - oldBounds.size.x)/2.f - oldBounds.position.x, baseY + 11.f));
                mWindow->draw(mUpdateInputText);

                mUpdateInputText.setString(mInputNew);
                sf::FloatRect newBounds = mUpdateInputText.getLocalBounds();
                mUpdateInputText.setPosition(sf::Vector2f(135.f + (70.f - newBounds.size.x)/2.f - newBounds.position.x, baseY + 11.f));
                mWindow->draw(mUpdateInputText);

                if (mCursorClock.getElapsedTime().asSeconds() >= 0.5f) { mShowCursor = !mShowCursor; mCursorClock.restart(); }
                if (mShowCursor) {
                    float cursorX;
                    if (mEditingOld) {
                        cursorX = 35.f + (70.f + oldBounds.size.x)/2.f + 2.f;
                    } else {
                        cursorX = 135.f + (70.f + newBounds.size.x)/2.f + 2.f;
                    }
                    mUpdateCursorLine.setPosition(sf::Vector2f(cursorX, baseY + 13.f));
                    mUpdateCursorLine.setFillColor(ThemeManager::current.textColor);
                    mWindow->draw(mUpdateCursorLine);
                }
            } else {
                if (sf::FloatRect(sf::Vector2f(32.f, 366.f), sf::Vector2f(160.f, 45.f)).contains(worldPos)) mUpdateHoverStroke.draw(*mWindow);
                    mUpdateBtn.draw(*mWindow);
            }
        } else {
            mShowControlsBtn.setThemeColor(ThemeManager::current.secondary);
            mShowControlsBtn.draw(*mWindow);
        }
    }
}

void DLLVisualizer::generateRandomList() {
    mList.initialize();
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 999);
    for (int i = 0; i < 5; ++i) {
        mList.insertTail(dis(gen));
    }
    
    int totalSteps = static_cast<int>(mList.getStepHistory().size());
    if (totalSteps > 0) {
        mList.setCurrentStep(totalSteps - 1); 
    }
    
    mLayout.setPaused(true); 
    
    mShowUndoBtn = false;

    mLayout.setDescription("Random list generated.");
}

void DLLVisualizer::loadListFromFile() {
    const char* filterPatterns[1] = {"*.txt"};
    const char* filePath = tinyfd_openFileDialog("Choose Doubly Linked List data file", "", 1, filterPatterns, "Text Files (*.txt)", 0);
    if (filePath != nullptr) {
        if (mList.initFromFile(filePath)) {
            resetPlayUI();
            mLayout.setDescription("List loaded from file.");
            mShowUndoBtn = false;
        } else {
            mLayout.setDescription("Failed to load list from file.");
        }
    } else {
        mLayout.setDescription("File selection canceled.");
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

DLLVisualizer::DLLLayout DLLVisualizer::computeLayout(int nodeCount) {
    float p     = mTransitionProgress;
    float easeP = p * p * (3.f - 2.f * p);

    float baseSpacing = 120.f;
    float baseRadius  = 30.f;
    float totalWidth  = (nodeCount > 1) ? (nodeCount - 1) * baseSpacing : 0.f;

    // ── Vis layout ─────────────────────────────────────────────
    // Scale chỉ khi list vượt safe zone
    float safeLeft  = 212.f;    // 192 + 20
    float safeRight = 1008.f;   // 1028 - 20
    float maxW      = safeRight - safeLeft - 70.f;

    float visScale   = (totalWidth > maxW && totalWidth > 0.f) ? maxW / totalWidth : 1.f;
    float visSpacing = baseSpacing * visScale;
    float visRadius  = baseRadius  * visScale;
    float visTotalW  = (nodeCount > 1) ? (nodeCount - 1) * visSpacing : 0.f;
    float visCenterX = (safeLeft + safeRight) * 0.5f;   // 610
    float visX       = visCenterX - visTotalW * 0.5f;
    float visY       = 400.f;

    // ── Menu thumbnail layout ───────────────────────────────────
    // Card 0 (DLL): x ∈ [0, 360], center = 180
    float menuUsableW = 280.f;
    float menuCenterX = 180.f;

    float menuScale   = (totalWidth > menuUsableW && totalWidth > 0.f)
                            ? menuUsableW / totalWidth : 1.f;
    float menuSpacing = baseSpacing * menuScale;
    float menuRadius  = baseRadius  * menuScale;
    float menuTotalW  = (nodeCount > 1) ? (nodeCount - 1) * menuSpacing : 0.f;
    float menuX       = menuCenterX - menuTotalW * 0.5f;
    float menuY       = 300.f;

    // ── Interpolate ─────────────────────────────────────────────
    DLLLayout result;
    result.spacing    = menuSpacing + (visSpacing - menuSpacing) * easeP;
    result.xOffset    = menuX       + (visX       - menuX)       * easeP;
    result.y          = menuY       + (visY       - menuY)       * easeP;
    result.nodeRadius = menuRadius  + (visRadius  - menuRadius)  * easeP;
    return result;
}
