#include "HashChaining/HashVisualizer.h"
#include "tinyfiledialogs.h"
#include "Common.h"
#include <cmath>
#include <cstdlib>
#include <cstdint>

HashVisualizer::HashColorTheme HashVisualizer::getHashTheme() {
    HashColorTheme t;
    
    if (!ThemeManager::isDark) {
        // ==========================================
        // ☀️ LIGHT MODE TEMPLATES
        // ==========================================
        if (ThemeManager::currentType == ThemeType::DEFAULT) {
            t.bucketFill            = sf::Color(160, 200, 160); // Xanh lá nhạt
            t.bucketBorder          = sf::Color(120, 165, 120); // Xanh lá đậm
            t.nodeFill              = sf::Color(205, 198, 222); // Tím Lavender nhạt
            t.nodeBorder            = sf::Color(180, 165, 205); // Tím đậm hơn chút
            t.arrow                 = sf::Color(150, 140, 170); // Mũi tên tím xám
            
            // Highlight (vàng)
            t.bucketHighlightFill   = sf::Color(245, 205, 95);
            t.bucketHighlightBorder = sf::Color(215, 160, 60);
            t.nodeHighlightFill     = sf::Color(155, 178, 192); // xanh
            t.nodeHighlightBorder   = sf::Color(106, 139, 161);
            t.highlightText         = sf::Color::White;
        }
        else if (ThemeManager::currentType == ThemeType::PINKY) {
            t.bucketFill            = sf::Color(243, 204, 151); // #f3cc97
            t.bucketBorder          = sf::Color(220, 150, 70);  // cam vàng đậm hơn
            t.nodeFill              = sf::Color(251, 217, 229); // #fbd9e5
            t.nodeBorder            = sf::Color(242, 131, 175); // #f283af
            t.arrow                 = sf::Color(180, 150, 160); // trung tính hơi ấm

            // Highlight (vàng cam)
            t.bucketHighlightFill   = sf::Color(249, 203, 214);
            t.bucketHighlightBorder = sf::Color(226, 139, 145);
            t.nodeHighlightFill     = sf::Color(255, 210, 130);
            t.nodeHighlightBorder   = sf::Color(230, 160, 90);
            t.highlightText         = sf::Color(230, 160, 90);
        }
        else if (ThemeManager::currentType == ThemeType::NAVY) {
            t.bucketFill            = sf::Color(148, 194, 218); // #94c2da
            t.bucketBorder          = sf::Color(78, 124, 178);  // #4e7cb2
            t.nodeFill              = sf::Color(239, 232, 224); // #efe8e0
            t.nodeBorder            = sf::Color(130, 170, 210); // xanh dương nhạt hơn
            t.arrow                 = sf::Color(140, 150, 170); // xám xanh

            // Highlight (vàng + hồng)
            t.bucketHighlightFill   = sf::Color(255, 210, 110);
            t.bucketHighlightBorder = sf::Color(220, 160, 70);
            t.nodeHighlightFill     = sf::Color(249, 203, 214);
            t.nodeHighlightBorder   = sf::Color(226, 139, 145);
            t.highlightText         = sf::Color(226, 139, 145);
        }
        else if (ThemeManager::currentType == ThemeType::GOLDEN) {
            t.bucketFill            = sf::Color(252, 200, 138); // #fcc88a
            t.bucketBorder          = sf::Color(200, 150, 70);  // vàng golden sẫm
            t.nodeFill              = sf::Color(241, 195, 179); // #f1c3b3
            t.nodeBorder            = sf::Color(231, 152, 151); // #e79897
            t.arrow                 = sf::Color(170, 140, 120); // nâu xám ấm

            // Highlight
            t.bucketHighlightFill   = sf::Color(239, 195, 194);
            t.bucketHighlightBorder = sf::Color(210, 168, 170);
            t.nodeHighlightFill     = sf::Color(255, 225, 150);
            t.nodeHighlightBorder   = sf::Color(230, 180, 110);
            t.highlightText         = sf::Color(230, 180, 110);
        }
    } else {
        // ==========================================
        // 🌙 DARK MODE TEMPLATES
        // ==========================================
        if (ThemeManager::currentType == ThemeType::DEFAULT) {
            t.bucketFill            = sf::Color(110, 145, 110);
            t.bucketBorder          = sf::Color(85, 120, 85);
            t.nodeFill              = sf::Color(150, 145, 175);
            t.nodeBorder            = sf::Color(125, 110, 160);
            t.arrow                 = sf::Color(120, 115, 140);
            
            // Highlight
            t.bucketHighlightFill   = sf::Color(220, 180, 80);
            t.bucketHighlightBorder = sf::Color(190, 140, 55);
            t.nodeHighlightFill     = sf::Color(120, 145, 160); // xanh trầm lại
            t.nodeHighlightBorder   = sf::Color(85, 110, 130);
            t.highlightText         = sf::Color::White;
        }
        else if (ThemeManager::currentType == ThemeType::PINKY) {
            t.bucketFill            = sf::Color(180, 150, 110);
            t.bucketBorder          = sf::Color(150, 105, 55);
            t.nodeFill              = sf::Color(190, 155, 170);
            t.nodeBorder            = sf::Color(180, 95, 130);
            t.arrow                 = sf::Color(140, 115, 120);

            // Highlight
            t.bucketHighlightFill   = sf::Color(210, 170, 180); // hồng trầm lại
            t.bucketHighlightBorder = sf::Color(185, 115, 120);
            t.nodeHighlightFill     = sf::Color(235, 185, 110);
            t.nodeHighlightBorder   = sf::Color(200, 145, 80);
            t.highlightText         = sf::Color::White;
        }
        else if (ThemeManager::currentType == ThemeType::NAVY) {
            t.bucketFill            = sf::Color(95, 130, 150);
            t.bucketBorder          = sf::Color(60, 90, 130);
            t.nodeFill              = sf::Color(170, 165, 155);
            t.nodeBorder            = sf::Color(100, 130, 165);
            t.arrow                 = sf::Color(110, 120, 140);

            // Highlight
            t.bucketHighlightFill   = sf::Color(235, 190, 95);
            t.bucketHighlightBorder = sf::Color(190, 140, 60);
            t.nodeHighlightFill     = sf::Color(210, 170, 180); // hồng trầm
            t.nodeHighlightBorder   = sf::Color(185, 115, 120);
            t.highlightText         = sf::Color::White;
        }
        else if (ThemeManager::currentType == ThemeType::GOLDEN) {
            t.bucketFill            = sf::Color(185, 145, 100);
            t.bucketBorder          = sf::Color(145, 110, 55);
            t.nodeFill              = sf::Color(175, 140, 125);
            t.nodeBorder            = sf::Color(165, 110, 110);
            t.arrow                 = sf::Color(135, 110, 95);

            // Highlight
            t.bucketHighlightFill   = sf::Color(200, 165, 165); // hồng beige trầm
            t.bucketHighlightBorder = sf::Color(175, 140, 145);
            t.nodeHighlightFill     = sf::Color(240, 200, 120);
            t.nodeHighlightBorder   = sf::Color(205, 160, 85);
            t.highlightText         = sf::Color::White;
        }
    }
    return t;
}

