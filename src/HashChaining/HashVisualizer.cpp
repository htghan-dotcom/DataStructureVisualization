#include "HashChaining/HashVisualizer.h"
#include "tinyfiledialogs.h"
#include "Common.h"
#include <cmath>
#include <cstdlib>

// ============================================================
//  Coordinate constants – mirror RBTVisualizer exactly
//  (sidebar buttons sit at the same x/y as RBT equivalents)
// ============================================================

// Sidebar button column
static constexpr float SB_X  = 70.f;   // left edge of button
static constexpr float SB_W  = 160.f;  // button width
static constexpr float SB_H  = 45.f;   // button height
static constexpr float SB_R  = 21.f;   // corner radius

// Button top-y values (matching RBT exactly)
static constexpr float Y_CLEAR  = 176.f;
static constexpr float Y_NEW    = 176.f;   // same row as Clear, offset right
static constexpr float Y_INSERT = 230.f;
static constexpr float Y_DELETE = 284.f;
static constexpr float Y_SEARCH = 338.f;

static constexpr float NEW_X    = 251.f;   // "New" button sits right of "Clear"

// Playback bar  (footer at y = 882)
static constexpr float FOOTER_Y = 882.f;

// ============================================================
//  Constructor
// ============================================================

HashVisualizer::HashVisualizer(sf::RenderWindow& window)
    : mWindow(&window),
      mHash(7),

      mTitleNum (mFontBold),
      mTitleHash(mFontBold),
      mTitleChain(mFontBold),
      mStepText (mFontRegular),

      // ── Main action buttons ──
      mClearBtn (mFontRegular, "Clear",   SB_X,   Y_CLEAR,  SB_W, SB_H, SB_R, sf::Color(217,217,217)),
      mNewBtn   (mFontRegular, "New",     NEW_X,  Y_NEW,    SB_W, SB_H, SB_R, sf::Color(217,217,217)),
      mInsertBtn(mFontRegular, "Insert",  SB_X,   Y_INSERT, SB_W, SB_H, SB_R, sf::Color(217,217,217)),
      mDeleteBtn(mFontRegular, "Delete",  SB_X,   Y_DELETE, SB_W, SB_H, SB_R, sf::Color(217,217,217)),
      mSearchBtn(mFontRegular, "Search",  SB_X,   Y_SEARCH, SB_W, SB_H, SB_R, sf::Color(217,217,217)),
      mUndoBtn  (mFontRegular, "Undo",    SB_X,   Y_CLEAR,  SB_W, SB_H, SB_R, sf::Color(217,217,217)),

      mSpeedSlider(mFontRegular, 956.f, 921.f),

      mStepBackBtn   (mFontRegular, "Step back",    326.f, 896.f, 165.f, 49.f, 24.5f, sf::Color(245,245,240)),
      mStepForwardBtn(mFontRegular, "Step forward", 504.f, 896.f, 201.f, 49.f, 24.5f, sf::Color(245,245,240)),

      // ── Insert expand ──
      mInsertHoverStroke   (mFontRegular, "", SB_X-2,  Y_INSERT-2,  SB_W+4,        SB_H+4,        23.f, sf::Color(90,150,44)),
      mInsertExpandedStroke(mFontRegular, "", SB_X-2,  Y_INSERT-2,  SB_W*2+4+12.f, SB_H+4,        23.f, sf::Color(90,150,44)),
      mInsertExpandedBg    (mFontRegular, "", SB_X,    Y_INSERT,    SB_W*2+8.f,    SB_H,          21.f, sf::Color(217,217,217)),
      mConfirmAddBtn       (mFontRegular, "Add",    SB_X+SB_W+12.f+8.f, Y_INSERT+3.f, 154.f, 39.f, 19.5f, sf::Color(245,245,240)),
      mInsertInputText     (mFontRegular),

      // ── Delete expand ──
      mDeleteHoverStroke   (mFontRegular, "", SB_X-2,  Y_DELETE-2,  SB_W+4,        SB_H+4,        23.f, sf::Color(90,150,44)),
      mDeleteExpandedStroke(mFontRegular, "", SB_X-2,  Y_DELETE-2,  SB_W*2+4+12.f, SB_H+4,        23.f, sf::Color(90,150,44)),
      mDeleteExpandedBg    (mFontRegular, "", SB_X,    Y_DELETE,    SB_W*2+8.f,    SB_H,          21.f, sf::Color(217,217,217)),
      mConfirmRemoveBtn    (mFontRegular, "Remove", SB_X+SB_W+12.f+8.f, Y_DELETE+3.f, 154.f, 39.f, 19.5f, sf::Color(245,245,240)),
      mDeleteInputText     (mFontRegular),

      // ── Search expand ──
      mSearchHoverStroke   (mFontRegular, "", SB_X-2,  Y_SEARCH-2,  SB_W+4,        SB_H+4,        23.f, sf::Color(90,150,44)),
      mSearchExpandedStroke(mFontRegular, "", SB_X-2,  Y_SEARCH-2,  SB_W*2+4+12.f, SB_H+4,        23.f, sf::Color(90,150,44)),
      mSearchExpandedBg    (mFontRegular, "", SB_X,    Y_SEARCH,    SB_W*2+8.f,    SB_H,          21.f, sf::Color(217,217,217)),
      mConfirmSearchBtn    (mFontRegular, "Search", SB_X+SB_W+12.f+8.f, Y_SEARCH+3.f, 154.f, 39.f, 19.5f, sf::Color(245,245,240)),
      mSearchInputText     (mFontRegular),

      // ── New expand (Random / From File) ──
      mNewHoverStroke   (mFontRegular, "", NEW_X-2, Y_NEW-2, SB_W+4,   SB_H+4,   23.f, sf::Color(90,150,44)),
      mNewExpandedStroke(mFontRegular, "", NEW_X-2, Y_NEW-2, SB_W+4,   SB_H*4,   23.f, sf::Color(90,150,44)),
      mNewExpandedBg    (mFontRegular, "", NEW_X,   Y_NEW,   SB_W,     SB_H*4-4, 21.f, sf::Color(217,217,217)),
      mRandomBtn        (mFontRegular, "Random",    NEW_X+3.f, Y_NEW+SB_H+4.f,  SB_W-6.f, 39.f, 19.5f, sf::Color(245,245,240)),
      mUploadBtn        (mFontRegular, "From File", NEW_X+3.f, Y_NEW+SB_H+49.f, SB_W-6.f, 39.f, 19.5f, sf::Color(245,245,240))
{
    // ── Fonts ──
    if (!mFontBold.openFromFile("assets/fonts/Inter-Bold.ttf") ||
        !mFontRegular.openFromFile("assets/fonts/Inter-Regular.ttf")) {
        cerr << "HashVisualizer: cannot load fonts" << endl;
    }

    // ── Playback textures ──
    auto loadTex = [](sf::Texture& t, const char* path){
        if (!t.loadFromFile(path)) cerr << "Cannot load " << path << endl;
        t.setSmooth(true);
        if (!t.generateMipmap()) cerr << "Warning: Could not generate mipmap for " << path << endl;
    };
    loadTex(mSkipBackTex,    "assets/images/skipbackButton.png");
    loadTex(mPauseTex,       "assets/images/pauseButton.png");
    loadTex(mSkipForwardTex, "assets/images/skipforwardButton.png");
    loadTex(mStartTex,       "assets/images/startButton.png");
    loadTex(mDiceTex,        "assets/images/randomButton.png");

    // ── Playback button layout (same as RBT) ──
    mSkipBackBtn .setup(mSkipBackTex,    333.f, 897.f, 48.f, 48.f);
    mPauseBtn    .setup(mPauseTex,       397.f, 897.f, 48.f, 48.f);
    mSkipForwardBtn.setup(mSkipForwardTex, 461.f, 897.f, 48.f, 48.f);
    mStartBtn    .setup(mStartTex,       134.f, 897.f, 48.f, 48.f);

    // ── Dice buttons ──
    mInsertDiceBtn.setup(mDiceTex, SB_X+SB_W-4.f,       Y_INSERT+7.f, 30.f, 30.f);
    mDeleteDiceBtn.setup(mDiceTex, SB_X+SB_W-4.f,       Y_DELETE+7.f, 30.f, 30.f);
    mSearchDiceBtn.setup(mDiceTex, SB_X+SB_W-4.f,       Y_SEARCH+7.f, 30.f, 30.f);

    // ── Input text display ──
    auto setupInputText = [&](sf::Text& t, float y){
        t.setFont(mFontRegular);
        t.setCharacterSize(22);
        t.setFillColor(sf::Color::Black);
        t.setPosition(sf::Vector2f(SB_X+8.f, y));
    };
    setupInputText(mInsertInputText, Y_INSERT+10.f);
    setupInputText(mDeleteInputText, Y_DELETE+10.f);
    setupInputText(mSearchInputText, Y_SEARCH+10.f);

    // ── Cursor lines ──
    for (auto* c : {&mInsertCursorLine, &mDeleteCursorLine, &mSearchCursorLine}){
        c->setSize(sf::Vector2f(2.f, 24.f));
        c->setFillColor(sf::Color::Black);
    }

    // ── Step pill background (same geometry as RBT) ──
    float bgX=131.f, bgY=890.f, bgW=580.f, bgH=61.f, bgR=30.5f;
    sf::Color bgCol(196,196,196);

    mStepBgLeft .setRadius(bgR);
    mStepBgLeft .setPosition(sf::Vector2f(bgX, bgY));
    mStepBgLeft .setFillColor(bgCol);

    mStepBgRight.setRadius(bgR);
    mStepBgRight.setPosition(sf::Vector2f(bgX+bgW-bgH, bgY));
    mStepBgRight.setFillColor(bgCol);

    mStepBgCenter.setSize(sf::Vector2f(bgW-bgH, bgH));
    mStepBgCenter.setPosition(sf::Vector2f(bgX+bgR, bgY));
    mStepBgCenter.setFillColor(bgCol);

    mStepText.setString("Step 0 / 0");
    mStepText.setCharacterSize(21);
    mStepText.setFillColor(sf::Color::Black);
    mStepText.setPosition(sf::Vector2f(192.f, 906.f));

    // ── Title labels (bottom-left) ──
    mTitleNum.setString("02");
    mTitleNum.setCharacterSize(30);
    mTitleNum.setFillColor(sf::Color::Black);
    mTitleNum.setPosition(sf::Vector2f(67.f, 663.f));

    mTitleHash.setString("Hash");
    mTitleHash.setCharacterSize(70);
    mTitleHash.setFillColor(sf::Color(242, 131, 175));   // pink
    mTitleHash.setPosition(sf::Vector2f(67.f, 695.f));

    mTitleChain.setString("Chaining");
    mTitleChain.setCharacterSize(70);
    mTitleChain.setFillColor(sf::Color(60, 140, 200));      // blue accent
    mTitleChain.setPosition(sf::Vector2f(67.f, 765.f));

    // ── refreshText() on all RoundedButtons ──
    for (auto* b : {&mClearBtn,&mNewBtn,&mInsertBtn,&mDeleteBtn,&mSearchBtn,&mUndoBtn,
                    &mStepBackBtn,&mStepForwardBtn,
                    &mConfirmAddBtn,&mConfirmRemoveBtn,&mConfirmSearchBtn,
                    &mRandomBtn,&mUploadBtn})
        b->refreshText();

    // ── Button callbacks ──

    mPauseBtn.setCallback([this](){
        mIsPaused = true;
        mSkipBackBtn  .setup(mSkipBackTex,    67.f,  897.f, 48.f, 48.f);
        mSkipForwardBtn.setup(mSkipForwardTex, 735.f, 897.f, 48.f, 48.f);
    });
    mStartBtn.setCallback([this](){
        mIsPaused = false;
        mSkipBackBtn  .setup(mSkipBackTex,    333.f, 897.f, 48.f, 48.f);
        mSkipForwardBtn.setup(mSkipForwardTex, 461.f, 897.f, 48.f, 48.f);
    });
    mSkipBackBtn.setCallback([this](){
        mCurrentStep = 0;
        mIsPaused = true;
    });
    mSkipForwardBtn.setCallback([this](){
        int total = (int)mHash.getSteps().size();
        mCurrentStep = std::max(0, total - 1);
        mIsPaused = true;
    });
    mStepBackBtn.setCallback([this](){
        if (mCurrentStep > 0){
            mTargetStep = mCurrentStep - 1;
            mStepAnimProgress = 0.f;
        }
        mIsPaused = true;
    });
    mStepForwardBtn.setCallback([this](){
        int total = (int)mHash.getSteps().size();
        if (mCurrentStep < total - 1){
            mTargetStep = mCurrentStep + 1;
            mStepAnimProgress = 0.f;
        }
        mIsPaused = true;
    });

    mClearBtn.setCallback([this](){
        mHash.clear();
        mHash.generateRandom(0);
        mCurrentStep = std::max(0, (int)mHash.getSteps().size() - 1);
        mTargetStep  = -1;
        mLayout.setDescription("Hash table cleared.");
        mShowUndoBtn = false;
        mIsPaused    = true;
    });
    mUndoBtn.setCallback([this](){
        // no backup/restore in HashChaining yet – just hide undo btn
        mShowUndoBtn = false;
    });

    // ── Initial random data ── jump to last step so it shows immediately
    mHash.generateRandom(4 + rand() % 5);
    mCurrentStep = std::max(0, (int)mHash.getSteps().size() - 1);
    mTargetStep  = -1;
    mIsPaused    = true;
}

