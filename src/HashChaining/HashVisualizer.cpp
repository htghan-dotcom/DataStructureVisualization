#include "HashChaining/HashVisualizer.h"
#include "tinyfiledialogs.h"
#include "Common.h"
#include <cmath>
#include <cstdlib>

// ── Layout constants (sidebar) ──────────────────────────────
static constexpr float SB_X  = 70.f;
static constexpr float SB_W  = 160.f;
static constexpr float SB_H  = 45.f;
static constexpr float SB_R  = 21.f;
static constexpr float Y_CLEAR  = 176.f;
static constexpr float Y_NEW    = 176.f;
static constexpr float Y_INSERT = 230.f;
static constexpr float Y_DELETE = 284.f;
static constexpr float Y_SEARCH = 338.f;
static constexpr float NEW_X    = 251.f;
static constexpr float FOOTER_Y = 882.f;

// ── Pastel palette ───────────────────────────────────────────
static const sf::Color PAL[][2] = {
    {{188,214,245,220},{210,230,255,220}},
    {{245,188,210,220},{255,210,230,220}},
    {{188,240,210,220},{210,255,230,220}},
    {{245,235,155,220},{255,248,190,220}},
    {{220,195,245,220},{238,220,255,220}},
    {{245,210,180,220},{255,230,205,220}},
    {{175,230,240,220},{205,245,255,220}},
    {{240,200,200,220},{255,225,225,220}},
};



