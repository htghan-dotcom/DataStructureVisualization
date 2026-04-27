#include "RedBlackTree/RBTVisualizer.h"
#include "ThemeManager.h"
#include "tinyfiledialogs.h"
#include "Common.h"

RBTVisualizer::RBTVisualizer(sf::RenderWindow& window)
    : mWindow(&window),
      mTitleNum(mFontBold),
      mTitleRed(mFontBold), mTitleBlack(mFontBold), mTitleTree(mFontBold),

      mClearBtn(mFontRegular, "Clear tree", 32.f, 150.f, 160.f, 45.f, 22.5f, ThemeManager::current.secondary),
      mNewBtn(mFontRegular, "New tree", 213.f, 150.f, 160.f, 45.f, 22.5f, ThemeManager::current.secondary),
      mInsertBtn(mFontRegular, "Insert", 32.f, 204.f, 160.f, 45.f, 22.5f, ThemeManager::current.secondary),
      mDeleteBtn(mFontRegular, "Delete", 32.f, 258.f, 160.f, 45.f, 22.5f, ThemeManager::current.secondary),
      mSearchBtn(mFontRegular, "Search", 32.f, 312.f, 160.f, 45.f, 22.5f, ThemeManager::current.secondary),
      mUndoBtn(mFontRegular, "Undo", 32.f, 150.f, 160.f, 45.f, 22.5f, ThemeManager::current.secondary),
      
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

      mNewHoverStroke(mFontRegular, "", 211.f, 148.f, 164.f, 49.f, 24.5f, ThemeManager::current.primary),
      mNewExpandedStroke(mFontRegular, "", 211.f, 148.f, 164.f, 157.f, 24.5f, ThemeManager::current.primary),
      mNewExpandedBg(mFontRegular, "", 213.f, 150.f, 160.f, 153.f, 22.5f, ThemeManager::current.secondary),
      mRandomBtn(mFontRegular, "Random", 216.f, 207.f, 154.f, 39.f, 19.5f, ThemeManager::current.bg),
      mUploadBtn(mFontRegular, "From File", 216.f, 261.f, 154.f, 39.f, 19.5f, ThemeManager::current.bg),

      mHideControlsBtn(mFontBold, "<", 382.f, 150.f, 45.f, 45.f, 22.5f, ThemeManager::current.secondary),
      mShowControlsBtn(mFontBold, ">", 32.f, 150.f, 45.f, 45.f, 22.5f, ThemeManager::current.secondary)
{
    if (!mFontBold.openFromFile("assets/fonts/Inter-Bold.ttf") or !mFontRegular.openFromFile("assets/fonts/Inter-Regular.ttf")){
        cerr << "Cannot load font!" << endl;
    }

    mClearBtn.refreshText();
    mNewBtn.refreshText();
    mInsertBtn.refreshText();
    mDeleteBtn.refreshText();
    mSearchBtn.refreshText();
    mConfirmAddBtn.refreshText();
    mConfirmRemoveBtn.refreshText();
    mConfirmSearchBtn.refreshText();
    mUndoBtn.refreshText();
    mRandomBtn.refreshText();
    mUploadBtn.refreshText();
    mHideControlsBtn.refreshText();
    mShowControlsBtn.refreshText();

    if (!mDiceTex.loadFromFile("assets/images/randomButton.png")){cerr << "Cannot load randomButton.png" << endl;}
    mDiceTex.setSmooth(true); mDiceTex.generateMipmap();
    
    mInsertDiceBtn.setup(mDiceTex, 167.f, 211.f, 30.f, 30.f);
    mDeleteDiceBtn.setup(mDiceTex, 167.f, 265.f, 30.f, 30.f);
    mSearchDiceBtn.setup(mDiceTex, 167.f, 319.f, 30.f, 30.f);
    
    mInsertInputText.setFont(mFontRegular);
    mInsertInputText.setCharacterSize(25);
    mInsertInputText.setFillColor(ThemeManager::current.textColor);
    mInsertInputText.setPosition(sf::Vector2f(47.f, 212.f));
    
    mDeleteInputText.setFont(mFontRegular);
    mDeleteInputText.setCharacterSize(25);
    mDeleteInputText.setFillColor(ThemeManager::current.textColor);
    mDeleteInputText.setPosition(sf::Vector2f(47.f, 266.f));
    
    mSearchInputText.setFont(mFontRegular);
    mSearchInputText.setCharacterSize(25);
    mSearchInputText.setFillColor(ThemeManager::current.textColor);
    mSearchInputText.setPosition(sf::Vector2f(47.f, 320.f));
    
    mInsertCursorLine.setSize(sf::Vector2f(2.f, 25.f));
    mInsertCursorLine.setFillColor(ThemeManager::current.textColor);
    
    mDeleteCursorLine.setSize(sf::Vector2f(2.f, 25.f));
    mDeleteCursorLine.setFillColor(ThemeManager::current.textColor);
    
    mSearchCursorLine.setSize(sf::Vector2f(2.f, 25.f));
    mSearchCursorLine.setFillColor(ThemeManager::current.textColor);

    mClearBtn.setCallback([this](){
        mTree.backup();
        mTree.initialize();
        mTree.resetHistory("Tree cleared.");
        mLayout.setDescription("Tree cleared.");
        resetPlayUI();
        mShowUndoBtn = true;
    });
        
    mUndoBtn.setCallback([this](){
        mTree.restore();
        mLayout.setDescription("Undo successfully.");
        mShowUndoBtn = false;
        mLayout.setPaused(true);
            
        mIsNewExpanded = false;
        mIsInsertExpanded = false;
        mIsDeleteExpanded = false;
        mIsSearchExpanded = false;
        mInputValue = "";
    });
    
    mHideControlsBtn.setCallback([this](){
        mIsControlsVisible = false;
        mIsNewExpanded = false;
        mIsInsertExpanded = false;
        mIsDeleteExpanded = false;
        mIsSearchExpanded = false;
    });
    
    mShowControlsBtn.setCallback([this](){
        mIsControlsVisible = true;
    });
    
    mLayout.setSkipBackCallback([this](){
        mTree.setCurrentStep(0);
        mTargetStep = -1;
        mLayout.setPaused(true);
    });

    mLayout.setStepBackCallback([this](){
        int cur = mTree.getCurrentStep();
        if (cur > 0){
            mTargetStep = cur - 1;
            mStepAnimProgress = 0.0f;
        }
        mLayout.setPaused(true);
    });

    mLayout.setStepForwardCallback([this](){
        int cur = mTree.getCurrentStep();
        int total = static_cast<int>(mTree.getStepHistory().size());
        if (cur < total - 1){
            mTargetStep = cur + 1;
            mStepAnimProgress = 0.0f;
        }
        mLayout.setPaused(true);
    });

    mLayout.setSkipForwardCallback([this](){
        int total = static_cast<int>(mTree.getStepHistory().size());
        if (total > 0){
            mTree.setCurrentStep(total - 1);
            mTargetStep = -1;
        }
        mLayout.setPaused(true);
    });
    
    mTitleNum.setString("03");
    mTitleNum.setCharacterSize(30);
    mTitleNum.setFillColor(ThemeManager::current.textColor);
    mTitleNum.setPosition(sf::Vector2f(32.f, 663.f));

    mTitleRed.setString("Red");
    mTitleRed.setCharacterSize(70);
    mTitleRed.setFillColor(sf::Color(230, 57, 70));
    mTitleRed.setPosition(sf::Vector2f(32.f, 695.f));
    
    mTitleBlack.setString("-Black");
    mTitleBlack.setCharacterSize(70);
    mTitleBlack.setFillColor(ThemeManager::current.textColor);
    mTitleBlack.setPosition(sf::Vector2f(44.f + mTitleRed.getLocalBounds().size.x, 695.f));

    mTitleTree.setString("Tree");
    mTitleTree.setCharacterSize(70);
    mTitleTree.setFillColor(ThemeManager::current.primary);
    mTitleTree.setPosition(sf::Vector2f(32.f, 765.f));
      
    mTree.initialize();
              
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 99);
              
    for (int i = 0; i < 7; ++i){
        mTree.insert(dis(gen));
    }

    generateRandomTree();
}