// ============================================================
//  resetPlayUI  (same as RBT)
// ============================================================
void HashVisualizer::resetPlayUI(){
    mIsPaused = false;
    mShowUndoBtn = false;
    mSkipBackBtn  .setup(mSkipBackTex,    333.f, 897.f, 48.f, 48.f);
    mSkipForwardBtn.setup(mSkipForwardTex, 461.f, 897.f, 48.f, 48.f);
    mAutoPlayClock.restart();
}

// ============================================================
//  runAction – commit an operation then reset step to 0
// ============================================================
void HashVisualizer::runAction(int action, int value, int oldValue){
    // action: 1=add  2=delete  3=search  4=update
    if      (action == 1) { mHash.add(value);                    mLayout.setDescription("Added " + std::to_string(value)); }
    else if (action == 2) { mHash.deleteNode(value);             mLayout.setDescription("Deleted " + std::to_string(value)); }
    else if (action == 3) { mHash.search(value);                 mLayout.setDescription("Searching " + std::to_string(value)); }
    else if (action == 4) { mHash.update(oldValue, value);       mLayout.setDescription("Updated " + std::to_string(oldValue) + " -> " + std::to_string(value)); }

    mCurrentStep = 0;
    mTargetStep  = -1;
    mStepAnimProgress = 1.f;
    resetPlayUI();
}