// ============================================================
//  Constructor
// ============================================================
HashVisualizer::HashVisualizer(sf::RenderWindow& window)
    : mWindow(&window),
      mHash(7),

      mTitleNum  (mFontBold),
      mTitleHash (mFontBold),
      mTitleChain(mFontBold),

      mClearBtn (mFontRegular, "Clear Table",   SB_X,  Y_CLEAR,  SB_W, SB_H, SB_R, sf::Color(217,217,217)),
      mNewBtn   (mFontRegular, "New Table",     NEW_X, Y_NEW,    SB_W, SB_H, SB_R, sf::Color(217,217,217)),
      mInsertBtn(mFontRegular, "Insert",  SB_X,  Y_INSERT, SB_W, SB_H, SB_R, sf::Color(217,217,217)),
      mDeleteBtn(mFontRegular, "Delete",  SB_X,  Y_DELETE, SB_W, SB_H, SB_R, sf::Color(217,217,217)),
      mSearchBtn(mFontRegular, "Search",  SB_X,  Y_SEARCH, SB_W, SB_H, SB_R, sf::Color(217,217,217)),
      mUndoBtn  (mFontRegular, "Undo",    SB_X,  Y_CLEAR,  SB_W, SB_H, SB_R, sf::Color(217,217,217)),

      // Insert expand
      mInsertHoverStroke   (mFontRegular,"",SB_X-2, Y_INSERT-2, SB_W+4,        SB_H+4,       23.f, sf::Color(90,150,44)),
      mInsertExpandedStroke(mFontRegular,"",SB_X-2, Y_INSERT-2, 345.f,         SB_H+4,       23.f, sf::Color(90,150,44)),
      mInsertExpandedBg    (mFontRegular,"",SB_X,   Y_INSERT,   341.f,         SB_H,         21.f, sf::Color(217,217,217)),
      mConfirmAddBtn(mFontRegular, "Add", 254.f, 233.f, 154.f, 39.f, 19.5f, sf::Color(245, 245, 240)),
      mInsertInputText     (mFontRegular),

      // Delete expand
      mDeleteHoverStroke   (mFontRegular,"",SB_X-2, Y_DELETE-2, SB_W+4,        SB_H+4,       23.f, sf::Color(90,150,44)),
      mDeleteExpandedStroke(mFontRegular,"",SB_X-2, Y_DELETE-2, 345.f, SB_H+4,       23.f, sf::Color(90,150,44)),
      mDeleteExpandedBg    (mFontRegular,"",SB_X,   Y_DELETE,   341.f,    SB_H,         21.f, sf::Color(217,217,217)),
      mConfirmRemoveBtn(mFontRegular, "Remove", 254.f, 287.f, 154.f, 39.f, 19.5f, sf::Color(245, 245, 240)),
      mDeleteInputText     (mFontRegular),

      // Search expand
      mSearchHoverStroke   (mFontRegular,"",SB_X-2, Y_SEARCH-2, SB_W+4,        SB_H+4,       23.f, sf::Color(90,150,44)),
      mSearchExpandedStroke(mFontRegular,"",SB_X-2, Y_SEARCH-2, 345.f, SB_H+4,       23.f, sf::Color(90,150,44)),
      mSearchExpandedBg    (mFontRegular,"",SB_X,   Y_SEARCH,   341.f,    SB_H,         21.f, sf::Color(217,217,217)),
      mConfirmSearchBtn(mFontRegular, "Search", 254.f, 341.f, 154.f, 39.f, 19.5f, sf::Color(245, 245, 240)),
      mSearchInputText     (mFontRegular),

      // New expand
      mNewHoverStroke(mFontRegular, "", 249.f, 174.f, 164.f, 49.f, 23.f, sf::Color(90, 150, 44)),
      mNewExpandedStroke(mFontRegular, "", 249.f, 174.f, 164.f, 157.f, 23.f, sf::Color(90, 150, 44)),
      mNewExpandedBg(mFontRegular, "", 251.f, 176.f, 160.f, 153.f, 21.f, sf::Color(217, 217, 217)),
      mRandomBtn(mFontRegular, "Random", 254.f, 233.f, 154.f, 39.f, 19.5f, sf::Color(245, 245, 240)),
      mUploadBtn(mFontRegular, "From File", 254.f, 287.f, 154.f, 39.f, 19.5f, sf::Color(245, 245, 240))
{
    if (!mFontBold.openFromFile("assets/fonts/Inter-Bold.ttf") ||
        !mFontRegular.openFromFile("assets/fonts/Inter-Regular.ttf"))
        cerr << "HashVisualizer: cannot load fonts" << endl;

    if (!mDiceTex.loadFromFile("assets/images/randomButton.png"))
        cerr << "Cannot load randomButton.png" << endl;
    mDiceTex.setSmooth(true); mDiceTex.generateMipmap();

    // Dice buttons
    mInsertDiceBtn.setup(mDiceTex, 205.f, 237.f, 30.f, 30.f);
    mDeleteDiceBtn.setup(mDiceTex, 205.f, 291.f, 30.f, 30.f);
    mSearchDiceBtn.setup(mDiceTex, 205.f, 345.f, 30.f, 30.f);

    // Input texts
    auto setupInput = [&](sf::Text& t, float y){
        t.setCharacterSize(22);
        t.setFillColor(sf::Color::Black);
        t.setPosition(sf::Vector2f(85.f, y+8.f));
    };
    setupInput(mInsertInputText, Y_INSERT);
    setupInput(mDeleteInputText, Y_DELETE);
    setupInput(mSearchInputText, Y_SEARCH);

    // Cursor lines
    for (auto* c : {&mInsertCursorLine,&mDeleteCursorLine,&mSearchCursorLine}){
        c->setSize(sf::Vector2f(2.f,24.f));
        c->setFillColor(sf::Color::Black);
    }

    // Title
    mTitleNum.setString("02");
    mTitleNum.setCharacterSize(30);
    mTitleNum.setPosition(sf::Vector2f(67.f, 663.f));

    mTitleHash.setString("Hash");
    mTitleHash.setCharacterSize(70);
    mTitleHash.setPosition(sf::Vector2f(67.f, 695.f));

    mTitleChain.setString("Table");
    mTitleChain.setCharacterSize(70);
    mTitleChain.setPosition(sf::Vector2f(67.f, 765.f));

    for (auto* b : {&mClearBtn,&mNewBtn,&mInsertBtn,&mDeleteBtn,&mSearchBtn,&mUndoBtn,
                    &mConfirmAddBtn,&mConfirmRemoveBtn,&mConfirmSearchBtn,
                    &mRandomBtn,&mUploadBtn})
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
        mHash.clear();
        mHash.generateRandom(0);
        mCurrentStep = std::max(0, (int)mHash.getSteps().size()-1);
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
    mHash.generateRandom(4 + rand() % 5);
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
    mBucketW=80.f; mBucketH=58.f; mBucketGapX=88.f;
    float totalW=n*mBucketGapX-(mBucketGapX-mBucketW);
    mBucketStartX=(1440.f-totalW)*0.5f;
    mBucketRowY=FOOTER_Y-mBucketH-55.f;

    for (int i=0;i<n;++i){
        float px=mBucketStartX+i*mBucketGapX, py=mBucketRowY;
        bool isTarget=(i==targetBucket);

        sf::Color fill  =isTarget?sf::Color(255,235,180,230):sf::Color(195,218,240,200);
        sf::Color border=isTarget?sf::Color(220,150, 60,220):sf::Color(130,170,210,200);

        // Tint with theme primary when targeted
        if (isTarget) border=ThemeManager::current.primary;

        drawRoundedRect(window,sf::FloatRect(sf::Vector2f(px,py),sf::Vector2f(mBucketW,mBucketH)),10.f,fill,border,3.5f);

        sf::Text lbl(mFontRegular,"H"+std::to_string(i),18);
        lbl.setFillColor(sf::Color(60,80,130));
        sf::FloatRect lb=lbl.getLocalBounds();
        lbl.setPosition(sf::Vector2f(px+(mBucketW-lb.size.x)*0.5f-lb.position.x,
                                     py+(mBucketH-lb.size.y)*0.5f-lb.position.y));
        window.draw(lbl);

        sf::Text idx(mFontRegular,std::to_string(i),11);
        idx.setFillColor(sf::Color(120,140,180));
        sf::FloatRect ib=idx.getLocalBounds();
        idx.setPosition(sf::Vector2f(px+(mBucketW-ib.size.x)*0.5f-ib.position.x,py+mBucketH+4.f));
        window.draw(idx);
    }
}