// ── Pseudocode tables ─────────────────────────────────────────
static const std::vector<std::string> PSEUDO_INSERT = {
    "Insert(key):",
    "  1. idx = key % tableSize",
    "  2. Go to bucket[idx]",
    "  3. Create new node(key)",
    "  4. node.next = bucket[idx]",
    "  5. bucket[idx] = node",
    "  6. Done.",
};

static const std::vector<std::string> PSEUDO_SEARCH = {
    "Search(key):",
    "  1. idx = key % tableSize",
    "  2. Go to cur = bucket[idx]",
    "  3. while cur != null:",
    "  4.   if cur.val == key -> Found",
    "  5.   cur = cur.next",
    "  6. Not found.",
};

static const std::vector<std::string> PSEUDO_DELETE = {
    "Delete(key):",
    "  1. idx = hash(key)",
    "  2. if bucket[idx] == null -> Not found",
    "  3. if head.val == key:",
    "  4.   bucket[idx] = head.next -> Done",
    "  5. while cur.next != null:",
    "  6.   if cur.next.val == key:",
    "  7.     cur.next = cur.next.next -> Done",
    "  8.   cur = cur.next",
    "  9. Not found.",
};

static std::pair<int, std::vector<int>> lidToPseudo(int lid){
    switch(lid){
        // INSERT (0-3)
        case 0: return {0, {1}};
        case 1: return {0, {2}};
        case 2: return {0, {3, 4, 5}}; // Create & link
        case 3: return {0, {6}};
        
        // SEARCH (4-8, 17)
        case 4: return {1, {1}};
        case 5: return {1, {2}};
        case 6: return {1, {3}};
        case 7: return {1, {4}};
        case 17: return {1, {5}};
        case 8: return {1, {6}};
        
        // DELETE
        case 9: case 10: return {2, {1}};
        case 11: return {2, {2}};
        case 12: return {2, {3}};
        case 13: return {2, {3, 4}};       // Delete & Done
        case 14: return {2, {5}};
        case 16: return {2, {6}};
        case 19: return {2, {6, 7}};       // Delete mid & done
        case 18: return {2, {8}};
        case 15: return {2, {9}};
        
        default: return {-1, {}}; // Highlight none
    }
}