// ============================================================
//  checkReturnHome
// ============================================================
bool HashVisualizer::checkReturnHome(){
    if (mGoHome){ mGoHome = false; return true; }
    return false;
}

// ============================================================
//  Draw helpers  (kept verbatim from original HashVisualizer)
// ============================================================

void HashVisualizer::drawRoundedRect(sf::RenderWindow& window, const sf::FloatRect& rect,
                                      float radius, const sf::Color& fill,
                                      const sf::Color& border, float borderThick) {
    auto drawFill = [&](const sf::FloatRect& r, float rad, const sf::Color& col){
        float rw = r.size.x, rh = r.size.y;
        float rx = r.position.x, ry = r.position.y;
        if (rw <= 0 || rh <= 0) return;
        float cr = std::min(rad, std::min(rw, rh) * 0.5f);
        sf::RectangleShape h(sf::Vector2f(rw - 2*cr, rh));
        h.setPosition({rx+cr, ry}); h.setFillColor(col); window.draw(h);
        sf::RectangleShape v(sf::Vector2f(rw, rh - 2*cr));
        v.setPosition({rx, ry+cr}); v.setFillColor(col); window.draw(v);
        sf::CircleShape c(cr); c.setFillColor(col);
        c.setPosition({rx,         ry        }); window.draw(c);
        c.setPosition({rx+rw-2*cr, ry        }); window.draw(c);
        c.setPosition({rx,         ry+rh-2*cr}); window.draw(c);
        c.setPosition({rx+rw-2*cr, ry+rh-2*cr}); window.draw(c);
    };
    drawFill(rect, radius, border);
    if (borderThick > 0.f){
        sf::FloatRect inner(
            sf::Vector2f(rect.position.x + borderThick, rect.position.y + borderThick),
            sf::Vector2f(std::max(0.f, rect.size.x - borderThick*2),
                         std::max(0.f, rect.size.y - borderThick*2)));
        drawFill(inner, std::max(0.f, radius-borderThick), fill);
    }
}