// ── renderNodes ──────────────────────────────────────────────
void HashVisualizer::renderNodes(sf::RenderWindow& window){
    auto steps=mHash.getSteps();
    if (steps.empty()) return;

    int si=std::max(0,std::min(mCurrentStep,(int)steps.size()-1));
    const auto& step=steps[si];
    const auto& snap=step.tableSnapshot;
    if (snap.empty()) return;

    int targetBucket=step.targetBucket;
    int targetVal   =step.targetVal;

    float nodeW=mBucketW, nodeH=46.f, gapY=72.f;

    for (int i=0;i<(int)snap.size();++i){
        float bx=mBucketStartX+i*mBucketGapX, by=mBucketRowY;
        const auto& chain=snap[i].vals;
        for (int depth=0;depth<(int)chain.size();++depth){
            int nv=chain[depth];
            float px=bx, py=by-(depth+1)*gapY;

            if (mColorIdx.find(nv)==mColorIdx.end())
                mColorIdx[nv]=(int)mColorIdx.size()%8;
            auto c1=PAL[mColorIdx[nv]][0];
            auto c2=PAL[mColorIdx[nv]][1];

            bool isTarget=(nv==targetVal&&i==targetBucket);
            if (isTarget){ c1=sf::Color(255,170,170,230); c2=sf::Color(255,200,185,230); }

            sf::Color outline=isTarget?ThemeManager::current.primary:sf::Color(160,180,220,180);
            drawRoundedRect(window,sf::FloatRect(sf::Vector2f(px,py),sf::Vector2f(nodeW,nodeH)),10.f,c1,outline,3.f);

            sf::RectangleShape tint(sf::Vector2f(nodeW*0.5f-2.f,nodeH-4.f));
            tint.setPosition(sf::Vector2f(px+nodeW*0.5f,py+2.f));
            tint.setFillColor(sf::Color(c2.r,c2.g,c2.b,120));
            window.draw(tint);

            sf::Text valText(mFontRegular,std::to_string(nv),14);
            valText.setFillColor(sf::Color(40,40,80));
            sf::FloatRect vb=valText.getLocalBounds();
            valText.setPosition(sf::Vector2f(px+(nodeW-vb.size.x)*0.5f-vb.position.x,
                                             py+(nodeH-vb.size.y)*0.5f-vb.position.y));
            window.draw(valText);

            sf::Color arrowCol=isTarget?ThemeManager::current.primary:sf::Color(140,170,210,180);
            if (depth==0)
                drawArrowUp(window,sf::Vector2f(bx+nodeW*0.5f,by),sf::Vector2f(px+nodeW*0.5f,py+nodeH+1.f),arrowCol);
            else
                drawArrowUp(window,sf::Vector2f(px+nodeW*0.5f,by-depth*gapY),sf::Vector2f(px+nodeW*0.5f,py+nodeH+1.f),arrowCol);
        }
    }
}

