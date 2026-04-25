#include "RedBlackTree/RBTVisualizer.h"
#include "ThemeManager.h"
#include "../config/Common.h"

#include <windows.h>

#include <iostream>
#include <string>
#include <random>
#include <map>
#include <set>
#include <functional>
#include <cmath>
#include <optional>

using namespace std;

RBTVisualizer::RBTVisualizer(sf::RenderWindow& window)
    : mWindow(&window),
      mTitleNum(mFontBold),
      mTitleRed(mFontBold), mTitleBlack(mFontBold), mTitleTree(mFontBold),

      mClearBtn(mFontRegular, "Clear tree", 70.f, 176.f, 160.f, 45.f, 21.f, sf::Color(217, 217, 217)),
      mNewBtn(mFontRegular, "New tree", 251.f, 176.f, 160.f, 45.f, 21.f, sf::Color(217, 217, 217)),
      mInsertBtn(mFontRegular, "Insert", 70.f, 230.f, 160.f, 45.f, 21.f, sf::Color(217, 217, 217)),
      mDeleteBtn(mFontRegular, "Delete", 70.f, 284.f, 160.f, 45.f, 21.f, sf::Color(217, 217, 217)),
      mSearchBtn(mFontRegular, "Search", 70.f, 338.f, 160.f, 45.f, 21.f, sf::Color(217, 217, 217)),
      mUndoBtn(mFontRegular, "Undo", 70.f, 176.f, 160.f, 45.f, 21.f, sf::Color(217, 217, 217)),
      
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
      mSearchInputText(mFontRegular),

      mNewHoverStroke(mFontRegular, "", 249.f, 174.f, 164.f, 49.f, 23.f, sf::Color(90, 150, 44)),
      mNewExpandedStroke(mFontRegular, "", 249.f, 174.f, 164.f, 157.f, 23.f, sf::Color(90, 150, 44)),
      mNewExpandedBg(mFontRegular, "", 251.f, 176.f, 160.f, 153.f, 21.f, sf::Color(217, 217, 217)),
      mRandomBtn(mFontRegular, "Random", 254.f, 233.f, 154.f, 39.f, 19.5f, sf::Color(245, 245, 240)),
      mUploadBtn(mFontRegular, "From File", 254.f, 287.f, 154.f, 39.f, 19.5f, sf::Color(245, 245, 240))
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

    if (!mDiceTex.loadFromFile("assets/images/randomButton.png")){cerr << "Loi load randomButton.png" << endl;}
    mDiceTex.setSmooth(true); 
    (void)mDiceTex.generateMipmap();
    
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
    
    if (mShowUndoBtn){
        mUndoBtn.update(mousePos);
    } else {
        mClearBtn.update(mousePos);
    }
    
    mNewBtn.update(mousePos);
    mInsertBtn.update(mousePos);
    mDeleteBtn.update(mousePos);
    mSearchBtn.update(mousePos);
        
    float speedMult = mLayout.getSpeed();
    if (speedMult > 0.0f){
        int speedMs = static_cast<int>(500.f / speedMult);
        mTree.setVisualizationSpeed(speedMs);
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
                
                if (sf::FloatRect(sf::Vector2f(70.f, 176.f), sf::Vector2f(160.f, 45.f)).contains(worldPos)){
                    clickedInsideAnyForm = true;
                    if (mShowUndoBtn){
                        mTree.restore();
                        mLayout.setDescription("Undo successfully.");
                        mShowUndoBtn = false;
                        mLayout.setPaused(true);
                    } else {
                        mTree.backup();
                        mTree.initialize();
                        mShowUndoBtn = true;
                    }
                    mIsNewExpanded = false; mIsInsertExpanded = false; mIsDeleteExpanded = false; mIsSearchExpanded = false;
                }
                else if (!mIsNewExpanded and sf::FloatRect(sf::Vector2f(251.f, 176.f), sf::Vector2f(160.f, 45.f)).contains(worldPos)){
                    mIsNewExpanded = true;
                    mIsInsertExpanded = false; mIsDeleteExpanded = false; mIsSearchExpanded = false;
                    clickedInsideAnyForm = true;
                }
                
                else if (mIsNewExpanded and sf::FloatRect(sf::Vector2f(251.f, 176.f), sf::Vector2f(160.f, 161.f)).contains(worldPos)){
                    clickedInsideAnyForm = true;
                    if (sf::FloatRect(sf::Vector2f(254.f, 233.f), sf::Vector2f(154.f, 39.f)).contains(worldPos)){
                        mTree.backup();
                        generateRandomTree();
                        mIsNewExpanded = false;
                        mShowUndoBtn = true;
                    }
                    
                    else if (sf::FloatRect(sf::Vector2f(254.f, 287.f), sf::Vector2f(154.f, 39.f)).contains(worldPos)){
                        mTree.backup();
                        
                        char filename[MAX_PATH] = "";
                        OPENFILENAMEA ofn;
                        ZeroMemory(&ofn, sizeof(ofn));
                        ofn.lStructSize = sizeof(ofn);
                        ofn.hwndOwner = nullptr;
                        ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
                        ofn.lpstrFile = filename;
                        ofn.nMaxFile = MAX_PATH;
                        ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
                        ofn.lpstrDefExt = "txt";

                        if (GetOpenFileNameA(&ofn)){
                            mTree.initializeFromFile(filename);
                            mTree.resetHistory("Tree loaded from file");
                            mLayout.setDescription("Tree loaded from file!");
                            mShowUndoBtn = true;
                        } else {
                            mLayout.setDescription("Cancel load file.");
                        }
                        mIsNewExpanded = false;
                    }
                }

                else if (!mIsInsertExpanded and sf::FloatRect(sf::Vector2f(70.f, 230.f), sf::Vector2f(160.f, 45.f)).contains(worldPos)){
                    mIsNewExpanded = false;
                    mIsInsertExpanded = true; mIsDeleteExpanded = false; mIsSearchExpanded = false;
                    mInputValue = "";
                    clickedInsideAnyForm = true;
                }
                
                else if (mIsInsertExpanded and sf::FloatRect(sf::Vector2f(70.f, 230.f), sf::Vector2f(341.f, 45.f)).contains(worldPos)){
                    clickedInsideAnyForm = true;
                    if (sf::FloatRect(sf::Vector2f(254.f, 233.f), sf::Vector2f(154.f, 39.f)).contains(worldPos) and !mInputValue.empty()){
                        int val = stoi(mInputValue);
                        mTree.backup();
                        mTree.resetHistory("Create new node " + to_string(val) + " (RED)");
                        mTree.insert(val);
                        mTree.setCurrentStep(0);
                        mIsInsertExpanded = false;
                        mInputValue = "";
                        mShowUndoBtn = true;
                        resetPlayUI();
                    }
                    
                    else if (sf::FloatRect(sf::Vector2f(205.f, 237.f), sf::Vector2f(30.f, 30.f)).contains(worldPos)){
                        mInputValue = to_string(rand() % 99 + 1);
                    }
                }

                else if (!mIsDeleteExpanded and sf::FloatRect(sf::Vector2f(70.f, 284.f), sf::Vector2f(160.f, 45.f)).contains(worldPos)){
                    mIsDeleteExpanded = true; mIsNewExpanded = false; mIsInsertExpanded = false; mIsSearchExpanded = false;
                    mInputValue = "";
                    clickedInsideAnyForm = true;
                }
                
                else if (mIsDeleteExpanded and sf::FloatRect(sf::Vector2f(70.f, 284.f), sf::Vector2f(341.f, 45.f)).contains(worldPos)){
                    clickedInsideAnyForm = true;
                    if (sf::FloatRect(sf::Vector2f(254.f, 287.f), sf::Vector2f(154.f, 39.f)).contains(worldPos) and !mInputValue.empty()){
                        int val = stoi(mInputValue);
                        mTree.backup();
                        mTree.resetHistory("Start to Remove " + to_string(val));
                        mTree.remove(val);
                        mTree.setCurrentStep(0);
                        mIsDeleteExpanded = false;
                        mInputValue = "";
                        mShowUndoBtn = true;
                        resetPlayUI();
                    }
                    
                    else if (sf::FloatRect(sf::Vector2f(205.f, 291.f), sf::Vector2f(30.f, 30.f)).contains(worldPos)){
                        mInputValue = to_string(rand() % 99 + 1);
                    }
                }
                
                else if (!mIsSearchExpanded and sf::FloatRect(sf::Vector2f(70.f, 338.f), sf::Vector2f(160.f, 45.f)).contains(worldPos)){
                    mIsSearchExpanded = true; mIsNewExpanded = false; mIsInsertExpanded = false; mIsDeleteExpanded = false;
                    mInputValue = "";
                    clickedInsideAnyForm = true;
                }
                
                else if (mIsSearchExpanded and sf::FloatRect(sf::Vector2f(70.f, 338.f), sf::Vector2f(341.f, 45.f)).contains(worldPos)){
                    clickedInsideAnyForm = true;
                    if (sf::FloatRect(sf::Vector2f(254.f, 341.f), sf::Vector2f(154.f, 39.f)).contains(worldPos) and !mInputValue.empty()){
                        int val = stoi(mInputValue);
                        mTree.resetHistory("Start to Search for " + to_string(val));
                        mTree.search(val);
                        mTree.setCurrentStep(0);
                        mIsSearchExpanded = false;
                        mInputValue = "";
                        resetPlayUI();
                    }
                    
                    else if (sf::FloatRect(sf::Vector2f(205.f, 345.f), sf::Vector2f(30.f, 30.f)).contains(worldPos)){
                        mInputValue = to_string(rand() % 99 + 1);
                    }
                }

                if (!clickedInsideAnyForm){
                    mIsInsertExpanded = false; mIsDeleteExpanded = false; mIsSearchExpanded = false; mIsNewExpanded = false;
                }
            }
        }
         
        else if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()){
            if (keyEvent->code == sf::Keyboard::Key::Z and (keyEvent->system or keyEvent->control)){
                mTree.restore();
                mLayout.setDescription("Undo successfully.");
                mShowUndoBtn = false;
                mLayout.setPaused(true);
                
                mIsNewExpanded = false;
                mIsInsertExpanded = false; mIsDeleteExpanded = false; mIsSearchExpanded = false;
                mInputValue = "";
            }
                    
            else if (keyEvent->code == sf::Keyboard::Key::I){
                mIsNewExpanded = false;
                mIsInsertExpanded = true; mIsDeleteExpanded = false; mIsSearchExpanded = false;
                mInputValue = "";
            }
            
            else if (keyEvent->code == sf::Keyboard::Key::D){
                mIsNewExpanded = false;
                mIsInsertExpanded = false; mIsDeleteExpanded = true; mIsSearchExpanded = false;
                mInputValue = "";
            }
            
            else if (keyEvent->code == sf::Keyboard::Key::S){
                mIsNewExpanded = false;
                mIsInsertExpanded = false; mIsDeleteExpanded = false; mIsSearchExpanded = true;
                mInputValue = "";
            }
            
            else if (keyEvent->code == sf::Keyboard::Key::N){
                mIsNewExpanded = !mIsNewExpanded;
                mIsInsertExpanded = false; mIsDeleteExpanded = false; mIsSearchExpanded = false;
                mInputValue = "";
            }
            
            else if (keyEvent->code == sf::Keyboard::Key::Space){
                mLayout.setPaused(!mLayout.isPaused());
            }
                    
            else if (keyEvent->code == sf::Keyboard::Key::R){
                if (mIsInsertExpanded or mIsDeleteExpanded or mIsSearchExpanded){
                    mInputValue = to_string(rand() % 99 + 1);
                } else {
                    mTree.backup();
                    generateRandomTree();
                    mShowUndoBtn = true;
                    mIsNewExpanded = false;
                }
            }
                    
            else if (mIsInsertExpanded or mIsDeleteExpanded or mIsSearchExpanded){
                if (keyEvent->code == sf::Keyboard::Key::Backspace and !mInputValue.empty()){
                    mInputValue.pop_back();
                }
                
                else if (keyEvent->code == sf::Keyboard::Key::Enter and !mInputValue.empty()){
                    int val = stoi(mInputValue);
                            
                    if (mIsInsertExpanded){
                        mTree.backup();
                        mTree.resetHistory("Create new node " + to_string(val) + " (RED)");
                        mTree.insert(val);
                        mShowUndoBtn = true;
                    }
                    
                    else if (mIsDeleteExpanded){
                        mTree.backup();
                        mTree.resetHistory("Start to Remove " + to_string(val));
                        mTree.remove(val);
                        mShowUndoBtn = true;
                    }
                    
                    else if (mIsSearchExpanded){
                        mTree.resetHistory("Start to Search for " + to_string(val));
                        mTree.search(val);
                    }
                                
                    mTree.setCurrentStep(0);
                    mIsInsertExpanded = false; mIsDeleteExpanded = false; mIsSearchExpanded = false;
                    mInputValue = "";
                    resetPlayUI();
                }
                
                else {
                    int digit = -1;
                    
                    int codeInt = static_cast<int>(keyEvent->code);
                    int num0Int = static_cast<int>(sf::Keyboard::Key::Num0);
                    int num9Int = static_cast<int>(sf::Keyboard::Key::Num9);
                    int numpad0Int = static_cast<int>(sf::Keyboard::Key::Numpad0);
                    int numpad9Int = static_cast<int>(sf::Keyboard::Key::Numpad9);
                    
                    if (codeInt >= num0Int and codeInt <= num9Int){
                        digit = codeInt - num0Int;
                    }
                    else if (codeInt >= numpad0Int and codeInt <= numpad9Int){
                        digit = codeInt - numpad0Int;
                    }
                    
                    if (digit != -1 and mInputValue.length() < 3){
                        mInputValue += to_string(digit);
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
    float menuScale = (nodeCount > 7) ? (7.0f / (float)nodeCount) : 1.0f;
    float visScale = (nodeCount > 8) ? (8.0f / (float)nodeCount) : 1.0f;
    float currentScale = menuScale + (visScale - menuScale) * easeP1;
    
    const float R = (20.f + (35.f - 20.f) * easeP1) * currentScale;
    float lineThickness = (2.f + (4.f - 2.f) * easeP1) * currentScale;
    float mainStroke = (2.f + (3.f - 2.f) * easeP1) * currentScale;
    int fontSize = static_cast<int>((14 + (30 - 14) * easeP1) * currentScale);
    
    sf::Color edgeColor = ThemeManager::current.primary;
    
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
            circle.setOutlineColor(sf::Color(67, 66, 67));
            textColor = sf::Color(67, 66, 67);
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
        }
            
        total = static_cast<int>(mTree.getStepHistory().size());
        int current = mTree.getCurrentStep();
        mLayout.setStepText("Step " + std::to_string(current + 1) + " / " + std::to_string(total));
        mLayout.draw(*mWindow);
            
        mWindow->draw(mTitleNum);
        mWindow->draw(mTitleRed);
        mWindow->draw(mTitleBlack);
        mWindow->draw(mTitleTree);

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
            if (sf::FloatRect(sf::Vector2f(251.f, 176.f), sf::Vector2f(160.f, 45.f)).contains(worldPos)){
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
                mShowCursor = !mShowCursor;
                mCursorClock.restart();
            }
            
            if (mShowCursor){
                float textWidth = mDeleteInputText.getLocalBounds().size.x;
                float cursorX = mInputValue.empty() ? 85.f : 85.f + textWidth + 2.f;
                mDeleteCursorLine.setPosition(sf::Vector2f(cursorX, 294.f));
                mWindow->draw(mDeleteCursorLine);
            }
        } else {
            if (sf::FloatRect(sf::Vector2f(70.f, 284.f), sf::Vector2f(160.f, 45.f)).contains(worldPos)){
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
                float cursorX = mInputValue.empty() ? 85.f : 85.f + textWidth + 2.f;
                mSearchCursorLine.setPosition(sf::Vector2f(cursorX, 348.f));
                mWindow->draw(mSearchCursorLine);
            }
        } else {
            if (sf::FloatRect(sf::Vector2f(70.f, 338.f), sf::Vector2f(160.f, 45.f)).contains(worldPos)){
                mSearchHoverStroke.draw(*mWindow);
            }
            mSearchBtn.draw(*mWindow);
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
    float menuScale = (nodeCount > 7) ? (7.0f / (float)nodeCount) : 1.0f;
    float visScale = (nodeCount > 8) ? (8.0f / (float)nodeCount) : 1.0f;
    float currentScale = menuScale + (visScale - menuScale) * easeP;
    float xSpacing = (40.f + (70.f - 40.f) * easeP) * currentScale;
    float ySpacing = (60.f + (100.f - 60.f) * easeP) * currentScale;
    
    float totalWidth = (nodeCount - 1) * xSpacing;
    float menuCenterX = 900.f;
    float visCenterX = 700.f;
        
    float menuX = menuCenterX - totalWidth / 2.0f;
    float visX = visCenterX - totalWidth / 2.0f;
        
    float menuY = 300.f;
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

void RBTVisualizer::resetPlayUI(){
    mLayout.setPaused(false);
    
    mTree.setCurrentStep(0);
    mTargetStep = -1;
    mStepAnimProgress = 1.0f;
    
    mShowUndoBtn = false;
    mAutoPlayClock.restart();
}