void RBTVisualizer::update(const optional<sf::Event>& event){
    sf::Vector2f worldPos = mWindow->mapPixelToCoords(sf::Mouse::getPosition(*mWindow));
    sf::Vector2i mousePos(static_cast<int>(worldPos.x), static_cast<int>(worldPos.y));
    
    mLayout.update(mousePos);
    if (mLayout.mGoHome){
        mGoHome = true;
        mLayout.mGoHome = false;
    }
        
    if (mIsControlsVisible){
        mHideControlsBtn.update(mousePos);
        
        if (mShowUndoBtn){
            mUndoBtn.update(mousePos);
        } else {
            mClearBtn.update(mousePos);
        }
                
        mNewBtn.update(mousePos);
                
        if (mIsInsertExpanded){
            mConfirmAddBtn.update(mousePos);
            mInsertDiceBtn.update(mousePos);
        } else {
            mInsertBtn.update(mousePos);
        }

        if (mIsDeleteExpanded){
            mConfirmRemoveBtn.update(mousePos);
            mDeleteDiceBtn.update(mousePos);
        } else {
            mDeleteBtn.update(mousePos);
        }

        if (mIsSearchExpanded){
            mConfirmSearchBtn.update(mousePos);
            mSearchDiceBtn.update(mousePos);
        } else {
            mSearchBtn.update(mousePos);
        }
    } else {
        mShowControlsBtn.update(mousePos);
    }
    
    float speedMult = mLayout.getSpeed();
    if (speedMult > 0.0f){
        int speedMs = static_cast<int>(500.f / speedMult);
        mTree.setVisualizationSpeed(speedMs);
    }

    if (event){
        if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()){
            if (mouseEvent->button == sf::Mouse::Button::Left){
                bool clickedInsideAnyForm = false;
                
                if (mIsControlsVisible){
                    if (!mIsNewExpanded and sf::FloatRect(sf::Vector2f(213.f, 150.f), sf::Vector2f(160.f, 45.f)).contains(worldPos)){
                        mIsNewExpanded = true;
                        mShowUndoBtn = false;
                        mIsInsertExpanded = false;
                        mIsDeleteExpanded = false;
                        mIsSearchExpanded = false;
                        
                        clickedInsideAnyForm = true;
                    }
                    
                    else if (mIsNewExpanded and sf::FloatRect(sf::Vector2f(213.f, 150.f), sf::Vector2f(160.f, 161.f)).contains(worldPos)){
                        clickedInsideAnyForm = true;
                        if (sf::FloatRect(sf::Vector2f(216.f, 207.f), sf::Vector2f(154.f, 39.f)).contains(worldPos)){
                            mTree.backup();
                            generateRandomTree();
                            mIsNewExpanded = false;
                            mShowUndoBtn = false;
                        } else if (sf::FloatRect(sf::Vector2f(216.f, 261.f), sf::Vector2f(154.f, 39.f)).contains(worldPos)){
                            mTree.backup();
                            const char* filterPatterns[1] = { "*.txt" };
                            const char* filePath = tinyfd_openFileDialog("Choose Red-Black Tree data file", "", 1, filterPatterns, "Text Files (*.txt)", 0);
                            if (filePath != nullptr){
                                mTree.initializeFromFile(filePath);
                                mTree.resetHistory("Tree loaded from file");
                                mLayout.setDescription("Tree loaded from file!");
                                mShowUndoBtn = false;
                            } else {
                                mLayout.setDescription("Cancel load file.");
                            }
                            mIsNewExpanded = false;
                        }
                    }
                    
                    else if (!mIsInsertExpanded and sf::FloatRect(sf::Vector2f(32.f, 204.f), sf::Vector2f(160.f, 45.f)).contains(worldPos)){
                        mIsNewExpanded = false;
                        mIsInsertExpanded = true;
                        mIsDeleteExpanded = false;
                        mIsSearchExpanded = false;
                        
                        mShowUndoBtn = false;
                        mInputValue = "";
                        clickedInsideAnyForm = true;
                    }
                    
                    else if (mIsInsertExpanded and sf::FloatRect(sf::Vector2f(32.f, 204.f), sf::Vector2f(341.f, 45.f)).contains(worldPos)){
                        clickedInsideAnyForm = true;
                        if (sf::FloatRect(sf::Vector2f(216.f, 207.f), sf::Vector2f(154.f, 39.f)).contains(worldPos) and !mInputValue.empty()){
                            int val = stoi(mInputValue);
                            mTree.backup();
                            mTree.resetHistory("Create new node " + to_string(val) + " (RED)");
                            mTree.insert(val);
                            mTree.setCurrentStep(0);
                            mIsInsertExpanded = false;
                            mInputValue = "";
                            resetPlayUI();
                        } else if (sf::FloatRect(sf::Vector2f(167.f, 211.f), sf::Vector2f(30.f, 30.f)).contains(worldPos)){
                            mInputValue = to_string(rand() % 99 + 1);
                        }
                    }
                    
                    else if (!mIsDeleteExpanded and sf::FloatRect(sf::Vector2f(32.f, 258.f), sf::Vector2f(160.f, 45.f)).contains(worldPos)){
                        mIsNewExpanded = false;
                        mIsDeleteExpanded = true;
                        mIsInsertExpanded = false;
                        mIsSearchExpanded = false;
                        
                        mShowUndoBtn = false;
                        mInputValue = "";
                        clickedInsideAnyForm = true;
                    }
                    
                    else if (mIsDeleteExpanded and sf::FloatRect(sf::Vector2f(32.f, 258.f), sf::Vector2f(341.f, 45.f)).contains(worldPos)){
                        clickedInsideAnyForm = true;
                        if (sf::FloatRect(sf::Vector2f(216.f, 261.f), sf::Vector2f(154.f, 39.f)).contains(worldPos) and !mInputValue.empty()){
                            int val = stoi(mInputValue);
                            mTree.backup();
                            mTree.resetHistory("Start to Remove " + to_string(val));
                            mTree.remove(val);
                            mTree.setCurrentStep(0);
                            mIsDeleteExpanded = false;
                            mInputValue = "";
                            resetPlayUI();
                        } else if (sf::FloatRect(sf::Vector2f(167.f, 265.f), sf::Vector2f(30.f, 30.f)).contains(worldPos)){
                            mInputValue = to_string(rand() % 99 + 1);
                        }
                    }
                    
                    else if (!mIsSearchExpanded and sf::FloatRect(sf::Vector2f(32.f, 312.f), sf::Vector2f(160.f, 45.f)).contains(worldPos)){
                        mIsNewExpanded = false;
                        mIsSearchExpanded = true;
                        mIsInsertExpanded = false;
                        mIsDeleteExpanded = false;
                        
                        mShowUndoBtn = false;
                        mInputValue = "";
                        clickedInsideAnyForm = true;
                    }
                    
                    else if (mIsSearchExpanded and sf::FloatRect(sf::Vector2f(32.f, 312.f), sf::Vector2f(341.f, 45.f)).contains(worldPos)){
                        clickedInsideAnyForm = true;
                        if (sf::FloatRect(sf::Vector2f(216.f, 315.f), sf::Vector2f(154.f, 39.f)).contains(worldPos) and !mInputValue.empty()){
                            int val = stoi(mInputValue);
                            mTree.resetHistory("Start to Search for " + to_string(val));
                            mTree.search(val);
                            mTree.setCurrentStep(0);
                            mIsSearchExpanded = false;
                            mInputValue = "";
                            resetPlayUI();
                        } else if (sf::FloatRect(sf::Vector2f(167.f, 319.f), sf::Vector2f(30.f, 30.f)).contains(worldPos)){
                            mInputValue = to_string(rand() % 99 + 1);
                        }
                    }
                }
                    
                if (!clickedInsideAnyForm){
                    mIsInsertExpanded = false;
                    mIsDeleteExpanded = false;
                    mIsSearchExpanded = false;
                    mIsNewExpanded = false;
                }
            }
        }
         
        else if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()){
            if (keyEvent->code == sf::Keyboard::Key::Z and (keyEvent->system or keyEvent->control)){
                mTree.restore();
                mLayout.setDescription("Undo successfully.");
                mShowUndoBtn = false;
                mLayout.setPaused(true);
                mIsNewExpanded = mIsInsertExpanded = mIsDeleteExpanded = mIsSearchExpanded = false;
                mInputValue = "";
            }
            else if (keyEvent->code == sf::Keyboard::Key::I){
                mIsInsertExpanded = true;
                mIsDeleteExpanded = mIsSearchExpanded = mIsNewExpanded = false;
                mInputValue = "";
            }
            else if (keyEvent->code == sf::Keyboard::Key::D){
                mIsDeleteExpanded = true;
                mIsInsertExpanded = mIsSearchExpanded = mIsNewExpanded = false;
                mInputValue = "";
            }
            else if (keyEvent->code == sf::Keyboard::Key::S){
                mIsSearchExpanded = true;
                mIsInsertExpanded = mIsDeleteExpanded = mIsNewExpanded = false;
                mInputValue = "";
            }
            else if (keyEvent->code == sf::Keyboard::Key::N){
                mIsNewExpanded = true;
                mIsInsertExpanded = mIsDeleteExpanded = mIsSearchExpanded = false;
                mInputValue = "";
            }
            else if (keyEvent->code == sf::Keyboard::Key::Space){
                mLayout.setPaused(!mLayout.isPaused());
            }
            else if (keyEvent->code == sf::Keyboard::Key::R){
                if (mIsInsertExpanded or mIsDeleteExpanded or mIsSearchExpanded){
                    mInputValue = std::to_string(rand() % 99 + 1);
                } else {
                    mTree.backup();
                    generateRandomTree();
                    mShowUndoBtn = false;
                    mIsNewExpanded = false;
                }
            }
            else if (keyEvent->code == sf::Keyboard::Key::Backspace){
                if (mIsInsertExpanded or mIsDeleteExpanded or mIsSearchExpanded){
                    if (!mInputValue.empty()){
                        mInputValue.pop_back();
                    }
                }
            }
            else if (keyEvent->code == sf::Keyboard::Key::Enter){
                if (mIsInsertExpanded and !mInputValue.empty()){
                    int val = stoi(mInputValue);
                    mTree.backup();
                    mTree.resetHistory("Create new node " + to_string(val) + " (RED)");
                    mTree.insert(val);
                    mTree.setCurrentStep(0);
                    mIsInsertExpanded = false;
                    mInputValue = "";
                    resetPlayUI();
                }
                else if (mIsDeleteExpanded and !mInputValue.empty()){
                    int val = stoi(mInputValue);
                    mTree.backup();
                    mTree.resetHistory("Start to Remove " + to_string(val));
                    mTree.remove(val); mTree.setCurrentStep(0);
                    mIsDeleteExpanded = false;
                    mInputValue = "";
                    resetPlayUI();
                }
                else if (mIsSearchExpanded and !mInputValue.empty()){
                    int val = stoi(mInputValue);
                    mTree.resetHistory("Start to Search for " + to_string(val));
                    mTree.search(val);
                    mTree.setCurrentStep(0);
                    mIsSearchExpanded = false;
                    mInputValue = "";
                    resetPlayUI();
                }
            }
            else {
                int digit = -1;
                if (keyEvent->code >= sf::Keyboard::Key::Num0 and keyEvent->code <= sf::Keyboard::Key::Num9){
                    digit = (int)keyEvent->code - (int)sf::Keyboard::Key::Num0;
                } else if (keyEvent->code >= sf::Keyboard::Key::Numpad0 and keyEvent->code <= sf::Keyboard::Key::Numpad9){
                    digit = (int)keyEvent->code - (int)sf::Keyboard::Key::Numpad0;
                }
                if (digit != -1){
                    if (mIsInsertExpanded or mIsDeleteExpanded or mIsSearchExpanded){
                        if (mInputValue.size() < 3){
                            mInputValue += to_string(digit);
                        }
                    }
                }
            }
        }
    }
}