void HashVisualizer::drawArrowUp(sf::RenderWindow& window, sf::Vector2f from,
                                  sf::Vector2f to, const sf::Color& color){
    sf::Vector2f dir = to - from;
    float len = std::sqrt(dir.x*dir.x + dir.y*dir.y);
    if (len < 1.f) return;
    dir /= len;
    sf::Vector2f perp(-dir.y, dir.x);

    float arrowSize = 8.f, arrowWidth = 8.f, lineThickness = 2.f;

    sf::RectangleShape line(sf::Vector2f(len - arrowSize, lineThickness));
    line.setOrigin(sf::Vector2f(0.f, lineThickness/2.f));
    line.setPosition(from);
    line.setFillColor(color);
    line.setRotation(sf::radians(std::atan2(dir.y, dir.x)));
    window.draw(line);

    sf::ConvexShape tri; tri.setPointCount(3);
    tri.setPoint(0, to);
    tri.setPoint(1, to - dir*arrowSize + perp*(arrowWidth/2.f));
    tri.setPoint(2, to - dir*arrowSize - perp*(arrowWidth/2.f));
    tri.setFillColor(color);
    window.draw(tri);
}

// ============================================================
//  renderBuckets  (original, just reads from mHash)
// ============================================================
void HashVisualizer::renderBuckets(sf::RenderWindow& window){
    auto steps = mHash.getSteps();
    int targetBucket = -1;
    if (mCurrentStep >= 0 && mCurrentStep < (int)steps.size())
        targetBucket = steps[mCurrentStep].targetBucket;

    int n = mHash.getSize();
    mBucketW   = 80.f;
    mBucketH   = 58.f;
    mBucketGapX= 88.f;

    float totalW   = n * mBucketGapX - (mBucketGapX - mBucketW);
    // Canvas sits above the footer (882px tall)
    float canvasH  = FOOTER_Y;
    mBucketStartX  = (1440.f - totalW) * 0.5f;
    mBucketRowY    = canvasH - mBucketH - 55.f;

    for (int i = 0; i < n; ++i){
        float px = mBucketStartX + i * mBucketGapX;
        float py = mBucketRowY;

        bool isTarget = (i == targetBucket);
        sf::Color fill   = isTarget ? sf::Color(255,235,180,230) : sf::Color(195,218,240,200);
        sf::Color border = isTarget ? sf::Color(220,150, 60,220) : sf::Color(130,170,210,200);

        drawRoundedRect(window, sf::FloatRect(sf::Vector2f(px, py), sf::Vector2f(mBucketW, mBucketH)), 10.f, fill, border, 3.5f);

        sf::Text lbl(mFontRegular, "H"+std::to_string(i), 18);
        lbl.setFillColor(sf::Color(60,80,130));
        sf::FloatRect lb = lbl.getLocalBounds();
        lbl.setPosition(sf::Vector2f(px+(mBucketW-lb.size.x)*0.5f-lb.position.x, py+(mBucketH-lb.size.y)*0.5f-lb.position.y));
        window.draw(lbl);

        sf::Text idx(mFontRegular, std::to_string(i), 11);
        idx.setFillColor(sf::Color(120,140,180));
        sf::FloatRect ib = idx.getLocalBounds();
        idx.setPosition(sf::Vector2f(px+(mBucketW-ib.size.x)*0.5f-ib.position.x, py+mBucketH+4.f));
        window.draw(idx);
    }
}