// ── update ───────────────────────────────────────────────────
void HashVisualizer::update(const std::optional<sf::Event>& event){
    sf::Vector2f worldPos=mWindow->mapPixelToCoords(sf::Mouse::getPosition(*mWindow));
    sf::Vector2i mousePos(static_cast<int>(worldPos.x),static_cast<int>(worldPos.y));

    mLayout.update(mousePos);
    if (mLayout.mGoHome){ mGoHome=true; mLayout.mGoHome=false; }

    if (mShowUndoBtn) mUndoBtn .update(mousePos);
    else              mClearBtn.update(mousePos);
    mNewBtn.update(mousePos);
    mInsertBtn.update(mousePos);
    mDeleteBtn.update(mousePos);
    mSearchBtn.update(mousePos);

    if (mIsInsertExpanded){ mConfirmAddBtn  .update(mousePos); mInsertDiceBtn.update(mousePos); }
    if (mIsDeleteExpanded){ mConfirmRemoveBtn.update(mousePos); mDeleteDiceBtn.update(mousePos); }
    if (mIsSearchExpanded){ mConfirmSearchBtn.update(mousePos); mSearchDiceBtn.update(mousePos); }

    // Auto-play
    // if (!mLayout.isPaused()){
    //     float dur=1.f/std::max(0.1f,mLayout.getSpeed());
    //     int total=(int)mHash.getSteps().size();
    //     if (mAutoPlayClock.getElapsedTime().asSeconds()>=dur){
    //         if (mCurrentStep<total-1) mCurrentStep++;
    //         else mLayout.setPaused(true);
    //         mAutoPlayClock.restart();
    //     }
    // }

    if (!event) return;

    if (const auto* me=event->getIf<sf::Event::MouseButtonPressed>()){
        if (me->button==sf::Mouse::Button::Left){
            bool inside=false;

            if (!mIsNewExpanded&&sf::FloatRect({NEW_X,Y_NEW},{SB_W,SB_H}).contains(worldPos)){
                mIsNewExpanded=true; mIsInsertExpanded=mIsDeleteExpanded=mIsSearchExpanded=false;
                inside=true;
            }
            else if (mIsNewExpanded&&sf::FloatRect({NEW_X-2,Y_NEW-2},{SB_W+4,SB_H*4}).contains(worldPos)){
                inside=true;
                if (sf::FloatRect({NEW_X+3.f,Y_NEW+SB_H+4.f},{SB_W-6.f,39.f}).contains(worldPos)){
                    doRandom(); mIsNewExpanded=false;
                }
                else if (sf::FloatRect({NEW_X+3.f,Y_NEW+SB_H+49.f},{SB_W-6.f,39.f}).contains(worldPos)){
                    const char* f[1]={"*.txt"};
                    const char* p=tinyfd_openFileDialog("Load hash table data","",1,f,"Text Files (*.txt)",0);
                    if (p){ mHash.loadFromFile(p); mCurrentStep=0; mTargetStep=-1; mLayout.setDescription("Loaded from file."); mLayout.setPaused(false); mAutoPlayClock.restart(); }
                    else mLayout.setDescription("Cancelled.");
                    mIsNewExpanded=false;
                }
            }
            else if (!mIsInsertExpanded&&sf::FloatRect({SB_X,Y_INSERT},{SB_W,SB_H}).contains(worldPos)){
                mIsInsertExpanded=true; mIsNewExpanded=mIsDeleteExpanded=mIsSearchExpanded=false;
                mInputValue=""; inside=true;
            }
            else if (mIsInsertExpanded&&sf::FloatRect({SB_X,Y_INSERT},{SB_W*2+12.f,SB_H}).contains(worldPos)){
                inside=true;
                if (sf::FloatRect({SB_X+SB_W+20.f,Y_INSERT+3.f},{154.f,39.f}).contains(worldPos)&&!mInputValue.empty()){
                    runAction(1,std::stoi(mInputValue)); mIsInsertExpanded=false; mInputValue="";
                }
                else if (sf::FloatRect({205.f,Y_INSERT+7.f},{30.f,30.f}).contains(worldPos))
                    mInputValue=std::to_string(rand()%999+1);
            }
            else if (!mIsDeleteExpanded&&sf::FloatRect({SB_X,Y_DELETE},{SB_W,SB_H}).contains(worldPos)){
                mIsDeleteExpanded=true; mIsNewExpanded=mIsInsertExpanded=mIsSearchExpanded=false;
                mInputValue=""; inside=true;
            }
            else if (mIsDeleteExpanded&&sf::FloatRect({SB_X,Y_DELETE},{SB_W*2+12.f,SB_H}).contains(worldPos)){
                inside=true;
                if (sf::FloatRect({SB_X+SB_W+20.f,Y_DELETE+3.f},{154.f,39.f}).contains(worldPos)&&!mInputValue.empty()){
                    runAction(2,std::stoi(mInputValue)); mIsDeleteExpanded=false; mInputValue="";
                }
                else if (sf::FloatRect({205.f,Y_DELETE+7.f},{30.f,30.f}).contains(worldPos))
                    mInputValue=std::to_string(rand()%999+1);
            }
            else if (!mIsSearchExpanded&&sf::FloatRect({SB_X,Y_SEARCH},{SB_W,SB_H}).contains(worldPos)){
                mIsSearchExpanded=true; mIsNewExpanded=mIsInsertExpanded=mIsDeleteExpanded=false;
                mInputValue=""; inside=true;
            }
            else if (mIsSearchExpanded&&sf::FloatRect({SB_X,Y_SEARCH},{SB_W*2+12.f,SB_H}).contains(worldPos)){
                inside=true;
                if (sf::FloatRect({SB_X+SB_W+20.f,Y_SEARCH+3.f},{154.f,39.f}).contains(worldPos)&&!mInputValue.empty()){
                    runAction(3,std::stoi(mInputValue)); mIsSearchExpanded=false; mInputValue="";
                }
                else if (sf::FloatRect({205.f,Y_SEARCH+7.f},{30.f,30.f}).contains(worldPos))
                    mInputValue=std::to_string(rand()%999+1);
            }

            if (!inside)
                mIsInsertExpanded=mIsDeleteExpanded=mIsSearchExpanded=mIsNewExpanded=false;
        }
    }
    else if (const auto* ke=event->getIf<sf::Event::KeyPressed>()){
        if (ke->code==sf::Keyboard::Key::Z&&(ke->control||ke->system)){
            mShowUndoBtn=false; mLayout.setPaused(true);
            mIsInsertExpanded=mIsDeleteExpanded=mIsSearchExpanded=mIsNewExpanded=false;
            mInputValue="";
        }
        else if (ke->code==sf::Keyboard::Key::I){
            mIsInsertExpanded=true; mIsNewExpanded=mIsDeleteExpanded=mIsSearchExpanded=false; mInputValue="";
        }
        else if (ke->code==sf::Keyboard::Key::D){
            mIsDeleteExpanded=true; mIsNewExpanded=mIsInsertExpanded=mIsSearchExpanded=false; mInputValue="";
        }
        else if (ke->code==sf::Keyboard::Key::S){
            mIsSearchExpanded=true; mIsNewExpanded=mIsInsertExpanded=mIsDeleteExpanded=false; mInputValue="";
        }
        else if (ke->code==sf::Keyboard::Key::N){
            mIsNewExpanded=!mIsNewExpanded;
            mIsInsertExpanded=mIsDeleteExpanded=mIsSearchExpanded=false; mInputValue="";
        }
        else if (ke->code==sf::Keyboard::Key::Space){
            mLayout.setPaused(!mLayout.isPaused());
            if (!mLayout.isPaused()) mAutoPlayClock.restart();
        }
        else if (ke->code==sf::Keyboard::Key::R){
            if (mIsInsertExpanded||mIsDeleteExpanded||mIsSearchExpanded)
                mInputValue=std::to_string(rand()%999+1);
            else{ doRandom(); mIsNewExpanded=false; }
        }
        else if (mIsInsertExpanded||mIsDeleteExpanded||mIsSearchExpanded){
            if (ke->code==sf::Keyboard::Key::Backspace&&!mInputValue.empty())
                mInputValue.pop_back();
            else if (ke->code==sf::Keyboard::Key::Enter&&!mInputValue.empty()){
                int val=std::stoi(mInputValue);
                if      (mIsInsertExpanded) runAction(1,val);
                else if (mIsDeleteExpanded) runAction(2,val);
                else if (mIsSearchExpanded) runAction(3,val);
                mIsInsertExpanded=mIsDeleteExpanded=mIsSearchExpanded=false; mInputValue="";
            }
            else{
                int digit=-1;
                if (ke->code>=sf::Keyboard::Key::Num0&&ke->code<=sf::Keyboard::Key::Num9)
                    digit=(int)ke->code-(int)sf::Keyboard::Key::Num0;
                else if (ke->code>=sf::Keyboard::Key::Numpad0&&ke->code<=sf::Keyboard::Key::Numpad9)
                    digit=(int)ke->code-(int)sf::Keyboard::Key::Numpad0;
                if (digit!=-1&&mInputValue.size()<4) mInputValue+=std::to_string(digit);
            }
        }
    }
}