// ============================================================
//  Constructor
// ============================================================
HashVisualizer::HashVisualizer(sf::RenderWindow& window)
    : mWindow(&window),
      mHash(7),

      mTitleNum  (mFontBold),
      mTitleHash (mFontBold),
      mTitleChain(mFontBold),

      mClearBtn (mFontRegular, "Clear Table", 32.f, 150.f, 160.f, 45.f, 22.5f, ThemeManager::current.secondary),
      mNewBtn   (mFontRegular, "New Table", 213.f, 150.f, 160.f, 45.f, 22.5f, ThemeManager::current.secondary),
      mInsertBtn(mFontRegular, "Insert", 32.f, 204.f, 160.f, 45.f, 22.5f, ThemeManager::current.secondary),
      mDeleteBtn(mFontRegular, "Delete", 32.f, 258.f, 160.f, 45.f, 22.5f, ThemeManager::current.secondary),
      mSearchBtn(mFontRegular, "Search", 32.f, 312.f, 160.f, 45.f, 22.5f, ThemeManager::current.secondary),
      mUndoBtn  (mFontRegular, "Undo", 32.f, 150.f, 160.f, 45.f, 22.5f, ThemeManager::current.secondary),
      mUpdateBtn(mFontRegular, "Update", 32.f, 366.f, 160.f, 45.f, 22.5f, ThemeManager::current.secondary),

      // Insert expand
      mInsertHoverStroke   (mFontRegular,"", 30.f, 202.f, 164.f, 49.f, 24.5f, ThemeManager::current.primary),
      mInsertExpandedStroke(mFontRegular,"", 30.f, 202.f, 345.f, 49.f, 24.5f, ThemeManager::current.primary),
      mInsertExpandedBg    (mFontRegular,"", 32.f, 204.f, 341.f, 45.f, 22.5f, ThemeManager::current.secondary),
      mConfirmAddBtn(mFontRegular, "Add", 216.f, 207.f, 154.f, 39.f, 19.5f, ThemeManager::current.bg),
      mInsertInputText     (mFontRegular),

      // Delete expand
      mDeleteHoverStroke   (mFontRegular,"", 30.f, 256.f, 164.f, 49.f, 24.5f, ThemeManager::current.primary),
      mDeleteExpandedStroke(mFontRegular,"", 30.f, 256.f, 345.f, 49.f, 24.5f, ThemeManager::current.primary),
      mDeleteExpandedBg    (mFontRegular,"", 32.f, 258.f, 341.f, 45.f, 22.5f, ThemeManager::current.secondary),
      mConfirmRemoveBtn(mFontRegular, "Remove", 216.f, 261.f, 154.f, 39.f, 19.5f, ThemeManager::current.bg),
      mDeleteInputText     (mFontRegular),

      // Search expand
      mSearchHoverStroke   (mFontRegular,"", 30.f, 310.f, 164.f, 49.f, 24.5f, ThemeManager::current.primary),
      mSearchExpandedStroke(mFontRegular,"", 30.f, 310.f, 345.f, 49.f, 24.5f, ThemeManager::current.primary),
      mSearchExpandedBg    (mFontRegular,"", 32.f, 312.f, 341.f, 45.f, 22.5f, ThemeManager::current.secondary),
      mConfirmSearchBtn(mFontRegular, "Search", 216.f, 315.f, 154.f, 39.f, 19.5f, ThemeManager::current.bg),
      mSearchInputText     (mFontRegular),

      // Update expand
      mUpdateHoverStroke   (mFontRegular,"", 30.f, 364.f, 164.f, 49.f, 24.5f, ThemeManager::current.primary),
      mUpdateExpandedStroke(mFontRegular,"", 30.f, 364.f, 345.f, 49.f, 24.5f, ThemeManager::current.primary),
      mUpdateExpandedBg    (mFontRegular,"", 32.f, 366.f, 341.f, 45.f, 22.5f, ThemeManager::current.secondary),
      mConfirmUpdateBtn(mFontRegular, "Update", 216.f, 369.f, 154.f, 39.f, 19.5f, ThemeManager::current.bg),
      mUpdateInputText     (mFontRegular),

      // New expand
      mNewHoverStroke(mFontRegular, "", 211.f, 148.f, 164.f, 49.f, 24.5f, ThemeManager::current.primary),
      mNewExpandedStroke(mFontRegular, "", 211.f, 148.f, 164.f, 157.f, 24.5f, ThemeManager::current.primary),
      mNewExpandedBg(mFontRegular, "", 213.f, 150.f, 160.f, 153.f, 22.5f, ThemeManager::current.secondary),
      mRandomBtn(mFontRegular, "Random", 216.f, 207.f, 154.f, 39.f, 19.5f, ThemeManager::current.bg),
      mUploadBtn(mFontRegular, "From File", 216.f, 261.f, 154.f, 39.f, 19.5f, ThemeManager::current.bg),

      mHideMenuBtn(mFontBold, "<", 382.f, 150.f, 45.f, 45.f, 22.5f, ThemeManager::current.secondary),
      mShowMenuBtn(mFontBold, ">", 32.f,  150.f, 45.f, 45.f, 22.5f, ThemeManager::current.secondary)
{
    if (!mFontBold.openFromFile("assets/fonts/Inter-Bold.ttf") ||
        !mFontRegular.openFromFile("assets/fonts/Inter-Regular.ttf") ||
        !mFontMedium.openFromFile("assets/fonts/Inter-Medium.otf"))
        cerr << "HashVisualizer: cannot load fonts" << endl;

    if (!mDiceTex.loadFromFile("assets/images/randomButton.png"))
        cerr << "Cannot load randomButton.png" << endl;
    mDiceTex.setSmooth(true); (void)mDiceTex.generateMipmap();

    // Dice buttons
    mInsertDiceBtn.setup(mDiceTex, 167.f, 211.f, 30.f, 30.f);
    mDeleteDiceBtn.setup(mDiceTex, 167.f, 265.f, 30.f, 30.f);
    mSearchDiceBtn.setup(mDiceTex, 167.f, 319.f, 30.f, 30.f);

    // Input texts
    auto setupInput = [&](sf::Text& t, float y){
        t.setCharacterSize(25);
        t.setFillColor(ThemeManager::current.textColor);
        t.setPosition(sf::Vector2f(47.f, y));
    };
    setupInput(mInsertInputText, 210.f);
    setupInput(mDeleteInputText, 264.f);
    setupInput(mSearchInputText, 318.f);

    // Cursor lines
    for (auto* c : {&mInsertCursorLine,&mDeleteCursorLine,&mSearchCursorLine}){
        c->setSize(sf::Vector2f(2.f, 25.f));
        c->setFillColor(ThemeManager::current.textColor);
    }

    // Title
    mTitleNum.setString("02");
    mTitleNum.setCharacterSize(30);
    mTitleNum.setFillColor(ThemeManager::current.textColor);
    mTitleNum.setPosition(sf::Vector2f(32.f, 663.f));

    mTitleHash.setString("Hash");
    mTitleHash.setCharacterSize(70);
    mTitleHash.setFillColor(ThemeManager::current.textColor);
    mTitleHash.setPosition(sf::Vector2f(32.f, 695.f));

    mTitleChain.setString("Table");
    mTitleChain.setCharacterSize(70);
    mTitleChain.setFillColor(ThemeManager::current.primary);
    mTitleChain.setPosition(sf::Vector2f(32.f, 765.f));

    for (auto* b : {&mClearBtn,&mNewBtn,&mInsertBtn,&mDeleteBtn,&mSearchBtn,&mUndoBtn, &mUpdateBtn,
                    &mConfirmAddBtn,&mConfirmRemoveBtn,&mConfirmSearchBtn, &mConfirmUpdateBtn,
                    &mRandomBtn,&mUploadBtn, &mHideMenuBtn, &mShowMenuBtn})
        b->refreshText();

    // ── Wire AppLayout callbacks ──────────────────────────────
    mLayout.setSkipBackCallback([this](){
        mCurrentStep = 0;
        mLayout.setPaused(true);
    });
    mLayout.setSkipForwardCallback([this](){
        int total = (int)mHash.getSteps().size();
        mCurrentStep = std::max(0, total - 1);
        mLayout.setPaused(true);
    });
    mLayout.setStepBackCallback([this](){
        if (mCurrentStep > 0){
            mTargetStep = mCurrentStep - 1;
            mStepAnimProgress = 0.f;
        }
        mLayout.setPaused(true);
    });
    mLayout.setStepForwardCallback([this](){
        int total = (int)mHash.getSteps().size();
        if (mCurrentStep < total - 1){
            mTargetStep = mCurrentStep + 1;
            mStepAnimProgress = 0.f;
        }
        mLayout.setPaused(true);
    });
    mLayout.setPlayPauseCallback([this](){
        if (!mLayout.isPaused()) mAutoPlayClock.restart();
    });

    // ── Sidebar callbacks ─────────────────────────────────────
    mClearBtn.setCallback([this](){
        mHash.clearTableUI(); // Gọi hàm mới tạo
        mCurrentStep = 0;
        mTargetStep  = -1;
        mLayout.setDescription("Hash table cleared.");
        mLayout.setPaused(true);
        mShowUndoBtn = false;
    });
    mUndoBtn.setCallback([this](){ mShowUndoBtn = false; });

    // ── Initial data ─────────────────────────────────────────
    doRandom();
}

// ── doRandom ─────────────────────────────────────────────────
void HashVisualizer::doRandom(){
    mHash.generateRandom(5 + rand() % 6);
    mCurrentStep = std::max(0, (int)mHash.getSteps().size()-1);
    mTargetStep  = -1;
    mLayout.setPaused(true);
    mLayout.setDescription("Random hash table generated.");
}

// ── runAction ────────────────────────────────────────────────
void HashVisualizer::runAction(int action, int value, int oldValue){
    if      (action==1){ mHash.add(value);              mLayout.setDescription("Added "+std::to_string(value)); }
    else if (action==2){ mHash.deleteNode(value);        mLayout.setDescription("Deleted "+std::to_string(value)); }
    else if (action==3){ mHash.search(value);            mLayout.setDescription("Searching "+std::to_string(value)); }
    else if (action==4){ mHash.update(oldValue,value);   mLayout.setDescription("Updated "+std::to_string(oldValue)+" -> "+std::to_string(value)); }
    mCurrentStep = 0;
    mTargetStep  = -1;
    mStepAnimProgress = 1.f;
    mLayout.setPaused(false);
    mAutoPlayClock.restart();
}

bool HashVisualizer::checkReturnHome(){
    if (mGoHome){ mGoHome=false; return true; }
    return false;
}

// ── drawRoundedRect ──────────────────────────────────────────
void HashVisualizer::drawRoundedRect(sf::RenderWindow& window, const sf::FloatRect& rect,
                                      float radius, const sf::Color& fill,
                                      const sf::Color& border, float borderThick){
    auto drawFill = [&](const sf::FloatRect& r, float rad, const sf::Color& col){
        float rw=r.size.x, rh=r.size.y, rx=r.position.x, ry=r.position.y;
        if (rw<=0||rh<=0) return;
        float cr=std::min(rad,std::min(rw,rh)*0.5f);
        sf::RectangleShape h(sf::Vector2f(rw-2*cr,rh)); h.setPosition({rx+cr,ry}); h.setFillColor(col); window.draw(h);
        sf::RectangleShape v(sf::Vector2f(rw,rh-2*cr)); v.setPosition({rx,ry+cr}); v.setFillColor(col); window.draw(v);
        sf::CircleShape c(cr); c.setFillColor(col);
        c.setPosition({rx,ry}); window.draw(c);
        c.setPosition({rx+rw-2*cr,ry}); window.draw(c);
        c.setPosition({rx,ry+rh-2*cr}); window.draw(c);
        c.setPosition({rx+rw-2*cr,ry+rh-2*cr}); window.draw(c);
    };
    drawFill(rect,radius,border);
    if (borderThick>0.f){
        sf::FloatRect inner(
            sf::Vector2f(rect.position.x+borderThick, rect.position.y+borderThick),
            sf::Vector2f(std::max(0.f,rect.size.x-borderThick*2), std::max(0.f,rect.size.y-borderThick*2)));
        drawFill(inner,std::max(0.f,radius-borderThick),fill);
    }
}