void RBTVisualizer::drawAnimatedTree(const StepState& stepA, const StepState& stepB, float animP){
    if (stepB.nodes.empty()){return;}
    
    auto posA = computeLayout(stepA.nodes);
    auto posB = computeLayout(stepB.nodes);
    
    float p1 = mTransitionProgress;
    float easeP1 = p1 * p1 * (3.0f - 2.0f * p1);
    float easeP2 = animP * animP * (3.0f - 2.0f * animP);
    
    int nodeCount = static_cast<int>(stepB.nodes.size());
    
    int maxDepth = 0;
    map<int, const NodeSnapshot*> byVal;
    set<int> hasParent;
    for (const auto& n : stepB.nodes){
        byVal[n.val] = &n;
        if (n.leftVal != -1){
            hasParent.insert(n.leftVal);
        }
        
        if (n.rightVal != -1){
            hasParent.insert(n.rightVal);
        }
    }
        
    int root = -1;
    for (const auto& n : stepB.nodes){
        if (hasParent.find(n.val) == hasParent.end()){
            root = n.val;
            break;
        }
    }
    
    function<void(int, int)> getDepth = [&](int val, int depth){
        if (depth > maxDepth) maxDepth = depth;
        const NodeSnapshot* n = byVal[val];
        if (n->leftVal != -1 and byVal.count(n->leftVal)){getDepth(n->leftVal, depth + 1);}
        if (n->rightVal != -1 and byVal.count(n->rightVal)){getDepth(n->rightVal, depth + 1);}
    };
    
    if (root != -1){getDepth(root, 0);}

    float safeLeft = mIsControlsVisible ? (427.f + 20.f) : 20.f;
    float safeRight = 1420.f;
    if (mLayout.isDescVisible() or mLayout.isPseudoVisible()){ safeRight = 1028.f - 20.f; }
    float safeTop = 150.f;
    float safeBottom = 670.f;

    float maxTreeWidth = (safeRight - safeLeft) - 70.f;
    float maxTreeHeight = (safeBottom - safeTop) - 70.f;

    float baseX = 55.f;
    float baseY = 85.f;
    float naturalWidth = (nodeCount > 1) ? (nodeCount - 1) * baseX : 0.f;

    float scaleX = (naturalWidth > maxTreeWidth) ? maxTreeWidth / naturalWidth : 1.0f;
    float scaleY = (maxDepth > 0 && maxDepth * baseY > maxTreeHeight) ? maxTreeHeight / (maxDepth * baseY) : 1.0f;
    float visScale = min(scaleX, scaleY);
        
    float menuScale = 300.f / (naturalWidth + 70.f);
    if (menuScale > 1.0f) menuScale = 1.0f;
    
    float currentScale = menuScale + (visScale - menuScale) * easeP1;
    
    const float R = 35.f * currentScale;
    float lineThickness = 4.f * currentScale;
    float mainStroke = 3.f * currentScale;
    int fontSize = static_cast<int>(30.f * currentScale);
    
    sf::Color edgeColor = ThemeManager::current.primary;
    
    map<int, sf::Vector2f> currentPos;
    for (const auto& n : stepB.nodes){
        sf::Vector2f endPos = posB[n.val];
        sf::Vector2f startPos;
        
        if (posA.count(n.val)){
            startPos = posA[n.val];
        } else {
            startPos = sf::Vector2f(300.f, 250.f);
        }
        
        currentPos[n.val] = startPos + (endPos - startPos) * easeP2;
    }

    auto drawThickLine = [&](sf::Vector2f p1, sf::Vector2f p2){
        sf::Vector2f dir = p2 - p1;
        float dist = sqrt(dir.x * dir.x + dir.y * dir.y);
        sf::RectangleShape line(sf::Vector2f(dist, lineThickness));
        line.setOrigin(sf::Vector2f(0.f, lineThickness / 2.0f));
        line.setPosition(p1);
        line.setFillColor(edgeColor);
        line.setRotation(sf::radians(atan2(dir.y, dir.x)));
        mWindow->draw(line);
    };

    for (auto& n : stepB.nodes){
        auto from = currentPos[n.val];
        
        if (n.leftVal != -1 and currentPos.count(n.leftVal)){
            auto to = currentPos[n.leftVal];
            sf::Vector2f dir = to - from;
            float dist = sqrt(dir.x * dir.x + dir.y * dir.y);
            if (dist > R * 2){
                sf::Vector2f unit = dir / dist;
                drawThickLine(from + unit * R, to - unit * R);
            }
        }
        if (n.rightVal != -1 and currentPos.count(n.rightVal)){
            auto to = currentPos[n.rightVal];
            sf::Vector2f dir = to - from;
            float dist = sqrt(dir.x * dir.x + dir.y * dir.y);
            if (dist > R * 2){
                sf::Vector2f unit = dir / dist;
                drawThickLine(from + unit * R, to - unit * R);
            }
        }
    }
    
    for (auto& n : stepB.nodes){
        sf::Vector2f p = currentPos[n.val];
        
        if (n.val == stepB.highlightedNode and easeP1 == 1.0f){
            float outerR = R + mainStroke + 5.f;
            sf::CircleShape highlightCirc(outerR);
            highlightCirc.setPointCount(150);
            highlightCirc.setOrigin(sf::Vector2f(outerR, outerR));
            highlightCirc.setPosition(p);
            highlightCirc.setFillColor(sf::Color::Yellow);
            mWindow->draw(highlightCirc);
        }
        
        sf::CircleShape circle(R);
        circle.setPointCount(150);
        circle.setOrigin(sf::Vector2f(R, R));
        circle.setPosition(p);
        circle.setOutlineThickness(mainStroke);
        
        sf::Color textColor;
        
        if (n.isRed){
            circle.setFillColor(sf::Color(255, 220, 225));
            circle.setOutlineColor(sf::Color(230, 57, 70));
            textColor = sf::Color(230, 57, 70);
        } else {
            circle.setFillColor(sf::Color(172, 172, 172));
            circle.setOutlineColor(ThemeManager::current.textColor);
            textColor = sf::Color(41, 42, 49);
        }
        
        mWindow->draw(circle);
        
        string labelStr = (n.isDummy) ? "NULL" : to_string(n.val);
        int finalFontSize = (n.isDummy) ? fontSize - 5 : fontSize;
        
        sf::Text label(mFontRegular, labelStr, finalFontSize);
        label.setFillColor(textColor);
        sf::FloatRect bounds = label.getLocalBounds();
        label.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2.0f, bounds.position.y + bounds.size.y / 2.0f));
        label.setPosition(p);
        mWindow->draw(label);
    }
}