// ── render ───────────────────────────────────────────────────
void HashVisualizer::render(bool showUI){
    auto steps=mHash.getSteps();
    int total=(int)steps.size();

    // // Step animation
    // if (mTargetStep!=-1&&mTargetStep!=mCurrentStep){
    //     mStepAnimProgress+=1.f/30.f;
    //     if (mStepAnimProgress>=1.f){
    //         mCurrentStep=mTargetStep;
    //         if (!mLayout.isPaused()&&mCurrentStep<total-1){
    //             mTargetStep=mCurrentStep+1; mStepAnimProgress=0.f;
    //         } else {
    //             mTargetStep=-1; mStepAnimProgress=1.f;
    //         }
    //     }
    // }
    // if (mCurrentStep>=total&&total>0) mCurrentStep=total-1;
    // if (mCurrentStep<0) mCurrentStep=0;

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
    if (!showUI) return;

    if (mCurrentStep >= total && total > 0) mCurrentStep = total - 1;
    if (mCurrentStep < 0) mCurrentStep = 0;

    // // Background
    // sf::RectangleShape bg(sf::Vector2f(1440.f,FOOTER_Y));
    // bg.setPosition(sf::Vector2f(0.f,0.f));
    // bg.setFillColor(ThemeManager::current.screenBg);
    // mWindow->draw(bg);

    renderBuckets(*mWindow);
    renderNodes  (*mWindow);

    // Step text
    if (total==0) mLayout.setStepText("Step 0 / 0");
    else          mLayout.setStepText("Step "+std::to_string(mCurrentStep+1)+" / "+std::to_string(total));

    // Description from current step
    if (mCurrentStep>=0&&mCurrentStep<total)
        mLayout.setDescription(steps[mCurrentStep].description);

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
    for (auto* b:{&mClearBtn,&mNewBtn,&mInsertBtn,&mDeleteBtn,&mSearchBtn,&mUndoBtn,
                  &mInsertHoverStroke,&mInsertExpandedStroke,
                  &mDeleteHoverStroke,&mDeleteExpandedStroke,
                  &mSearchHoverStroke,&mSearchExpandedStroke,
                  &mNewHoverStroke,&mNewExpandedStroke})
        b->setThemeColor(sbCol);
    for (auto* b:{&mInsertHoverStroke,&mInsertExpandedStroke,
                  &mDeleteHoverStroke,&mDeleteExpandedStroke,
                  &mSearchHoverStroke,&mSearchExpandedStroke,
                  &mNewHoverStroke,&mNewExpandedStroke})
        b->setThemeColor(hlCol);
    for (auto* b:{&mInsertExpandedBg,&mDeleteExpandedBg,&mSearchExpandedBg,&mNewExpandedBg})
        b->setThemeColor(sbCol);
    for (auto* b:{&mConfirmAddBtn,&mConfirmRemoveBtn,&mConfirmSearchBtn,&mRandomBtn,&mUploadBtn})
        b->setThemeColor(ThemeManager::current.bg);

    sf::Vector2f worldPos=mWindow->mapPixelToCoords(sf::Mouse::getPosition(*mWindow));

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
        if (sf::FloatRect({NEW_X,Y_NEW},{SB_W,SB_H}).contains(worldPos)) mNewHoverStroke.draw(*mWindow);
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
            float cursorX = mInputValue.empty() ? 85.f : 85.f + tw + 2.f;
            mInsertCursorLine.setPosition(sf::Vector2f(cursorX, 240.f));
            mWindow->draw(mInsertCursorLine);
        }
    } else {
        if (sf::FloatRect({SB_X,Y_INSERT},{SB_W,SB_H}).contains(worldPos)) mInsertHoverStroke.draw(*mWindow);
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
            float cursorX = mInputValue.empty() ? 85.f : 85.f + tw + 2.f;
            mDeleteCursorLine.setPosition(sf::Vector2f(cursorX, 294.f));
            mWindow->draw(mDeleteCursorLine);
        }
    } else {
        if (sf::FloatRect({SB_X,Y_DELETE},{SB_W,SB_H}).contains(worldPos)) mDeleteHoverStroke.draw(*mWindow);
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
            float cursorX = mInputValue.empty() ? 85.f : 85.f + tw + 2.f;
            mSearchCursorLine.setPosition(sf::Vector2f(cursorX, 348.f));
            mWindow->draw(mSearchCursorLine);
        }
    } else {
        if (sf::FloatRect({SB_X,Y_SEARCH},{SB_W,SB_H}).contains(worldPos)) mSearchHoverStroke.draw(*mWindow);
        mSearchBtn.draw(*mWindow);
    }
}