// ── drawArrowUp ──────────────────────────────────────────────
void HashVisualizer::drawArrowUp(sf::RenderWindow& window, sf::Vector2f from,
                                  sf::Vector2f to, const sf::Color& color){
    sf::Vector2f dir=to-from;
    float len=std::sqrt(dir.x*dir.x+dir.y*dir.y);
    if (len<1.f) return;
    dir/=len;
    sf::Vector2f perp(-dir.y,dir.x);
    float arrowSize=8.f,arrowWidth=8.f,thick=2.f;
    sf::RectangleShape line(sf::Vector2f(len-arrowSize,thick));
    line.setOrigin(sf::Vector2f(0.f,thick/2.f));
    line.setPosition(from);
    line.setFillColor(color);
    line.setRotation(sf::radians(std::atan2(dir.y,dir.x)));
    window.draw(line);
    sf::ConvexShape tri; tri.setPointCount(3);
    tri.setPoint(0,to);
    tri.setPoint(1,to-dir*arrowSize+perp*(arrowWidth/2.f));
    tri.setPoint(2,to-dir*arrowSize-perp*(arrowWidth/2.f));
    tri.setFillColor(color);
    window.draw(tri);
}

// ── renderBuckets ────────────────────────────────────────────
void HashVisualizer::renderBuckets(sf::RenderWindow& window){
    auto steps=mHash.getSteps();
    int targetBucket=-1;
    if (mCurrentStep>=0&&mCurrentStep<(int)steps.size())
        targetBucket=steps[mCurrentStep].targetBucket;

    int n=mHash.getSize();
    
    HashColorTheme theme = getHashTheme();

    for (int i=0;i<n;++i){
        float px=mBucketStartX+i*mBucketGapX, py=mBucketRowY;
        bool isTarget=(i==targetBucket);

        // BUCKET COLOR SYNC WITH THEME
        sf::Color fill   = isTarget ? theme.bucketHighlightFill : theme.bucketFill;
        sf::Color border = isTarget ? theme.bucketHighlightBorder : theme.bucketBorder;

        drawRoundedRect(window, sf::FloatRect(sf::Vector2f(px,py), sf::Vector2f(mBucketW,mBucketH)), 10.f, fill, border, 3.5f);

        sf::Text lbl(mFontMedium, "H" + std::to_string(i), 18);
        lbl.setFillColor(ThemeManager::current.textColor);

        sf::FloatRect lb=lbl.getLocalBounds();
        lbl.setPosition(sf::Vector2f(px+(mBucketW-lb.size.x)*0.5f-lb.position.x,
                                     py+(mBucketH-lb.size.y)*0.5f-lb.position.y));
        window.draw(lbl);

        sf::Text idx(mFontMedium, std::to_string(i), 11);
        sf::Color dimText = ThemeManager::current.textColor;
        dimText.a = 150; // Small text
        idx.setFillColor(dimText);
        sf::FloatRect ib=idx.getLocalBounds();
        idx.setPosition(sf::Vector2f(px+(mBucketW-ib.size.x)*0.5f-ib.position.x, py+mBucketH+4.f));
        window.draw(idx);
    }
}

// ── renderNodes (Fade In/Out & Theme Color) ────────────────
void HashVisualizer::renderNodes(sf::RenderWindow& window){
    auto steps = mHash.getSteps();
    if (steps.empty()) return;

    int si = std::max(0, std::min(mCurrentStep, (int)steps.size()-1));
    int ti = (mTargetStep != -1) ? mTargetStep : si;

    const auto& currentSnap = steps[si].tableSnapshot;
    const auto& targetSnap = steps[ti].tableSnapshot;
    if (currentSnap.empty()) return;

    int targetBucket = steps[si].targetBucket;
    int targetVal    = steps[si].targetVal;

    float nodeW = mBucketW, nodeH = mNodeH, gapY = mGapY;

    for (int i = 0; i < (int)currentSnap.size(); ++i){
        float bx = mBucketStartX + i * mBucketGapX, by = mBucketRowY;
        std::vector<int> allNodes = currentSnap[i].vals;
        for (int val : targetSnap[i].vals) {
            if (std::find(allNodes.begin(), allNodes.end(), val) == allNodes.end()) {
                allNodes.push_back(val);
            }
        }
        
        for (int depth = 0; depth < (int)allNodes.size(); ++depth){
            int nv = allNodes[depth];

            // LOGIC FADE IN / FADE OUT
            bool inCurrent = (std::find(currentSnap[i].vals.begin(), currentSnap[i].vals.end(), nv) != currentSnap[i].vals.end());
            bool inTarget = (std::find(targetSnap[i].vals.begin(), targetSnap[i].vals.end(), nv) != targetSnap[i].vals.end());

            float alphaMult = 1.0f;
            if (inCurrent && !inTarget) alphaMult = std::max(0.f, 1.0f - mStepAnimProgress * 3.0f); // Đang bị xóa -> Nhạt dần
            else if (!inCurrent && inTarget) alphaMult = std::min(1.0f, mStepAnimProgress * 3.0f);   // Đang được thêm -> Đậm dần

            if (alphaMult <= 0.01f) continue;
            std::uint8_t alpha = static_cast<std::uint8_t>(255 * alphaMult);

            int realDepth = depth;
            if (inTarget) {
                auto it = std::find(targetSnap[i].vals.begin(), targetSnap[i].vals.end(), nv);
                realDepth = std::distance(targetSnap[i].vals.begin(), it);
            } else if (inCurrent) {
                auto it = std::find(currentSnap[i].vals.begin(), currentSnap[i].vals.end(), nv);
                realDepth = std::distance(currentSnap[i].vals.begin(), it);
            }
            float px = bx, py = by - (realDepth + 1) * gapY;

            // CHANGE NODE COLOR TO THEME
            HashColorTheme theme = getHashTheme();
            bool isTarget = (nv == targetVal && i == targetBucket);
            
            sf::Color fillCol   = isTarget ? theme.nodeHighlightFill : theme.nodeFill;
            sf::Color borderCol = isTarget ? theme.nodeHighlightBorder : theme.nodeBorder;
            
            sf::Color arrowCol  = isTarget ? theme.nodeHighlightBorder : theme.arrow;
            sf::Color textCol   = isTarget ? theme.highlightText : ThemeManager::current.textColor;
    
            fillCol.a = alpha; textCol.a = alpha; borderCol.a = alpha;
            
            drawRoundedRect(window, sf::FloatRect(sf::Vector2f(px,py), sf::Vector2f(nodeW,nodeH)), 10.f, fillCol, borderCol, 3.f);
            
            sf::Text valText(mFontMedium, std::to_string(nv), 16);
            valText.setFillColor(textCol);
            sf::FloatRect vb = valText.getLocalBounds();
            valText.setPosition(sf::Vector2f(px + (nodeW - vb.size.x) * 0.5f - vb.position.x,
                                             py + (nodeH - vb.size.y) * 0.5f - vb.position.y));
            window.draw(valText);
            arrowCol.a = alpha;
        
            if (realDepth == 0) drawArrowUp(window, sf::Vector2f(bx + nodeW*0.5f, by), sf::Vector2f(px + nodeW*0.5f, py + nodeH + 1.f), arrowCol);
            else drawArrowUp(window, sf::Vector2f(px + nodeW*0.5f, by - realDepth*gapY), sf::Vector2f(px + nodeW*0.5f, py + nodeH + 1.f), arrowCol);
        }
    }
}