void RBTVisualizer::render(bool showUI){
    mNewHoverStroke.setThemeColor(ThemeManager::current.primary);
    mNewExpandedStroke.setThemeColor(ThemeManager::current.primary);
    mInsertHoverStroke.setThemeColor(ThemeManager::current.primary);
    mInsertExpandedStroke.setThemeColor(ThemeManager::current.primary);
    mDeleteHoverStroke.setThemeColor(ThemeManager::current.primary);
    mDeleteExpandedStroke.setThemeColor(ThemeManager::current.primary);
    mSearchHoverStroke.setThemeColor(ThemeManager::current.primary);
    mSearchExpandedStroke.setThemeColor(ThemeManager::current.primary);
    
    if (!showUI and mTransitionProgress == 1.0f){
        setTransitionProgress(0.0f);
    }
    
    auto history = mTree.getStepHistory();
    int cur = mTree.getCurrentStep();
    int total = static_cast<int>(history.size());
    
    if (mTargetStep != -1 and mTargetStep != cur){
        float speedSec = mTree.getVisualizationSpeed() / 1000.f;
        float frameDelta = 1.0f / (speedSec * 60.0f);
        
        mStepAnimProgress += frameDelta;
        
        if (mStepAnimProgress >= 1.0f){
            mStepAnimProgress = 1.0f;
            mTree.setCurrentStep(mTargetStep);
            cur = mTargetStep;
            
            if (!mLayout.isPaused() and cur < total - 1){
                mTargetStep = cur + 1;
                mStepAnimProgress = 0.0f;
            } else {
                mTargetStep = -1;
            }
        }
    }
    
    else if (!mLayout.isPaused() and showUI and cur < total - 1 and mTargetStep == -1){
        mTargetStep = cur + 1;
        mStepAnimProgress = 0.0f;
    }
    
    if (cur >= 0 and cur < total){
        if (mTargetStep != -1 and mTargetStep < total and mStepAnimProgress < 1.0f){
            drawAnimatedTree(history[cur], history[mTargetStep], mStepAnimProgress);
        } else {
            drawAnimatedTree(history[cur], history[cur], 1.0f);
        }
    }
    
    if (showUI){
        setTransitionProgress(1.0f);
        
        mTitleNum.setFillColor(ThemeManager::current.textColor);
        mTitleBlack.setFillColor(ThemeManager::current.textColor);
        mTitleTree.setFillColor(ThemeManager::current.primary);
        
        if (mInsertDiceBtn.mSprite){mInsertDiceBtn.mSprite->setColor(ThemeManager::current.textColor);}
        if (mDeleteDiceBtn.mSprite){mDeleteDiceBtn.mSprite->setColor(ThemeManager::current.textColor);}
        if (mSearchDiceBtn.mSprite){mSearchDiceBtn.mSprite->setColor(ThemeManager::current.textColor);}

        mClearBtn.setThemeColor(ThemeManager::current.secondary);
        mNewBtn.setThemeColor(ThemeManager::current.secondary);
        mInsertBtn.setThemeColor(ThemeManager::current.secondary);
        mDeleteBtn.setThemeColor(ThemeManager::current.secondary);
        mSearchBtn.setThemeColor(ThemeManager::current.secondary);
        mUndoBtn.setThemeColor(ThemeManager::current.secondary);
            
        mUploadBtn.setThemeColor(ThemeManager::current.screenBg);
        mRandomBtn.setThemeColor(ThemeManager::current.screenBg);
        mConfirmAddBtn.setThemeColor(ThemeManager::current.screenBg);
        mConfirmRemoveBtn.setThemeColor(ThemeManager::current.screenBg);
        mConfirmSearchBtn.setThemeColor(ThemeManager::current.screenBg);
        
        mInsertExpandedBg.setThemeColor(ThemeManager::current.secondary);
        mDeleteExpandedBg.setThemeColor(ThemeManager::current.secondary);
        mSearchExpandedBg.setThemeColor(ThemeManager::current.secondary);
        mNewExpandedBg.setThemeColor(ThemeManager::current.secondary);

        mInsertInputText.setFillColor(ThemeManager::current.textColor);
        mDeleteInputText.setFillColor(ThemeManager::current.textColor);
        mSearchInputText.setFillColor(ThemeManager::current.textColor);
        mInsertCursorLine.setFillColor(ThemeManager::current.textColor);
        mDeleteCursorLine.setFillColor(ThemeManager::current.textColor);
        mSearchCursorLine.setFillColor(ThemeManager::current.textColor);
        
        sf::Vector2f worldPos = mWindow->mapPixelToCoords(sf::Mouse::getPosition(*mWindow));
        
        if (cur >= 0 and cur < static_cast<int>(history.size())){
            mLayout.setDescription(history[cur].description);
            
            mLayout.setPseudoCode(history[cur].codeLines);
            mLayout.setActiveCodeLine(history[cur].activeLine);
        }
            
        int total = static_cast<int>(mTree.getStepHistory().size());
        int current = mTree.getCurrentStep();
        mLayout.setStepText("Step " + to_string(current + 1) + " / " + to_string(total));
        mLayout.draw(*mWindow);
            
        mWindow->draw(mTitleNum);
        mWindow->draw(mTitleRed);
        mWindow->draw(mTitleBlack);
        mWindow->draw(mTitleTree);

        if (mIsControlsVisible){
            mHideControlsBtn.setThemeColor(ThemeManager::current.secondary);
            mHideControlsBtn.draw(*mWindow);
            
            if (mShowUndoBtn){
                mUndoBtn.draw(*mWindow);
            } else {mClearBtn.draw(*mWindow);}
            
            if (mShowUndoBtn){
                mUndoBtn.draw(*mWindow);
            } else {
                mClearBtn.draw(*mWindow);
            }
            
            if (mIsNewExpanded){
                mNewExpandedStroke.draw(*mWindow);
                mNewExpandedBg.draw(*mWindow);
                mNewBtn.draw(*mWindow);
                mRandomBtn.draw(*mWindow);
                mUploadBtn.draw(*mWindow);
            } else {
                if (sf::FloatRect(sf::Vector2f(213.f, 150.f), sf::Vector2f(160.f, 45.f)).contains(worldPos)){
                    mNewHoverStroke.draw(*mWindow);
                }
                mNewBtn.draw(*mWindow);
            }
            
            if (mIsInsertExpanded){
                mInsertExpandedStroke.draw(*mWindow);
                mInsertExpandedBg.draw(*mWindow);
                mConfirmAddBtn.draw(*mWindow);
                mInsertDiceBtn.draw(*mWindow);
                
                mInsertInputText.setString(mInputValue);
                mWindow->draw(mInsertInputText);
                
                if (mCursorClock.getElapsedTime().asSeconds() >= 0.5f){
                    mShowCursor = !mShowCursor;
                    mCursorClock.restart();
                }
                
                if (mShowCursor){
                    float textWidth = mInsertInputText.getLocalBounds().size.x;
                    float cursorX = mInputValue.empty() ? 47.f : 47.f + textWidth + 2.f;
                    mInsertCursorLine.setPosition(sf::Vector2f(cursorX, 214.f));
                    mWindow->draw(mInsertCursorLine);
                }
            } else {
                if (sf::FloatRect(sf::Vector2f(32.f, 204.f), sf::Vector2f(160.f, 45.f)).contains(worldPos)){
                    mInsertHoverStroke.draw(*mWindow);
                }
                mInsertBtn.draw(*mWindow);
            }
            
            if (mIsDeleteExpanded){
                mDeleteExpandedStroke.draw(*mWindow);
                mDeleteExpandedBg.draw(*mWindow);
                mConfirmRemoveBtn.draw(*mWindow);
                mDeleteDiceBtn.draw(*mWindow);
                
                mDeleteInputText.setString(mInputValue);
                mWindow->draw(mDeleteInputText);
                
                if (mCursorClock.getElapsedTime().asSeconds() >= 0.5f){
                    mShowCursor = !mShowCursor;
                    mCursorClock.restart();
                }
                
                if (mShowCursor){
                    float textWidth = mDeleteInputText.getLocalBounds().size.x;
                    float cursorX = mInputValue.empty() ? 47.f : 47.f + textWidth + 2.f;
                    mDeleteCursorLine.setPosition(sf::Vector2f(cursorX, 268.f));
                    mWindow->draw(mDeleteCursorLine);
                }
            } else {
                if (sf::FloatRect(sf::Vector2f(32.f, 258.f), sf::Vector2f(160.f, 45.f)).contains(worldPos)){
                    mDeleteHoverStroke.draw(*mWindow);
                }
                mDeleteBtn.draw(*mWindow);
            }
            
            if (mIsSearchExpanded){
                mSearchExpandedStroke.draw(*mWindow);
                mSearchExpandedBg.draw(*mWindow);
                mConfirmSearchBtn.draw(*mWindow);
                mSearchDiceBtn.draw(*mWindow);
                
                mSearchInputText.setString(mInputValue);
                mWindow->draw(mSearchInputText);
                
                if (mCursorClock.getElapsedTime().asSeconds() >= 0.5f){
                    mShowCursor = !mShowCursor;
                    mCursorClock.restart();
                }
                
                if (mShowCursor){
                    float textWidth = mSearchInputText.getLocalBounds().size.x;
                    float cursorX = mInputValue.empty() ? 47.f : 47.f + textWidth + 2.f;
                    mSearchCursorLine.setPosition(sf::Vector2f(cursorX, 322.f));
                    mWindow->draw(mSearchCursorLine);
                }
            } else {
                if (sf::FloatRect(sf::Vector2f(32.f, 312.f), sf::Vector2f(160.f, 45.f)).contains(worldPos)){
                    mSearchHoverStroke.draw(*mWindow);
                }
                mSearchBtn.draw(*mWindow);
            }
        } else {
            mShowControlsBtn.setThemeColor(ThemeManager::current.secondary);
            mShowControlsBtn.draw(*mWindow);
        }
    }
}