// ============================================================
//  renderNodes  (original, highlight from current step)
// ============================================================
void HashVisualizer::renderNodes(sf::RenderWindow& window){
    auto steps = mHash.getSteps();
    if (steps.empty()) return;

    int stepIdx = std::max(0, std::min(mCurrentStep, (int)steps.size()-1));
    const auto& step = steps[stepIdx];

    int targetBucket = step.targetBucket;
    int targetVal    = step.targetVal;

    // Use snapshot stored in this step — not the live table
    const auto& snap = step.tableSnapshot;
    if (snap.empty()) return;

    float nodeW = mBucketW;
    float nodeH = 46.f;
    float gapY  = 72.f;

    static const sf::Color pal[][2] = {
        {{188,214,245,220},{210,230,255,220}},
        {{245,188,210,220},{255,210,230,220}},
        {{188,240,210,220},{210,255,230,220}},
        {{245,235,155,220},{255,248,190,220}},
        {{220,195,245,220},{238,220,255,220}},
        {{245,210,180,220},{255,230,205,220}},
        {{175,230,240,220},{205,245,255,220}},
        {{240,200,200,220},{255,225,225,220}},
    };
    static std::unordered_map<int,int> colorIdx;

    for (int i = 0; i < (int)snap.size(); ++i){
        float bx = mBucketStartX + i * mBucketGapX;
        float by = mBucketRowY;

        const auto& chain = snap[i].vals;
        for (int depth = 0; depth < (int)chain.size(); ++depth){
            int nodeVal = chain[depth];
            float px = bx;
            float py = by - (depth+1)*gapY;

            if (colorIdx.find(nodeVal) == colorIdx.end())
                colorIdx[nodeVal] = (int)colorIdx.size() % 8;
            auto c1 = pal[colorIdx[nodeVal]][0];
            auto c2 = pal[colorIdx[nodeVal]][1];

            bool isTarget = (nodeVal == targetVal && i == targetBucket);
            if (isTarget){ c1 = sf::Color(255,170,170,230); c2 = sf::Color(255,200,185,230); }

            sf::Color nodeOutline = isTarget ? sf::Color(210,80,80,200) : sf::Color(160,180,220,180);

            drawRoundedRect(window, sf::FloatRect(sf::Vector2f(px,py), sf::Vector2f(nodeW,nodeH)), 10.f, c1, nodeOutline, 3.f);

            sf::RectangleShape tint(sf::Vector2f(nodeW*0.5f-2.f, nodeH-4.f));
            tint.setPosition(sf::Vector2f(px+nodeW*0.5f, py+2.f));
            tint.setFillColor(sf::Color(c2.r,c2.g,c2.b,120));
            window.draw(tint);

            sf::Text valText(mFontRegular, std::to_string(nodeVal), 14);
            valText.setFillColor(sf::Color(40,40,80));
            sf::FloatRect vb = valText.getLocalBounds();
            valText.setPosition(sf::Vector2f(px+(nodeW-vb.size.x)*0.5f-vb.position.x, py+(nodeH-vb.size.y)*0.5f-vb.position.y));
            window.draw(valText);

            sf::Color arrowCol = isTarget ? sf::Color(200,80,80,180) : sf::Color(140,170,210,180);
            if (depth == 0){
                drawArrowUp(window,
                    sf::Vector2f(bx+nodeW*0.5f, by),
                    sf::Vector2f(px+nodeW*0.5f, py+nodeH+1.f), arrowCol);
            } else {
                float prevPy = by - depth*gapY;
                drawArrowUp(window,
                    sf::Vector2f(px+nodeW*0.5f, prevPy),
                    sf::Vector2f(px+nodeW*0.5f, py+nodeH+1.f), arrowCol);
            }
        }
    }
}
// ============================================================
//  update  (mirrors RBTVisualizer::update exactly)
// ============================================================
void HashVisualizer::update(const std::optional<sf::Event>& event){
    sf::Vector2f worldPos = mWindow->mapPixelToCoords(sf::Mouse::getPosition(*mWindow));
    sf::Vector2i mousePos(static_cast<int>(worldPos.x), static_cast<int>(worldPos.y));

    mLayout.update(mousePos);
    if (mLayout.mGoHome){ mGoHome = true; mLayout.mGoHome = false; }

    if (mShowUndoBtn) mUndoBtn .update(mousePos);
    else              mClearBtn.update(mousePos);

    mNewBtn.update(mousePos);
    mInsertBtn.update(mousePos);
    mDeleteBtn.update(mousePos);
    mSearchBtn.update(mousePos);
    mSpeedSlider.update(mousePos);

    float speedMult = mSpeedSlider.getSpeed();
    // speed: steps per second.  We store ms-per-step for the autoplay clock.
    int speedMs = (speedMult > 0.f) ? static_cast<int>(500.f / speedMult) : 9999;

    mSkipBackBtn  .update(mousePos);
    mSkipForwardBtn.update(mousePos);

    if (mIsPaused){
        mStartBtn      .update(mousePos);
        mStepBackBtn   .update(mousePos);
        mStepForwardBtn.update(mousePos);
    } else {
        mPauseBtn.update(mousePos);
    }

    if (mIsInsertExpanded){ mConfirmAddBtn .update(mousePos); mInsertDiceBtn.update(mousePos); }
    if (mIsDeleteExpanded){ mConfirmRemoveBtn.update(mousePos); mDeleteDiceBtn.update(mousePos); }
    if (mIsSearchExpanded){ mConfirmSearchBtn.update(mousePos); mSearchDiceBtn.update(mousePos); }

    // ── Auto-play advance ──
    if (!mIsPaused){
        float durSec = (speedMult > 0.f) ? (1.f / speedMult) : 99999.f;
        if (mAutoPlayClock.getElapsedTime().asSeconds() >= durSec){
            int total = (int)mHash.getSteps().size();
            if (mCurrentStep < total - 1) mCurrentStep++;
            else mIsPaused = true;
            mAutoPlayClock.restart();
        }
    }

    if (!event) return;

    // ── Mouse clicks ──
    if (const auto* me = event->getIf<sf::Event::MouseButtonPressed>()){
        if (me->button == sf::Mouse::Button::Left){
            bool inside = false;

            // New button
            if (!mIsNewExpanded && sf::FloatRect({NEW_X,Y_NEW},{SB_W,SB_H}).contains(worldPos)){
                mIsNewExpanded = true;
                mIsInsertExpanded = mIsDeleteExpanded = mIsSearchExpanded = false;
                inside = true;
            }
            else if (mIsNewExpanded && sf::FloatRect({NEW_X-2,Y_NEW-2},{SB_W+4,SB_H*4}).contains(worldPos)){
                inside = true;
                if (sf::FloatRect({NEW_X+3.f, Y_NEW+SB_H+4.f},{SB_W-6.f,39.f}).contains(worldPos)){
                    // Random — no step animation, show result immediately
                    mHash.generateRandom(4 + rand() % 5);
                    mCurrentStep = std::max(0, (int)mHash.getSteps().size() - 1);
                    mTargetStep  = -1;
                    mIsPaused    = true;
                    mLayout.setDescription("Random hash table generated.");
                    mIsNewExpanded = false;
                }
                else if (sf::FloatRect({NEW_X+3.f, Y_NEW+SB_H+49.f},{SB_W-6.f,39.f}).contains(worldPos)){
                    // From file
                    const char* filters[1] = {"*.txt"};
                    const char* path = tinyfd_openFileDialog("Load hash table data","",1,filters,"Text Files (*.txt)",0);
                    if (path){ mHash.loadFromFile(path); mCurrentStep=0; mTargetStep=-1; mLayout.setDescription("Loaded from file."); }
                    else      { mLayout.setDescription("Cancelled."); }
                    mIsNewExpanded = false;
                    resetPlayUI();
                }
            }

            // Insert button
            else if (!mIsInsertExpanded && sf::FloatRect({SB_X,Y_INSERT},{SB_W,SB_H}).contains(worldPos)){
                mIsNewExpanded = false;
                mIsInsertExpanded = true; mIsDeleteExpanded = mIsSearchExpanded = false;
                mInputValue = ""; inside = true;
            }
            else if (mIsInsertExpanded && sf::FloatRect({SB_X,Y_INSERT},{SB_W*2+12.f,SB_H}).contains(worldPos)){
                inside = true;
                if (sf::FloatRect({SB_X+SB_W+20.f,Y_INSERT+3.f},{154.f,39.f}).contains(worldPos) && !mInputValue.empty()){
                    runAction(1, std::stoi(mInputValue));
                    mIsInsertExpanded = false; mInputValue = "";
                }
                else if (sf::FloatRect({SB_X+SB_W-4.f,Y_INSERT+7.f},{30.f,30.f}).contains(worldPos)){
                    mInputValue = std::to_string(rand()%999+1);
                }
            }

            // Delete button
            else if (!mIsDeleteExpanded && sf::FloatRect({SB_X,Y_DELETE},{SB_W,SB_H}).contains(worldPos)){
                mIsDeleteExpanded = true; mIsNewExpanded = mIsInsertExpanded = mIsSearchExpanded = false;
                mInputValue = ""; inside = true;
            }
            else if (mIsDeleteExpanded && sf::FloatRect({SB_X,Y_DELETE},{SB_W*2+12.f,SB_H}).contains(worldPos)){
                inside = true;
                if (sf::FloatRect({SB_X+SB_W+20.f,Y_DELETE+3.f},{154.f,39.f}).contains(worldPos) && !mInputValue.empty()){
                    runAction(2, std::stoi(mInputValue));
                    mIsDeleteExpanded = false; mInputValue = "";
                }
                else if (sf::FloatRect({SB_X+SB_W-4.f,Y_DELETE+7.f},{30.f,30.f}).contains(worldPos)){
                    mInputValue = std::to_string(rand()%999+1);
                }
            }

            // Search button
            else if (!mIsSearchExpanded && sf::FloatRect({SB_X,Y_SEARCH},{SB_W,SB_H}).contains(worldPos)){
                mIsSearchExpanded = true; mIsNewExpanded = mIsInsertExpanded = mIsDeleteExpanded = false;
                mInputValue = ""; inside = true;
            }
            else if (mIsSearchExpanded && sf::FloatRect({SB_X,Y_SEARCH},{SB_W*2+12.f,SB_H}).contains(worldPos)){
                inside = true;
                if (sf::FloatRect({SB_X+SB_W+20.f,Y_SEARCH+3.f},{154.f,39.f}).contains(worldPos) && !mInputValue.empty()){
                    runAction(3, std::stoi(mInputValue));
                    mIsSearchExpanded = false; mInputValue = "";
                }
                else if (sf::FloatRect({SB_X+SB_W-4.f,Y_SEARCH+7.f},{30.f,30.f}).contains(worldPos)){
                    mInputValue = std::to_string(rand()%999+1);
                }
            }

            if (!inside){
                mIsInsertExpanded = mIsDeleteExpanded = mIsSearchExpanded = mIsNewExpanded = false;
            }
        }
    }

    // ── Keyboard ──
    else if (const auto* ke = event->getIf<sf::Event::KeyPressed>()){
        // Ctrl+Z = undo
        if (ke->code == sf::Keyboard::Key::Z && (ke->control || ke->system)){
            mShowUndoBtn = false;
            mIsPaused = true;
            mIsInsertExpanded = mIsDeleteExpanded = mIsSearchExpanded = mIsNewExpanded = false;
            mInputValue = "";
        }
        else if (ke->code == sf::Keyboard::Key::I){
            mIsNewExpanded = false;
            mIsInsertExpanded = true; mIsDeleteExpanded = mIsSearchExpanded = false;
            mInputValue = "";
        }
        else if (ke->code == sf::Keyboard::Key::D){
            mIsNewExpanded = false;
            mIsDeleteExpanded = true; mIsInsertExpanded = mIsSearchExpanded = false;
            mInputValue = "";
        }
        else if (ke->code == sf::Keyboard::Key::S){
            mIsNewExpanded = false;
            mIsSearchExpanded = true; mIsInsertExpanded = mIsDeleteExpanded = false;
            mInputValue = "";
        }
        else if (ke->code == sf::Keyboard::Key::N){
            mIsNewExpanded = !mIsNewExpanded;
            mIsInsertExpanded = mIsDeleteExpanded = mIsSearchExpanded = false;
            mInputValue = "";
        }
        else if (ke->code == sf::Keyboard::Key::Space){
            mIsPaused = !mIsPaused;
            if (!mIsPaused) mAutoPlayClock.restart();
        }
        else if (ke->code == sf::Keyboard::Key::R){
            if (mIsInsertExpanded || mIsDeleteExpanded || mIsSearchExpanded){
                mInputValue = std::to_string(rand()%999+1);
            } else {
                mHash.generateRandom(4 + rand() % 5);
                mCurrentStep = std::max(0, (int)mHash.getSteps().size() - 1);
                mTargetStep  = -1;
                mIsPaused    = true;
                mLayout.setDescription("Random hash table generated.");
                mIsNewExpanded = false;
            }
        }
        else if (mIsInsertExpanded || mIsDeleteExpanded || mIsSearchExpanded){
            if (ke->code == sf::Keyboard::Key::Backspace && !mInputValue.empty()){
                mInputValue.pop_back();
            }
            else if (ke->code == sf::Keyboard::Key::Enter && !mInputValue.empty()){
                int val = std::stoi(mInputValue);
                if      (mIsInsertExpanded) runAction(1, val);
                else if (mIsDeleteExpanded) runAction(2, val);
                else if (mIsSearchExpanded) runAction(3, val);
                mIsInsertExpanded = mIsDeleteExpanded = mIsSearchExpanded = false;
                mInputValue = "";
            }
            else {
                int digit = -1;
                if (ke->code >= sf::Keyboard::Key::Num0 && ke->code <= sf::Keyboard::Key::Num9)
                    digit = (int)ke->code - (int)sf::Keyboard::Key::Num0;
                else if (ke->code >= sf::Keyboard::Key::Numpad0 && ke->code <= sf::Keyboard::Key::Numpad9)
                    digit = (int)ke->code - (int)sf::Keyboard::Key::Numpad0;
                if (digit != -1 && mInputValue.size() < 4)
                    mInputValue += std::to_string(digit);
            }
        }
    }
}

