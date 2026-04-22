#include "RBTVisualizer.h"
#include "Common.h"

RBTVisualizer::RBTVisualizer(sf::RenderWindow& window)
    : mWindow(&window),
      mHeaderText(mFontBold), mTitleNum(mFontBold),
      mTitleRed(mFontBold), mTitleBlack(mFontBold), mTitleTree(mFontBold), mDescriptionText(mFontRegular),

      mStepText(mFontRegular),
      mClearBtn(mFontRegular, "Clear tree", 70.f, 176.f, 160.f, 45.f, 21.f, sf::Color(217, 217, 217)),
      mNewBtn(mFontRegular, "New tree", 251.f, 176.f, 160.f, 45.f, 21.f, sf::Color(217, 217, 217)),
      mInsertBtn(mFontRegular, "Insert", 70.f, 230.f, 160.f, 45.f, 21.f, sf::Color(217, 217, 217)),
      mDeleteBtn(mFontRegular, "Delete", 70.f, 284.f, 160.f, 45.f, 21.f, sf::Color(217, 217, 217)),
      mSearchBtn(mFontRegular, "Search", 70.f, 338.f, 160.f, 45.f, 21.f, sf::Color(217, 217, 217)),
      mUndoBtn(mFontRegular, "Undo", 70.f, 176.f, 160.f, 45.f, 21.f, sf::Color(217, 217, 217)),
      
      mSpeedSlider(mFontRegular, 956.f, 921.f),
      mStepBackBtn(mFontRegular, "Step back", 326.f, 896.f, 165.f, 49.f, 24.5f, sf::Color(245, 245, 240)),
      mStepForwardBtn(mFontRegular, "Step forward", 504.f, 896.f, 201.f, 49.f, 24.5f, sf::Color(245, 245, 240)),

      mInsertHoverStroke(mFontRegular, "", 68.f, 228.f, 164.f, 49.f, 23.f, sf::Color(90, 150, 44)),
      mInsertExpandedStroke(mFontRegular, "", 68.f, 228.f, 345.f, 49.f, 23.f, sf::Color(90, 150, 44)),
      mInsertExpandedBg(mFontRegular, "", 70.f, 230.f, 341.f, 45.f, 21.f, sf::Color(217, 217, 217)),
      mConfirmAddBtn(mFontRegular, "Add", 254.f, 233.f, 154.f, 39.f, 19.5f, sf::Color(245, 245, 240)),
      mInsertInputText(mFontRegular),

      mDeleteHoverStroke(mFontRegular, "", 68.f, 282.f, 164.f, 49.f, 23.f, sf::Color(90, 150, 44)),
      mDeleteExpandedStroke(mFontRegular, "", 68.f, 282.f, 345.f, 49.f, 23.f, sf::Color(90, 150, 44)),
      mDeleteExpandedBg(mFontRegular, "", 70.f, 284.f, 341.f, 45.f, 21.f, sf::Color(217, 217, 217)),
      mConfirmRemoveBtn(mFontRegular, "Remove", 254.f, 287.f, 154.f, 39.f, 19.5f, sf::Color(245, 245, 240)),
      mDeleteInputText(mFontRegular),

      mSearchHoverStroke(mFontRegular, "", 68.f, 336.f, 164.f, 49.f, 23.f, sf::Color(90, 150, 44)),
      mSearchExpandedStroke(mFontRegular, "", 68.f, 336.f, 345.f, 49.f, 23.f, sf::Color(90, 150, 44)),
      mSearchExpandedBg(mFontRegular, "", 70.f, 338.f, 341.f, 45.f, 21.f, sf::Color(217, 217, 217)),
      mConfirmSearchBtn(mFontRegular, "Search", 254.f, 341.f, 154.f, 39.f, 19.5f, sf::Color(245, 245, 240)),
      mSearchInputText(mFontRegular)
{
    if (!mFontBold.openFromFile("assets/fonts/Inter-Bold.ttf") or !mFontRegular.openFromFile("assets/fonts/Inter-Regular.ttf")){
        cerr << "Cannot load font!" << endl;
    }
    
    if (!mHomeTex.loadFromFile("assets/images/homeButton.png")){cerr << "Cannot load homeButton.png" << endl;}
    if (!mSkipBackTex.loadFromFile("assets/images/skipbackButton.png")){cerr << "Cannot load skipbackButton.png" << endl;}
    if (!mPauseTex.loadFromFile("assets/images/pauseButton.png")){cerr << "Cannot load pauseButton.png" << endl; }
    if (!mSkipForwardTex.loadFromFile("assets/images/skipforwardButton.png")){cerr << "Cannot load skipforwardButton.png" << endl;}
    if (!mStartTex.loadFromFile("assets/images/startButton.png")){cerr << "Loi load startButton.png" << endl;}

    mHomeTex.setSmooth(true);
    mSkipBackTex.setSmooth(true);
    mPauseTex.setSmooth(true);
    mSkipForwardTex.setSmooth(true);
    mStartTex.setSmooth(true);
    
    mHomeTex.generateMipmap();
    mSkipBackTex.generateMipmap();
    mPauseTex.generateMipmap();
    mSkipForwardTex.generateMipmap();
    mStartTex.generateMipmap();
    
    mHomeBtn.setup(mHomeTex, 67.f, 62.f, 48.f, 48.f);
    mSkipBackBtn.setup(mSkipBackTex, 333.f, 897.f, 48.f, 48.f);
    mPauseBtn.setup(mPauseTex, 397.f, 897.f, 48.f, 48.f);
    mSkipForwardBtn.setup(mSkipForwardTex, 461.f, 897.f, 48.f, 48.f);
    mStartBtn.setup(mStartTex, 134.f, 897.f, 48.f, 48.f);

    mHomeBtn.setCallback([this](){
        mGoHome = true;
    });
    
    mPauseBtn.setCallback([this](){
        mIsPaused = true;
        mSkipBackBtn.setup(mSkipBackTex, 67.f, 897.f, 48.f, 48.f);
        mSkipForwardBtn.setup(mSkipForwardTex, 735.f, 897.f, 48.f, 48.f);
    });

    mStartBtn.setCallback([this](){
        mIsPaused = false;
        mSkipBackBtn.setup(mSkipBackTex, 333.f, 897.f, 48.f, 48.f);
        mSkipForwardBtn.setup(mSkipForwardTex, 461.f, 897.f, 48.f, 48.f);
    });
    
    mSkipBackBtn.setCallback([this](){
        mTree.setCurrentStep(0);
        mIsPaused = true;
    });

    mStepBackBtn.setCallback([this](){
        int cur = mTree.getCurrentStep();
        if (cur > 0){
            mTargetStep = cur - 1;
            mStepAnimProgress = 0.0f;
        }
        mIsPaused = true;
    });

    mStepForwardBtn.setCallback([this](){
        int cur = mTree.getCurrentStep();
        if (cur < mTree.getStepHistory().size() - 1){
            mTargetStep = cur + 1;
            mStepAnimProgress = 0.0f;
        }
        mIsPaused = true;
    });

    mSkipForwardBtn.setCallback([this](){
        mTree.goToFinalStep();
        mIsPaused = true;
    });
    
    mClearBtn.refreshText();
    mNewBtn.refreshText();
    mInsertBtn.refreshText();
    mDeleteBtn.refreshText();
    mSearchBtn.refreshText();
    mStepBackBtn.refreshText();
    mStepForwardBtn.refreshText();
    mConfirmAddBtn.refreshText();
    mConfirmRemoveBtn.refreshText();
    mConfirmSearchBtn.refreshText();
    mUndoBtn.refreshText();

    if (!mDiceTex.loadFromFile("assets/images/randomButton.png")){cerr << "Loi load randomButton.png" << endl;}
    mDiceTex.setSmooth(true);
    mDiceTex.generateMipmap();
    
    mInsertDiceBtn.setup(mDiceTex, 205.f, 237.f, 30.f, 30.f);
    mDeleteDiceBtn.setup(mDiceTex, 205.f, 291.f, 30.f, 30.f);
    mSearchDiceBtn.setup(mDiceTex, 205.f, 345.f, 30.f, 30.f);
    
    mInsertInputText.setFont(mFontRegular);
    mInsertInputText.setCharacterSize(22);
    mInsertInputText.setFillColor(sf::Color::Black);
    mInsertInputText.setPosition(sf::Vector2f(85.f, 238.f));
    
    mDeleteInputText.setFont(mFontRegular);
    mDeleteInputText.setCharacterSize(22);
    mDeleteInputText.setFillColor(sf::Color::Black);
    mDeleteInputText.setPosition(sf::Vector2f(85.f, 292.f));
    
    mSearchInputText.setFont(mFontRegular);
    mSearchInputText.setCharacterSize(22);
    mSearchInputText.setFillColor(sf::Color::Black);
    mSearchInputText.setPosition(sf::Vector2f(85.f, 346.f));
    
    mInsertCursorLine.setSize(sf::Vector2f(2.f, 24.f));
    mInsertCursorLine.setFillColor(sf::Color::Black);
    
    mDeleteCursorLine.setSize(sf::Vector2f(2.f, 24.f));
    mDeleteCursorLine.setFillColor(sf::Color::Black);
    
    mSearchCursorLine.setSize(sf::Vector2f(2.f, 24.f));
    mSearchCursorLine.setFillColor(sf::Color::Black);

    mBackground.setSize({1440.f, 960.f});
    mBackground.setFillColor(sf::Color(245, 245, 240));

    mFooter.setSize({1440.f, 78.f});
    mFooter.setPosition({0.f, 882.f});
    mFooter.setFillColor(sf::Color(217, 217, 217));

    mHeaderText.setString("Data Structure Visualization");
    mHeaderText.setCharacterSize(30);
    mHeaderText.setFillColor(sf::Color::Black);
    mHeaderText.setPosition({130.f, 68.f});

    mTitleNum.setString("03");
    mTitleNum.setCharacterSize(30);
    mTitleNum.setFillColor(sf::Color::Black);
    mTitleNum.setPosition(sf::Vector2f(67.f, 663.f));

    mTitleRed.setString("Red");
    mTitleRed.setCharacterSize(70);
    mTitleRed.setFillColor(sf::Color(230, 57, 70));
    mTitleRed.setPosition(sf::Vector2f(67.f, 695.f));
    
    mTitleBlack.setString("-Black");
    mTitleBlack.setCharacterSize(70);
    mTitleBlack.setFillColor(sf::Color::Black);
    mTitleBlack.setPosition(sf::Vector2f(79.f + mTitleRed.getLocalBounds().size.x, 695.f));

    mTitleTree.setString("Tree");
    mTitleTree.setCharacterSize(70);
    mTitleTree.setFillColor(sf::Color(89, 149, 43));
    mTitleTree.setPosition(sf::Vector2f(67.f, 765.f));
    
    float bgX = 131.f, bgY = 890.f, bgW = 580.f, bgH = 61.f, bgR = 30.5f;
    sf::Color bgColor(196, 196, 196);
        
    mStepBgLeft.setRadius(bgR);
    mStepBgLeft.setPosition(sf::Vector2f(bgX, bgY));
    mStepBgLeft.setFillColor(bgColor);
    
    mStepBgRight.setRadius(bgR);
    mStepBgRight.setPosition(sf::Vector2f(bgX + bgW - bgH, bgY));
    mStepBgRight.setFillColor(bgColor);
    
    mStepBgCenter.setSize(sf::Vector2f(bgW - bgH, bgH));
    mStepBgCenter.setPosition(sf::Vector2f(bgX + bgR, bgY));
    mStepBgCenter.setFillColor(bgColor);

    mStepText.setString("Step 0 / 0");
    mStepText.setCharacterSize(21);
    mStepText.setFillColor(sf::Color::Black);
    mStepText.setOrigin(sf::Vector2f(0.f, 0.f));
    mStepText.setPosition(sf::Vector2f(192.f, 906.f));

    mDescriptionBox.setSize({400.f, 150.f});
    mDescriptionBox.setFillColor(sf::Color(217, 217, 217));
    mDescriptionBox.setPosition({1018.f, 165.f});

    mDescriptionText.setString("Step State Description");
    mDescriptionText.setCharacterSize(20);
    mDescriptionText.setFillColor(sf::Color::Black);
    mDescriptionText.setPosition({1040.f, 185.f});

    mClearBtn.setCallback([this](){
        mTree.backup();
        mTree.initialize();
        mTree.resetHistory("Tree cleared");
        mDescriptionText.setString("Tree cleared.");
        mShowUndoBtn = true;
    });
    
    mUndoBtn.setCallback([this](){
        mTree.restore();
        mDescriptionText.setString("Tree restored.");
        mShowUndoBtn = false;
    });
        
    mNewBtn.setCallback([this](){
        generateRandomTree();
        mDescriptionText.setString("Tree random initialized.");
    });
      
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
    
    if (mShowUndoBtn){
        mUndoBtn.update(mousePos);
    } else {
        mClearBtn.update(mousePos);
    }
    
    mNewBtn.update(mousePos);
    mInsertBtn.update(mousePos);
    mDeleteBtn.update(mousePos);
    mSearchBtn.update(mousePos);
    mSpeedSlider.update(mousePos);
        
    float speedMult = mSpeedSlider.getSpeed();
    
    if (speedMult > 0.0f){
        int speedMs = static_cast<int>(500.f / speedMult);
        mTree.setVisualizationSpeed(speedMs);
    }
    
    mHomeBtn.update(mousePos);
    mSkipBackBtn.update(mousePos);
    mSkipForwardBtn.update(mousePos);
    
    if (mIsPaused){
        mStartBtn.update(mousePos);
        mStepBackBtn.update(mousePos);
        mStepForwardBtn.update(mousePos);
    } else {
        mPauseBtn.update(mousePos);
    }
    
    if (mIsInsertExpanded){
        mConfirmAddBtn.update(mousePos);
        mInsertDiceBtn.update(mousePos);
    } else {
        mInsertBtn.update(mousePos);
    }

    if (event){
        if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()){
            if (mouseEvent->button == sf::Mouse::Button::Left){
                bool clickedInsideAnyForm = false;

                if (!mIsInsertExpanded and sf::FloatRect(sf::Vector2f(70.f, 230.f), sf::Vector2f(160.f, 45.f)).contains(worldPos)){
                    mIsInsertExpanded = true; mIsDeleteExpanded = false; mIsSearchExpanded = false;
                    mInputValue = "";
                    clickedInsideAnyForm = true;
                }
                
                else if (mIsInsertExpanded and sf::FloatRect(sf::Vector2f(70.f, 230.f), sf::Vector2f(341.f, 45.f)).contains(worldPos)){
                    clickedInsideAnyForm = true;
                    
                    if (sf::FloatRect(sf::Vector2f(254.f, 233.f), sf::Vector2f(154.f, 39.f)).contains(worldPos) and !mInputValue.empty()){
                        int val = stoi(mInputValue);
                        mTree.backup(); mTree.resetHistory("Create new node " + to_string(val) + " (RED)");
                        mTree.insert(val); mTree.setCurrentStep(0);
                        mIsInsertExpanded = false; mInputValue = ""; mIsPaused = false;
                        mSkipBackBtn.setup(mSkipBackTex, 333.f, 897.f, 48.f, 48.f);
                        mSkipForwardBtn.setup(mSkipForwardTex, 461.f, 897.f, 48.f, 48.f);
                        mAutoPlayClock.restart();
                    }
                    
                    else if (sf::FloatRect(sf::Vector2f(205.f, 237.f), sf::Vector2f(30.f, 30.f)).contains(worldPos)){
                        mInputValue = to_string(rand() % 99 + 1);
                    }
                }

                if (!mIsDeleteExpanded and sf::FloatRect(sf::Vector2f(70.f, 284.f), sf::Vector2f(160.f, 45.f)).contains(worldPos)){
                    mIsDeleteExpanded = true; mIsInsertExpanded = false; mIsSearchExpanded = false;
                    mInputValue = ""; clickedInsideAnyForm = true;
                }
                
                else if (mIsDeleteExpanded and sf::FloatRect(sf::Vector2f(70.f, 284.f), sf::Vector2f(341.f, 45.f)).contains(worldPos)){
                    clickedInsideAnyForm = true;
                    
                    if (sf::FloatRect(sf::Vector2f(254.f, 287.f), sf::Vector2f(154.f, 39.f)).contains(worldPos) and !mInputValue.empty()){
                        int val = stoi(mInputValue);
                        mTree.backup();
                        mTree.resetHistory("Start to Remove " + to_string(val));
                        mTree.remove(val); mTree.setCurrentStep(0);
                        mIsDeleteExpanded = false; mInputValue = ""; mIsPaused = false;
                        mSkipBackBtn.setup(mSkipBackTex, 333.f, 897.f, 48.f, 48.f);
                        mSkipForwardBtn.setup(mSkipForwardTex, 461.f, 897.f, 48.f, 48.f);
                        mAutoPlayClock.restart();
                    }
                    
                    else if (sf::FloatRect(sf::Vector2f(205.f, 291.f), sf::Vector2f(30.f, 30.f)).contains(worldPos)){
                        mInputValue = to_string(rand() % 99 + 1);
                    }
                }
                
                if (!mIsSearchExpanded and sf::FloatRect(sf::Vector2f(70.f, 338.f), sf::Vector2f(160.f, 45.f)).contains(worldPos)){
                    mIsSearchExpanded = true; mIsInsertExpanded = false; mIsDeleteExpanded = false;
                    mInputValue = ""; clickedInsideAnyForm = true;
                }
                
                else if (mIsSearchExpanded and sf::FloatRect(sf::Vector2f(70.f, 338.f), sf::Vector2f(341.f, 45.f)).contains(worldPos)){
                    clickedInsideAnyForm = true;
                    
                    if (sf::FloatRect(sf::Vector2f(254.f, 341.f), sf::Vector2f(154.f, 39.f)).contains(worldPos) and !mInputValue.empty()){
                        int val = stoi(mInputValue);
                        mTree.resetHistory("Start to Search for " + to_string(val));
                        mTree.search(val); mTree.setCurrentStep(0);
                        mIsSearchExpanded = false; mInputValue = ""; mIsPaused = false;
                        mSkipBackBtn.setup(mSkipBackTex, 333.f, 897.f, 48.f, 48.f);
                        mSkipForwardBtn.setup(mSkipForwardTex, 461.f, 897.f, 48.f, 48.f);
                        mAutoPlayClock.restart();
                    }
                    
                    else if (sf::FloatRect(sf::Vector2f(205.f, 345.f), sf::Vector2f(30.f, 30.f)).contains(worldPos)){
                        mInputValue = to_string(rand() % 99 + 1);
                    }
                }
                 
                if (!clickedInsideAnyForm){
                    mIsInsertExpanded = false; mIsDeleteExpanded = false; mIsSearchExpanded = false;
                }
            }
        }
        
        if (mIsInsertExpanded or mIsDeleteExpanded or mIsSearchExpanded){
            if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()){
                if (keyEvent->code == sf::Keyboard::Key::Backspace and !mInputValue.empty()){
                    mInputValue.pop_back();
                }
                        
                else if (keyEvent->code == sf::Keyboard::Key::Enter and !mInputValue.empty()){
                    int val = stoi(mInputValue);
                        if (mIsInsertExpanded){
                            mTree.backup();
                            mTree.resetHistory("Create new node " + to_string(val) + " (RED)");
                            mTree.insert(val);
                        }
                    
                        else if (mIsDeleteExpanded){
                            mTree.backup();
                            mTree.resetHistory("Start to Remove " + to_string(val));
                            mTree.remove(val);
                        }
                            
                        else if (mIsSearchExpanded){
                            mTree.resetHistory("Start to Search for " + to_string(val));
                            mTree.search(val);
                        }
                            
                    mTree.setCurrentStep(0);
                    mIsInsertExpanded = false;
                    mIsDeleteExpanded = false;
                    mIsSearchExpanded = false;
                    mInputValue = "";
                    mIsPaused = false;
                        
                    mSkipBackBtn.setup(mSkipBackTex, 333.f, 897.f, 48.f, 48.f);
                    mSkipForwardBtn.setup(mSkipForwardTex, 461.f, 897.f, 48.f, 48.f);
                        
                    mAutoPlayClock.restart();
                }
                
                else if (keyEvent->code >= sf::Keyboard::Key::Num0 and keyEvent->code <= sf::Keyboard::Key::Num9){
                    if (mInputValue.length() < 3){
                        int digit = static_cast<int>(keyEvent->code) - static_cast<int>(sf::Keyboard::Key::Num0);
                        mInputValue += to_string(digit);
                    }
                }
                
                else if (keyEvent->code >= sf::Keyboard::Key::Numpad0 and keyEvent->code <= sf::Keyboard::Key::Numpad9){
                    if (mInputValue.length() < 3){
                        int digit = static_cast<int>(keyEvent->code) - static_cast<int>(sf::Keyboard::Key::Numpad0);
                        mInputValue += to_string(digit);
                    }
                }
            }
        }
        
        if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()){
            if (keyEvent->code == sf::Keyboard::Key::Z and (keyEvent->system or keyEvent->control)){
                mTree.restore();
                mDescriptionText.setString("Undo successfully.");
                mShowUndoBtn = false;
                mIsPaused = true;
                        
                mIsInsertExpanded = false;
                mIsDeleteExpanded = false;
                mIsSearchExpanded = false;
                mInputValue = "";
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
    float menuScale = (nodeCount > 7) ? (7.0f / (float)nodeCount) : 1.0f;
    float visScale = (nodeCount > 8) ? (8.0f / (float)nodeCount) : 1.0f;
    float currentScale = menuScale + (visScale - menuScale) * easeP1;
    
    const float R = (20.f + (35.f - 20.f) * easeP1) * currentScale;
    float lineThickness = (2.f + (4.f - 2.f) * easeP1) * currentScale;
    float mainStroke = (2.f + (3.f - 2.f) * easeP1) * currentScale;
    int fontSize = static_cast<int>((14 + (25 - 14) * easeP1) * currentScale);
    
    sf::Color edgeColor(89, 149, 43);

    map<int, sf::Vector2f> currentPos;
    for (const auto& n : stepB.nodes){
        sf::Vector2f endPos = posB[n.val];
        sf::Vector2f startPos;
        
        if (posA.count(n.val)){
            startPos = posA[n.val];
        } else{
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
            mWindow -> draw(highlightCirc);
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
            circle.setFillColor(sf::Color(139, 139, 139));
            circle.setOutlineColor(sf::Color(0, 0, 0));
            textColor = sf::Color(0, 0, 0);
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
                
            if (!mIsPaused and cur < total - 1){
                mTargetStep = cur + 1;
                mStepAnimProgress = 0.0f;
            } else {
                mTargetStep = -1;
            }
        }
    }
        
    else if (!mIsPaused and showUI and cur < total - 1 and mTargetStep == -1){
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
        
        if (cur >= 0 and cur < static_cast<int>(history.size())){
            mDescriptionText.setString(history[cur].description);
        }
        
        mWindow->draw(mFooter);
        mWindow->draw(mDescriptionBox);
        mWindow->draw(mDescriptionText);
        mWindow->draw(mHeaderText);
        mWindow->draw(mTitleNum);
        mWindow->draw(mTitleRed);
        mWindow->draw(mTitleBlack);
        mWindow->draw(mTitleTree);

        if (mShowUndoBtn){
            mUndoBtn.draw(*mWindow);
        } else {
            mClearBtn.draw(*mWindow);
        }
        
        mNewBtn.draw(*mWindow);
        sf::Vector2f worldPos = mWindow->mapPixelToCoords(sf::Mouse::getPosition(*mWindow));
                
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
                float cursorX = mInputValue.empty() ? 85.f : 85.f + textWidth + 2.f;
                mInsertCursorLine.setPosition(sf::Vector2f(cursorX, 240.f));
                mWindow->draw(mInsertCursorLine);
            }
            
        } else {
            if (sf::FloatRect(sf::Vector2f(70.f, 230.f), sf::Vector2f(160.f, 45.f)).contains(worldPos)){
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
                mShowCursor = !mShowCursor; mCursorClock.restart();
            }
                
            if (mShowCursor){
                float textW = mDeleteInputText.getLocalBounds().size.x;
                mDeleteCursorLine.setPosition(sf::Vector2f(mInputValue.empty() ? 85.f : 85.f + textW + 2.f, 294.f));
                mWindow->draw(mDeleteCursorLine);
            }
        } else {
            if (sf::FloatRect(sf::Vector2f(70.f, 284.f), sf::Vector2f(160.f, 45.f)).contains(worldPos)){
                mDeleteHoverStroke.draw(*mWindow);
                mDeleteBtn.draw(*mWindow);
            }
        }

        if (mIsSearchExpanded){
            mSearchExpandedStroke.draw(*mWindow);
            mSearchExpandedBg.draw(*mWindow);
            mConfirmSearchBtn.draw(*mWindow);
            mSearchDiceBtn.draw(*mWindow);
                    
            mSearchInputText.setString(mInputValue);
            mWindow->draw(mSearchInputText);
                    
            if (mCursorClock.getElapsedTime().asSeconds() >= 0.5f){
                mShowCursor = !mShowCursor; mCursorClock.restart();
            }
            
            if (mShowCursor){
                float textW = mSearchInputText.getLocalBounds().size.x;
                mSearchCursorLine.setPosition(sf::Vector2f(mInputValue.empty() ? 85.f : 85.f + textW + 2.f, 348.f));
                mWindow->draw(mSearchCursorLine);
            }
        } else {
            if (sf::FloatRect(sf::Vector2f(70.f, 338.f), sf::Vector2f(160.f, 45.f)).contains(worldPos)){
                mSearchHoverStroke.draw(*mWindow);
                mSearchBtn.draw(*mWindow);
            }
        }
        
        mSpeedSlider.draw(*mWindow);
        
        mHomeBtn.draw(*mWindow);
        mSkipBackBtn.draw(*mWindow);
        mSkipForwardBtn.draw(*mWindow);
        
        if (mIsPaused){
            mWindow->draw(mStepBgLeft);
            mWindow->draw(mStepBgRight);
            mWindow->draw(mStepBgCenter);
            
            int total = static_cast<int>(history.size());
            if (total == 0){
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

std::map<int, sf::Vector2f> RBTVisualizer::computeLayout(const std::vector<NodeSnapshot>& nodes){
    if (nodes.empty()){return {};}

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
    float menuScale = (nodeCount > 7) ? (7.0f / (float)nodeCount) : 1.0f;
    float visScale = (nodeCount > 8) ? (8.0f / (float)nodeCount) : 1.0f;
    float currentScale = menuScale + (visScale - menuScale) * easeP;
    float xSpacing = (40.f + (70.f - 40.f) * easeP) * currentScale;
    float ySpacing = (60.f + (100.f - 60.f) * easeP) * currentScale;
    
    float menuX = 780.f;
    float menuY = 300.f;
    
    float visX = 470.f;
    float visY = 200.f;
    
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