std::map<int, sf::Vector2f> RBTVisualizer::computeLayout(const std::vector<NodeSnapshot>& nodes){
    if (nodes.empty()){
        return {};
    }

    map<int, const NodeSnapshot*> byVal;
    set<int> allVals, hasParent;
    
    for (const auto& n : nodes){
        byVal[n.val] = &n;
        allVals.insert(n.val);
        if (n.leftVal != -1){
            hasParent.insert(n.leftVal);
        }
        
        if (n.rightVal != -1){
            hasParent.insert(n.rightVal);
        }
    }

    int root = -1;
    for (int v : allVals){
        if (hasParent.find(v) == hasParent.end()){
            root = v;
            break;
        }
    }

    map<int, sf::Vector2f> pos;
    int inorderIdx = 0;
        
    float p = mTransitionProgress;
    float easeP = p * p * (3.0f - 2.0f * p);
        
    int nodeCount = static_cast<int>(nodes.size());
    
    float safeLeft = mIsControlsVisible ? (427.f + 20.f) : 20.f;
    
    float safeRight = 1420.f;
    if (mLayout.isDescVisible() or mLayout.isPseudoVisible()){safeRight = 1028.f - 20.f;}
    float safeTop = 150.f;
    float safeBottom = 670.f;

    float maxTreeWidth = (safeRight - safeLeft) - 70.f;
    float maxTreeHeight = (safeBottom - safeTop) - 70.f;

    int maxDepth = 0;
    function<void(int, int)> getDepth = [&](int val, int depth){
        if (depth > maxDepth) maxDepth = depth;
        const NodeSnapshot* n = byVal[val];
        if (n->leftVal != -1 and byVal.count(n->leftVal)){
            getDepth(n->leftVal, depth + 1);
        }
        
        if (n->rightVal != -1 and byVal.count(n->rightVal)){
            getDepth(n->rightVal, depth + 1);
        }
    };
        
    if (root != -1){getDepth(root, 0);}
        
    float baseX = 55.f;
    float baseY = 85.f;
        
    float naturalWidth = (nodeCount > 1) ? (nodeCount - 1) * baseX : 0.f;
        
    float scaleX = (naturalWidth > maxTreeWidth) ? maxTreeWidth / naturalWidth : 1.0f;
    float scaleY = (maxDepth > 0 && maxDepth * baseY > maxTreeHeight) ? maxTreeHeight / (maxDepth * baseY) : 1.0f;
    float visScale = min(scaleX, scaleY);
        
    float menuScale = 300.f / (naturalWidth + 70.f);
    if (menuScale > 1.0f){menuScale = 1.0f;}
    
    float currentScale = menuScale + (visScale - menuScale) * easeP;
        
    float xSpacing = baseX * currentScale;
    float ySpacing = baseY * currentScale;
        
    float totalWidth = (nodeCount > 1) ? (nodeCount - 1) * xSpacing : 0.f;
    float visCenterX = (safeLeft + safeRight) / 2.0f;
    float menuCenterX = 900.f;
        
    float menuX = menuCenterX - totalWidth / 2.0f;
    float visX = visCenterX - totalWidth / 2.0f;
        
    float menuY = 300.f;
    float visY = safeTop + 35.f * currentScale;
        
    float xOffset = menuX + (visX - menuX) * easeP;
    float yStart = menuY + (visY - menuY) * easeP;
    
    function<void(int, int)> dfs = [&](int val, int depth){
        const NodeSnapshot* n = byVal[val];
        if (n->leftVal != -1 and byVal.count(n->leftVal)){
            dfs(n->leftVal, depth + 1);
        }
        
        pos[val] = {static_cast<float>(inorderIdx) * xSpacing + xOffset, yStart + static_cast<float>(depth) * ySpacing};
        inorderIdx++;
        
        if (n->rightVal != -1 and byVal.count(n->rightVal)){
            dfs(n->rightVal, depth + 1);
        }
    };

    if (root != -1){dfs(root, 0);}
    return pos;
}

bool RBTVisualizer::checkReturnHome(){
    if (mGoHome){
        mGoHome = false;
        return true;
    }
    return false;
}

void RBTVisualizer::generateRandomTree(){
    mTree.initialize();
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 99);
    
    for (int i = 0; i<7; ++i){
        mTree.insert(dis(gen));
    }
    
    mTree.resetHistory("Random new tree");
}

bool RBTVisualizer::isEmpty(){
    auto history = mTree.getStepHistory();
    int cur = mTree.getCurrentStep();
    if (cur >= 0 and cur < static_cast<int>(history.size())){
        return history[cur].nodes.empty();
    }
    return true;
}

void RBTVisualizer::resetPlayUI(){
    mLayout.setPaused(false);
    
    mTree.setCurrentStep(0);
    mTargetStep = -1;
    mStepAnimProgress = 1.0f;
    
    mShowUndoBtn = false;
    mAutoPlayClock.restart();
}