// ============================================================
//  render  (mirrors RBTVisualizer::render)
// ============================================================
void HashVisualizer::render(bool showUI){
    auto steps = mHash.getSteps();
    int total  = (int)steps.size();

    // ── Step animation advance (same logic as RBT) ──
    if (mTargetStep != -1 && mTargetStep != mCurrentStep){
        mStepAnimProgress += 1.f / 30.f;   // ~30 frames per step transition
        if (mStepAnimProgress >= 1.f){
            mStepAnimProgress = 1.f;
            mCurrentStep = mTargetStep;
            if (!mIsPaused && mCurrentStep < total - 1){
                mTargetStep = mCurrentStep + 1;
                mStepAnimProgress = 0.f;
            } else {
                mTargetStep = -1;
            }
        }
    }
    else if (!mIsPaused && showUI && mCurrentStep < total-1 && mTargetStep == -1){
        // autoplay driven by mAutoPlayClock in update(); nothing extra here
    }

    // Clamp
    if (mCurrentStep >= total && total > 0) mCurrentStep = total - 1;
    if (mCurrentStep < 0) mCurrentStep = 0;

    if (!showUI) return;

    // ── Background ──
    sf::RectangleShape bg(sf::Vector2f(1440.f, FOOTER_Y));
    bg.setPosition(sf::Vector2f(0.f, 0.f));
    bg.setFillColor(sf::Color(242,247,255,255));
    mWindow->draw(bg);

    // ── Hash visualization ──
    renderBuckets(*mWindow);
    renderNodes  (*mWindow);

    // ── Step description → AppLayout description box ──
    if (mCurrentStep >= 0 && mCurrentStep < total)
        mLayout.setDescription(steps[mCurrentStep].description);

    // ── AppLayout (footer + home btn + description) ──
    mLayout.draw(*mWindow);

    // ── Title ──
    mWindow->draw(mTitleNum);
    mWindow->draw(mTitleHash);
    mWindow->draw(mTitleChain);

    // ── Sidebar ──
    sf::Vector2f worldPos = mWindow->mapPixelToCoords(sf::Mouse::getPosition(*mWindow));

    if (mShowUndoBtn) mUndoBtn .draw(*mWindow);
    else              mClearBtn.draw(*mWindow);

    // New button
    if (mIsNewExpanded){
        mNewExpandedStroke.draw(*mWindow);
        mNewExpandedBg    .draw(*mWindow);
        mNewBtn   .draw(*mWindow);
        mRandomBtn.draw(*mWindow);
        mUploadBtn.draw(*mWindow);
    } else {
        if (sf::FloatRect({NEW_X,Y_NEW},{SB_W,SB_H}).contains(worldPos))
            mNewHoverStroke.draw(*mWindow);
        mNewBtn.draw(*mWindow);
    }

    // Insert button
    if (mIsInsertExpanded){
        mInsertExpandedStroke.draw(*mWindow);
        mInsertExpandedBg    .draw(*mWindow);
        mConfirmAddBtn       .draw(*mWindow);
        mInsertDiceBtn       .draw(*mWindow);
        mInsertInputText.setString(mInputValue);
        mWindow->draw(mInsertInputText);
        if (mCursorClock.getElapsedTime().asSeconds() >= 0.5f){
            mShowCursor = !mShowCursor; mCursorClock.restart();
        }
        if (mShowCursor){
            float tw = mInsertInputText.getLocalBounds().size.x;
            mInsertCursorLine.setPosition(sf::Vector2f(mInputValue.empty() ? SB_X+8.f : SB_X+8.f+tw+2.f, Y_INSERT+12.f));
            mWindow->draw(mInsertCursorLine);
        }
    } else {
        if (sf::FloatRect({SB_X,Y_INSERT},{SB_W,SB_H}).contains(worldPos))
            mInsertHoverStroke.draw(*mWindow);
        mInsertBtn.draw(*mWindow);
    }

    // Delete button
    if (mIsDeleteExpanded){
        mDeleteExpandedStroke.draw(*mWindow);
        mDeleteExpandedBg    .draw(*mWindow);
        mConfirmRemoveBtn    .draw(*mWindow);
        mDeleteDiceBtn       .draw(*mWindow);
        mDeleteInputText.setString(mInputValue);
        mWindow->draw(mDeleteInputText);
        if (mCursorClock.getElapsedTime().asSeconds() >= 0.5f){
            mShowCursor = !mShowCursor; mCursorClock.restart();
        }
        if (mShowCursor){
            float tw = mDeleteInputText.getLocalBounds().size.x;
            mDeleteCursorLine.setPosition(sf::Vector2f(mInputValue.empty() ? SB_X+8.f : SB_X+8.f+tw+2.f, Y_DELETE+12.f));
            mWindow->draw(mDeleteCursorLine);
        }
    } else {
        if (sf::FloatRect({SB_X,Y_DELETE},{SB_W,SB_H}).contains(worldPos))
            mDeleteHoverStroke.draw(*mWindow);
        mDeleteBtn.draw(*mWindow);
    }

    // Search button
    if (mIsSearchExpanded){
        mSearchExpandedStroke.draw(*mWindow);
        mSearchExpandedBg    .draw(*mWindow);
        mConfirmSearchBtn    .draw(*mWindow);
        mSearchDiceBtn       .draw(*mWindow);
        mSearchInputText.setString(mInputValue);
        mWindow->draw(mSearchInputText);
        if (mCursorClock.getElapsedTime().asSeconds() >= 0.5f){
            mShowCursor = !mShowCursor; mCursorClock.restart();
        }
        if (mShowCursor){
            float tw = mSearchInputText.getLocalBounds().size.x;
            mSearchCursorLine.setPosition(sf::Vector2f(mInputValue.empty() ? SB_X+8.f : SB_X+8.f+tw+2.f, Y_SEARCH+12.f));
            mWindow->draw(mSearchCursorLine);
        }
    } else {
        if (sf::FloatRect({SB_X,Y_SEARCH},{SB_W,SB_H}).contains(worldPos))
            mSearchHoverStroke.draw(*mWindow);
        mSearchBtn.draw(*mWindow);
    }

    // ── Speed slider ──
    mSpeedSlider.draw(*mWindow);

    // ── Skip back / forward (always visible) ──
    mSkipBackBtn  .draw(*mWindow);
    mSkipForwardBtn.draw(*mWindow);

    // ── Step pill + step/play controls ──
    if (mIsPaused){
        mWindow->draw(mStepBgLeft);
        mWindow->draw(mStepBgRight);
        mWindow->draw(mStepBgCenter);

        if (total == 0) mStepText.setString("Step 0 / 0");
        else            mStepText.setString("Step " + std::to_string(mCurrentStep+1)
                                          + " / "  + std::to_string(total));
        mWindow->draw(mStepText);

        mStartBtn      .draw(*mWindow);
        mStepBackBtn   .draw(*mWindow);
        mStepForwardBtn.draw(*mWindow);
    } else {
        mPauseBtn.draw(*mWindow);
    }
}