// ── update ───────────────────────────────────────────────────
void HashVisualizer::update(const std::optional<sf::Event>& event){
    sf::Vector2f worldPos=mWindow->mapPixelToCoords(sf::Mouse::getPosition(*mWindow));
    sf::Vector2i mousePos(static_cast<int>(worldPos.x),static_cast<int>(worldPos.y));

    mLayout.update(mousePos);
    if (mLayout.mGoHome){ mGoHome=true; mLayout.mGoHome=false; }

    if (mIsSidebarVisible) {
        if (mShowUndoBtn) mUndoBtn .update(mousePos);
        else              mClearBtn.update(mousePos);
    
        mNewBtn.update(mousePos);
        mInsertBtn.update(mousePos);
        mDeleteBtn.update(mousePos);
        mSearchBtn.update(mousePos);
        mHideMenuBtn.update(mousePos);
        mShowMenuBtn.update(mousePos);
    } else { mShowMenuBtn.update(mousePos); }

    if (mIsInsertExpanded){ mConfirmAddBtn  .update(mousePos); mInsertDiceBtn.update(mousePos); }
    if (mIsDeleteExpanded){ mConfirmRemoveBtn.update(mousePos); mDeleteDiceBtn.update(mousePos); }
    if (mIsSearchExpanded){ mConfirmSearchBtn.update(mousePos); mSearchDiceBtn.update(mousePos); }
    if (mIsUpdateExpanded){ mConfirmUpdateBtn.update(mousePos); }

    if (!event) return;

    if (const auto* me=event->getIf<sf::Event::MouseButtonPressed>()){
        if (me->button==sf::Mouse::Button::Left){
            bool inside=false;

            if (mIsSidebarVisible) {
                // Click Hide (<)
                if (sf::FloatRect({382.f, 150.f}, {45.f, 45.f}).contains(worldPos)) {
                    mIsSidebarVisible = false;
                    mIsInsertExpanded = mIsDeleteExpanded = mIsSearchExpanded = mIsNewExpanded = mIsUpdateExpanded = false;
                    inside = true;
                }
                // NEW BUTTON
                if (!mIsNewExpanded && sf::FloatRect({213.f, 150.f}, {160.f, 45.f}).contains(worldPos)){
                    mIsNewExpanded=true; mIsInsertExpanded=mIsDeleteExpanded=mIsSearchExpanded=mIsUpdateExpanded=false;
                    inside=true;
                }
                else if (mIsNewExpanded && sf::FloatRect({213.f, 150.f}, {160.f, 161.f}).contains(worldPos)){
                    inside=true;
                    if (sf::FloatRect({216.f, 207.f}, {154.f, 39.f}).contains(worldPos)){
                        doRandom(); mIsNewExpanded=false;
                    }
                    else if (sf::FloatRect({216.f, 261.f}, {154.f, 39.f}).contains(worldPos)){
                        const char* f[1]={"*.txt"};
                        const char* p=tinyfd_openFileDialog("Load hash table data","",1,f,"Text Files (*.txt)",0);
                        if (p){ mHash.loadFromFile(p); mCurrentStep=0; mTargetStep=-1; mLayout.setDescription("Loaded from file."); mLayout.setPaused(false); mAutoPlayClock.restart(); }
                        else mLayout.setDescription("Cancelled.");
                        mIsNewExpanded=false;
                    }
                }
                
                // INSERT BUTTON
                else if (!mIsInsertExpanded && sf::FloatRect({32.f, 204.f}, {160.f, 45.f}).contains(worldPos)){
                    mIsInsertExpanded=true; mIsNewExpanded=mIsDeleteExpanded=mIsSearchExpanded=mIsUpdateExpanded=false;
                    mInputValue=""; inside=true;
                }
                else if (mIsInsertExpanded && sf::FloatRect({32.f, 204.f}, {341.f, 45.f}).contains(worldPos)){
                    inside=true;
                    if (sf::FloatRect({216.f, 207.f}, {154.f, 39.f}).contains(worldPos) && !mInputValue.empty()){
                        runAction(1,std::stoi(mInputValue)); mIsInsertExpanded=false; mInputValue="";
                    }
                    else if (sf::FloatRect({167.f, 211.f}, {30.f, 30.f}).contains(worldPos))
                        mInputValue=std::to_string(rand()%999+1);
                }
                
                // DELETE BUTTON
                else if (!mIsDeleteExpanded && sf::FloatRect({32.f, 258.f}, {160.f, 45.f}).contains(worldPos)){
                    mIsDeleteExpanded=true; mIsNewExpanded=mIsInsertExpanded=mIsSearchExpanded=mIsUpdateExpanded=false;
                    mInputValue=""; inside=true;
                }
                else if (mIsDeleteExpanded && sf::FloatRect({32.f, 258.f}, {341.f, 45.f}).contains(worldPos)){
                    inside=true;
                    if (sf::FloatRect({216.f, 261.f}, {154.f, 39.f}).contains(worldPos) && !mInputValue.empty()){
                        runAction(2,std::stoi(mInputValue)); mIsDeleteExpanded=false; mInputValue="";
                    }
                    else if (sf::FloatRect({167.f, 265.f}, {30.f, 30.f}).contains(worldPos))
                        mInputValue=std::to_string(rand()%999+1);
                }
                
                // SEARCH BUTTON
                else if (!mIsSearchExpanded && sf::FloatRect({32.f, 312.f}, {160.f, 45.f}).contains(worldPos)){
                    mIsSearchExpanded=true; mIsNewExpanded=mIsInsertExpanded=mIsDeleteExpanded=mIsUpdateExpanded=false;
                    mInputValue=""; inside=true;
                }
                else if (mIsSearchExpanded && sf::FloatRect({32.f, 312.f}, {341.f, 45.f}).contains(worldPos)){
                    inside=true;
                    if (sf::FloatRect({216.f, 315.f}, {154.f, 39.f}).contains(worldPos) && !mInputValue.empty()){
                        runAction(3,std::stoi(mInputValue)); mIsSearchExpanded=false; mInputValue="";
                    }
                    else if (sf::FloatRect({167.f, 319.f}, {30.f, 30.f}).contains(worldPos))
                        mInputValue=std::to_string(rand()%999+1);
                }
                
                // UPDATE BUTTON
                else if (!mIsUpdateExpanded && sf::FloatRect({32.f, 366.f}, {160.f, 45.f}).contains(worldPos)){
                    mIsUpdateExpanded=true; mIsNewExpanded=mIsInsertExpanded=mIsDeleteExpanded=mIsSearchExpanded=false;
                    mInputOld=""; mInputNew=""; mEditingOld=true; inside=true;
                }
                // --- UPDATE ---
                else if (mIsUpdateExpanded && sf::FloatRect({32.f, 366.f}, {341.f, 45.f}).contains(worldPos)){
                    inside=true;
                    if (sf::FloatRect({216.f, 369.f}, {154.f, 39.f}).contains(worldPos) && !mInputOld.empty() && !mInputNew.empty()){
                        runAction(4, std::stoi(mInputNew), std::stoi(mInputOld));
                        mIsUpdateExpanded=false; mInputOld=""; mInputNew="";
                    }

                    else if (sf::FloatRect({35.f, 369.f}, {70.f, 39.f}).contains(worldPos)) { mEditingOld = true; }
                    else if (sf::FloatRect({135.f, 369.f}, {70.f, 39.f}).contains(worldPos)) { mEditingOld = false; }
                }
            } else {
                if (sf::FloatRect({32.f, 150.f}, {45.f, 45.f}).contains(worldPos)) {
                    mIsSidebarVisible = true;
                    inside = true;
                }
            }
            if (!inside)
                mIsInsertExpanded=mIsDeleteExpanded=mIsSearchExpanded=mIsUpdateExpanded=mIsNewExpanded=false;
        }
    }
    else if (const auto* ke=event->getIf<sf::Event::KeyPressed>()){
        if (ke->code==sf::Keyboard::Key::Z && (ke->control || ke->system)){
            mShowUndoBtn=false; mLayout.setPaused(true);
            mIsInsertExpanded=mIsDeleteExpanded=mIsSearchExpanded=mIsNewExpanded=mIsUpdateExpanded=false;
            mInputValue=""; mInputOld=""; mInputNew="";
        }
        else if (ke->code == sf::Keyboard::Key::I) {
            mIsInsertExpanded = true; mIsDeleteExpanded = mIsSearchExpanded = mIsUpdateExpanded = mIsNewExpanded = false; mInputValue = "";
        }
        else if (ke->code == sf::Keyboard::Key::D) {
            mIsDeleteExpanded = true; mIsInsertExpanded = mIsSearchExpanded = mIsUpdateExpanded = mIsNewExpanded = false; mInputValue = "";
        }
        else if (ke->code == sf::Keyboard::Key::S) {
            mIsSearchExpanded = true; mIsInsertExpanded = mIsDeleteExpanded = mIsUpdateExpanded = mIsNewExpanded = false; mInputValue = "";
        }
        else if (ke->code == sf::Keyboard::Key::U) {
            mIsUpdateExpanded = true; mIsInsertExpanded = mIsDeleteExpanded = mIsSearchExpanded = mIsNewExpanded = false; mEditingOld = true; mInputOld = ""; mInputNew = "";
        }
        else if (ke->code == sf::Keyboard::Key::N) {
            mIsNewExpanded = true; mIsInsertExpanded = mIsDeleteExpanded = mIsSearchExpanded = mIsUpdateExpanded = false; mInputValue = "";
        }
        else if (ke->code == sf::Keyboard::Key::Space) {
            mLayout.setPaused(!mLayout.isPaused());
            if (!mLayout.isPaused()) mAutoPlayClock.restart();
        }
        else if (ke->code == sf::Keyboard::Key::R) {
            if (mIsInsertExpanded || mIsDeleteExpanded || mIsSearchExpanded) {
                mInputValue = std::to_string(rand() % 999 + 1);
            } else if (mIsUpdateExpanded) {
            } else {
                doRandom(); mIsNewExpanded = false; mShowUndoBtn = false;
            }
        }
        else if (ke->code == sf::Keyboard::Key::Backspace) {
            if (mIsInsertExpanded || mIsDeleteExpanded || mIsSearchExpanded) {
                if (!mInputValue.empty()) mInputValue.pop_back();
            } else if (mIsUpdateExpanded) {
                if (mEditingOld && !mInputOld.empty()) mInputOld.pop_back();
                else if (!mEditingOld && !mInputNew.empty()) mInputNew.pop_back();
            }
        }
        else if (ke->code == sf::Keyboard::Key::Enter) {
            if (mIsInsertExpanded && !mInputValue.empty()) {
                runAction(1, std::stoi(mInputValue)); mIsInsertExpanded = false; mInputValue = "";
            }
            else if (mIsDeleteExpanded && !mInputValue.empty()) {
                runAction(2, std::stoi(mInputValue)); mIsDeleteExpanded = false; mInputValue = "";
            }
            else if (mIsSearchExpanded && !mInputValue.empty()) {
                runAction(3, std::stoi(mInputValue)); mIsSearchExpanded = false; mInputValue = "";
            }
            else if (mIsUpdateExpanded && !mInputOld.empty() && !mInputNew.empty()) {
                runAction(4, std::stoi(mInputNew), std::stoi(mInputOld)); mIsUpdateExpanded = false; mInputOld = ""; mInputNew = "";
            }
        }
        else if (ke->code == sf::Keyboard::Key::Tab) {
            if (mIsUpdateExpanded) mEditingOld = !mEditingOld;
        }
        else {
            int digit = -1;
            if (ke->code >= sf::Keyboard::Key::Num0 && ke->code <= sf::Keyboard::Key::Num9)
                digit = (int)ke->code - (int)sf::Keyboard::Key::Num0;
            else if (ke->code >= sf::Keyboard::Key::Numpad0 && ke->code <= sf::Keyboard::Key::Numpad9)
                digit = (int)ke->code - (int)sf::Keyboard::Key::Numpad0;

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

// ── render ───────────────────────────────────────────────────
void HashVisualizer::render(bool showUI){
    if (showUI) {
        setTransitionProgress(1.0f);
    } else if (mTransitionProgress == 1.0f) {
        setTransitionProgress(0.0f);
    }
    
    auto steps = mHash.getSteps();
    int total  = (int)steps.size();

    // Step animation
    if (mTargetStep != -1 && mTargetStep != mCurrentStep) {
        float speedMult = std::max(0.1f, mLayout.getSpeed());
        float frameDelta = (1.f / 60.f) * speedMult;
        
        mStepAnimProgress += frameDelta;
        
        if (mStepAnimProgress >= 1.f) {
            mCurrentStep = mTargetStep;
            if (!mLayout.isPaused() && mCurrentStep < total - 1) {
                mTargetStep = mCurrentStep + 1;
                mStepAnimProgress = 0.f;
            } else {
                mTargetStep = -1;
                mStepAnimProgress = 1.f;
            }
        }
    }
    else if (!mLayout.isPaused() && showUI && mCurrentStep < total - 1 && mTargetStep == -1) {
        mTargetStep = mCurrentStep + 1;
        mStepAnimProgress = 0.f;
    }
    
    computeHashLayout();
    renderBuckets(*mWindow);
    renderNodes  (*mWindow);
    
    if (!showUI) return;

    if (mCurrentStep >= total && total > 0) mCurrentStep = total - 1;
    if (mCurrentStep < 0) mCurrentStep = 0;


    // Step text
    if (total==0) mLayout.setStepText("Step 0 / 0");
    else          mLayout.setStepText("Step "+std::to_string(mCurrentStep+1)+" / "+std::to_string(total));

    // Description from current step
    if (mCurrentStep>=0 && mCurrentStep<total) {
        mLayout.setDescription(steps[mCurrentStep].description);
        
        int currentLid = steps[mCurrentStep].lineID;
        
        auto [tableIdx, activeLines] = lidToPseudo(currentLid);
        
        if (tableIdx == 0) mLayout.setPseudoCode(PSEUDO_INSERT);
        else if (tableIdx == 1) mLayout.setPseudoCode(PSEUDO_SEARCH);
        else if (tableIdx == 2) mLayout.setPseudoCode(PSEUDO_DELETE);
        else mLayout.setPseudoCode({});

        mLayout.setActiveCodeLines(activeLines);
    }
    mLayout.draw(*mWindow);

    // Title — use theme colors
    mTitleNum  .setFillColor(ThemeManager::current.textColor);
    mTitleHash .setFillColor(ThemeManager::current.primary);
    mTitleChain.setFillColor(ThemeManager::current.primaryLight);
    mWindow->draw(mTitleNum);
    mWindow->draw(mTitleHash);
    mWindow->draw(mTitleChain);

    // Sidebar theme colors
    sf::Color sbCol=ThemeManager::current.secondary;
    sf::Color hlCol=ThemeManager::current.primary;
    for (auto* b:{&mClearBtn,&mNewBtn,&mInsertBtn,&mDeleteBtn,&mSearchBtn,&mUndoBtn, &mUpdateBtn,
                  &mInsertHoverStroke,&mInsertExpandedStroke,
                  &mDeleteHoverStroke,&mDeleteExpandedStroke,
                  &mSearchHoverStroke,&mSearchExpandedStroke,
                  &mNewHoverStroke,&mNewExpandedStroke, &mHideMenuBtn, &mShowMenuBtn})
        b->setThemeColor(sbCol);
    for (auto* b:{&mInsertHoverStroke,&mInsertExpandedStroke,
                  &mDeleteHoverStroke,&mDeleteExpandedStroke,
                  &mSearchHoverStroke,&mSearchExpandedStroke,
                  &mUpdateHoverStroke,&mUpdateExpandedStroke,
                  &mNewHoverStroke,&mNewExpandedStroke})
        b->setThemeColor(hlCol);
    for (auto* b:{&mInsertExpandedBg,&mDeleteExpandedBg,&mSearchExpandedBg, &mUpdateExpandedBg, &mNewExpandedBg})
        b->setThemeColor(sbCol);
    for (auto* b:{&mConfirmAddBtn,&mConfirmRemoveBtn,&mConfirmSearchBtn, &mConfirmUpdateBtn, &mRandomBtn,&mUploadBtn})
        b->setThemeColor(ThemeManager::current.bg);

    sf::Vector2f worldPos=mWindow->mapPixelToCoords(sf::Mouse::getPosition(*mWindow));
    
    if (!mIsSidebarVisible) {
        mShowMenuBtn.setThemeColor(ThemeManager::current.secondary);
        mShowMenuBtn.draw(*mWindow);
    }
    else {
        mHideMenuBtn.setThemeColor(ThemeManager::current.secondary);
        mHideMenuBtn.draw(*mWindow);
        if (mShowUndoBtn){ mUndoBtn.setThemeColor(sbCol); mUndoBtn.draw(*mWindow); }
        else             { mClearBtn.setThemeColor(sbCol); mClearBtn.draw(*mWindow); }

        if (mInsertDiceBtn.mSprite){mInsertDiceBtn.mSprite->setColor(ThemeManager::current.textColor);}
        if (mDeleteDiceBtn.mSprite){mDeleteDiceBtn.mSprite->setColor(ThemeManager::current.textColor);}
        if (mSearchDiceBtn.mSprite){mSearchDiceBtn.mSprite->setColor(ThemeManager::current.textColor);}
        
        // New
        if (mIsNewExpanded){
            mNewExpandedStroke.draw(*mWindow);
            mNewExpandedBg.draw(*mWindow);
            mNewBtn.draw(*mWindow);
            mRandomBtn.draw(*mWindow);
            mUploadBtn.draw(*mWindow);
        } else {
            if (sf::FloatRect({213.f, 150.f}, {160.f, 45.f}).contains(worldPos)) mNewHoverStroke.draw(*mWindow);
            mNewBtn.draw(*mWindow);
        }

        // Insert
        if (mIsInsertExpanded){
            mInsertExpandedStroke.draw(*mWindow);
            mInsertExpandedBg.draw(*mWindow);
            mConfirmAddBtn.draw(*mWindow);
            mInsertDiceBtn.draw(*mWindow);
            mInsertInputText.setString(mInputValue);
            mInsertInputText.setFillColor(ThemeManager::current.textColor);
            mWindow->draw(mInsertInputText);
            
            if (mCursorClock.getElapsedTime().asSeconds()>= 0.5f){ mShowCursor=!mShowCursor; mCursorClock.restart(); }
            if (mShowCursor){
                mInsertCursorLine.setFillColor(ThemeManager::current.textColor);
                float tw = mInsertInputText.getLocalBounds().size.x;
                float cursorX = mInputValue.empty() ? 47.f : 47.f + tw + 2.f;
                mInsertCursorLine.setPosition(sf::Vector2f(cursorX, 214.f));
                mWindow->draw(mInsertCursorLine);
            }
        } else {
            if (sf::FloatRect({32.f, 204.f}, {160.f, 45.f}).contains(worldPos)) mInsertHoverStroke.draw(*mWindow);
            mInsertBtn.draw(*mWindow);
        }

        // Delete
        if (mIsDeleteExpanded){
            mDeleteExpandedStroke.draw(*mWindow);
            mDeleteExpandedBg.draw(*mWindow);
            mConfirmRemoveBtn.draw(*mWindow);
            mDeleteDiceBtn.draw(*mWindow);
            mDeleteInputText.setString(mInputValue);
            mDeleteInputText.setFillColor(ThemeManager::current.textColor);
            mWindow->draw(mDeleteInputText);
            
            if (mCursorClock.getElapsedTime().asSeconds()>=0.5f){ mShowCursor=!mShowCursor; mCursorClock.restart(); }
            if (mShowCursor){
                mDeleteCursorLine.setFillColor(ThemeManager::current.textColor);
                float tw = mDeleteInputText.getLocalBounds().size.x;
                float cursorX = mInputValue.empty() ? 47.f : 47.f + tw + 2.f;
                mDeleteCursorLine.setPosition(sf::Vector2f(cursorX, 268.f));
                mWindow->draw(mDeleteCursorLine);
            }
        } else {
            if (sf::FloatRect({32.f, 258.f}, {160.f, 45.f}).contains(worldPos)) mDeleteHoverStroke.draw(*mWindow);
            mDeleteBtn.draw(*mWindow);
        }

        // Search
        if (mIsSearchExpanded){
            mSearchExpandedStroke.draw(*mWindow);
            mSearchExpandedBg.draw(*mWindow);
            mConfirmSearchBtn.draw(*mWindow);
            mSearchDiceBtn.draw(*mWindow);
            mSearchInputText.setString(mInputValue);
            mSearchInputText.setFillColor(ThemeManager::current.textColor);
            mWindow->draw(mSearchInputText);
            
            if (mCursorClock.getElapsedTime().asSeconds()>=0.5f){ mShowCursor=!mShowCursor; mCursorClock.restart(); }
            if (mShowCursor){
                mSearchCursorLine.setFillColor(ThemeManager::current.textColor);
                float tw = mSearchInputText.getLocalBounds().size.x;
                float cursorX = mInputValue.empty() ? 47.f : 47.f + tw + 2.f;
                mSearchCursorLine.setPosition(sf::Vector2f(cursorX, 322.f));
                mWindow->draw(mSearchCursorLine);
            }
        } else {
            if (sf::FloatRect({32.f, 312.f}, {160.f, 45.f}).contains(worldPos)) mSearchHoverStroke.draw(*mWindow);
            mSearchBtn.draw(*mWindow);
        }

        // --- UPDATE ---
        if (mIsUpdateExpanded) {
            mUpdateExpandedStroke.draw(*mWindow);
            mUpdateExpandedBg.draw(*mWindow);
            mConfirmUpdateBtn.draw(*mWindow);

            float baseY = 366.f;
            sf::Color boxFill = ThemeManager::current.bg;
            sf::Color activeBorder = ThemeManager::current.primary;
            sf::Color inactiveBorder = sf::Color(200, 200, 200);

            drawRoundedRect(*mWindow, sf::FloatRect({35.f, baseY + 3.f}, {70.f, 39.f}), 19.5f,
                            boxFill, mEditingOld ? activeBorder : inactiveBorder, 2.f);
            drawRoundedRect(*mWindow, sf::FloatRect({135.f, baseY + 3.f}, {70.f, 39.f}), 19.5f,
                            boxFill, !mEditingOld ? activeBorder : inactiveBorder, 2.f);
            
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

            // Xử lý con trỏ nhấp nháy (|)
            if (mCursorClock.getElapsedTime().asSeconds() >= 0.5f) { mShowCursor = !mShowCursor; mCursorClock.restart(); }
            if (mShowCursor) {
                float cursorX;
                if (mEditingOld) {
                    cursorX = 35.f + (70.f + oldBounds.size.x)/2.f + 2.f;
                } else {
                    cursorX = 135.f + (70.f + newBounds.size.x)/2.f + 2.f;
                }
                sf::RectangleShape cursorLine(sf::Vector2f(1.5f, 20.f));
                cursorLine.setFillColor(ThemeManager::current.textColor);
                cursorLine.setPosition(sf::Vector2f(cursorX, baseY + 13.f));
                mWindow->draw(cursorLine);
            }
        } else {
            if (sf::FloatRect({32.f, 366.f}, {160.f, 45.f}).contains(worldPos)) mUpdateHoverStroke.draw(*mWindow);
            mUpdateBtn.draw(*mWindow);
        }
    }
}

void HashVisualizer::computeHashLayout() {
    // Tự lấy snapshot hiện tại để tính maxChainLen
    auto steps = mHash.getSteps();
    int  total = (int)steps.size();

    int maxChainLen = 0;
    if (!steps.empty() && mCurrentStep >= 0 && mCurrentStep < total) {
        for (const auto& bucket : steps[mCurrentStep].tableSnapshot) {
            maxChainLen = std::max(maxChainLen, (int)bucket.vals.size());
        }
    }

    float p     = mTransitionProgress;
    float easeP = p * p * (3.f - 2.f * p);

    int   n              = mHash.getSize();
    float baseBucketGapX = 88.f;
    float baseBucketW    = 80.f;
    float baseBucketH    = 58.f;
    float baseGapY       = 72.f;
    float baseNodeH      = 46.f;

    float totalW = (float)n * baseBucketGapX - (baseBucketGapX - baseBucketW);

    // ── Vis layout ──────────────────────────────────────────────
    float visBucketRowY   = 882.f - baseBucketH - 55.f;
    float visBucketStartX = (1440.f - totalW) * 0.5f;

    float safeTop       = 150.f;
    float naturalHeight = (float)maxChainLen * baseGapY;
    float availH        = visBucketRowY - safeTop;
    float visScaleY     = (naturalHeight > 0.f && naturalHeight > availH)
                              ? availH / naturalHeight : 1.f;
    float visGapY  = baseGapY  * visScaleY;
    float visNodeH = baseNodeH * visScaleY;

    // ── Menu thumbnail layout ────────────────────────────────────
    float menuCenterX = 540.f;
    float menuUsableW = 300.f;
    float menuScaleX  = (totalW > menuUsableW) ? menuUsableW / totalW : 1.f;

    float menuBucketGapX   = baseBucketGapX * menuScaleX;
    float menuBucketW      = baseBucketW    * menuScaleX;
    float menuBucketH      = baseBucketH    * menuScaleX;
    float menuTotalW       = (float)n * menuBucketGapX - (menuBucketGapX - menuBucketW);
    float menuBucketStartX = menuCenterX - menuTotalW * 0.5f;
    float menuBucketRowY   = 395.f;

    float menuUsableH = 195.f;
    float menuScaleY  = (naturalHeight > menuUsableH) ? menuUsableH / naturalHeight : 1.f;
    float menuGapY    = baseGapY  * menuScaleY;
    float menuNodeH   = baseNodeH * menuScaleY;

    // ── Interpolate ─────────────────────────────────────────────
    mBucketGapX   = menuBucketGapX   + (baseBucketGapX  - menuBucketGapX)   * easeP;
    mBucketW      = menuBucketW      + (baseBucketW      - menuBucketW)      * easeP;
    mBucketH      = menuBucketH      + (baseBucketH      - menuBucketH)      * easeP;
    mBucketStartX = menuBucketStartX + (visBucketStartX  - menuBucketStartX) * easeP;
    mBucketRowY   = menuBucketRowY   + (visBucketRowY    - menuBucketRowY)   * easeP;
    mGapY         = menuGapY         + (visGapY          - menuGapY)         * easeP;
    mNodeH        = menuNodeH        + (visNodeH         - menuNodeH)        * easeP;
}
