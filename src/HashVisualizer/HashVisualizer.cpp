#include "HashVisualizer.h"
#include <cmath>
#include <sstream>
#include <iomanip>
#include <algorithm>

// ============================================================================
// Draw Helpers
// ============================================================================

// Draw a filled rounded rectangle, then inset border
void HashVisualizer::drawRoundedRect(sf::RenderWindow& window, const sf::FloatRect& rect,
                                      float radius, const sf::Color& fill,
                                      const sf::Color& border, float borderThick) {
    auto drawFill = [&](const sf::FloatRect& r, float rad, const sf::Color& col) {
        if (r.width <= 0 || r.height <= 0) return;
        float cr = std::min(rad, std::min(r.width, r.height) * 0.5f);

        // Center cross
        sf::RectangleShape h(sf::Vector2f(r.width - 2*cr, r.height));
        h.setPosition(r.left + cr, r.top);
        h.setFillColor(col);
        window.draw(h);

        sf::RectangleShape v(sf::Vector2f(r.width, r.height - 2*cr));
        v.setPosition(r.left, r.top + cr);
        v.setFillColor(col);
        window.draw(v);

        // 4 corners
        sf::CircleShape c(cr);
        c.setFillColor(col);
        c.setPosition(r.left,                   r.top);                   window.draw(c);
        c.setPosition(r.left + r.width - 2*cr,  r.top);                   window.draw(c);
        c.setPosition(r.left,                   r.top + r.height - 2*cr); window.draw(c);
        c.setPosition(r.left + r.width - 2*cr,  r.top + r.height - 2*cr); window.draw(c);
    };

    drawFill(rect, radius, border);
    if (borderThick > 0.f) {
        sf::FloatRect inner(rect.left + borderThick, rect.top + borderThick,
                            std::max(0.f, rect.width  - borderThick * 2),
                            std::max(0.f, rect.height - borderThick * 2));
        drawFill(inner, std::max(0.f, radius - borderThick), fill);
    }
}

void HashVisualizer::drawArrowUp(sf::RenderWindow& window, sf::Vector2f from,
                                 sf::Vector2f to, const sf::Color& color) {
    sf::Vector2f dir = to - from;
    float currentLen = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (currentLen < 1.f) return;
    dir /= currentLen;

    sf::Vector2f perp(-dir.y, dir.x);

    // --- CHỈNH Ở ĐÂY ---
    float arrowSize = 8.f;   // Giảm từ 12 xuống 8 để đầu mũi tên nhỏ xinh hơn
    float arrowWidth = 8.f;  // Giảm độ rộng đáy tam giác cho cân đối
    float lineThickness = 2.f; // Tăng độ dày của thân mũi tên lên cho đậm

    // 1. Vẽ thân mũi tên bằng RectangleShape để có độ dày (lineThickness)
    // Thân sẽ vẽ từ 'from' đến sát điểm 'to'
    sf::RectangleShape line(sf::Vector2f(currentLen - arrowSize, lineThickness));
    line.setOrigin(0, lineThickness / 2.f); // Căn giữa theo chiều dọc
    line.setPosition(from);
    line.setFillColor(color);
    
    // Tính góc xoay cho thân (vì sf::Lines không cần góc nhưng Rectangle thì có)
    float angle = std::atan2(dir.y, dir.x) * 180.f / 3.14159f;
    line.setRotation(angle);
    window.draw(line);

    // 2. Vẽ đầu mũi tên (Tam giác)
    sf::ConvexShape triangle;
    triangle.setPointCount(3);
    triangle.setPoint(0, to); // Đỉnh chạm thẳng vào Node
    triangle.setPoint(1, to - dir * arrowSize + perp * (arrowWidth / 2.f));
    triangle.setPoint(2, to - dir * arrowSize - perp * (arrowWidth / 2.f));

    triangle.setFillColor(color);
    window.draw(triangle);
}

// void HashVisualizer::drawArrowUp(sf::RenderWindow& window, sf::Vector2f from,
//                                   sf::Vector2f to, const sf::Color& color) {
//     sf::Vertex line[] = { sf::Vertex(from, color), sf::Vertex(to, color) };
//     window.draw(line, 2, sf::Lines);

//     sf::Vector2f dir = to - from;
//     float len = std::sqrt(dir.x*dir.x + dir.y*dir.y);
//     if (len < 1.f) return;
//     dir /= len;
//     sf::Vector2f perp(-dir.y, dir.x);

//     sf::Vertex a1[] = { sf::Vertex(to, color), sf::Vertex(to - dir*10.f + perp*5.f, color) };
//     sf::Vertex a2[] = { sf::Vertex(to, color), sf::Vertex(to - dir*10.f - perp*5.f, color) };
//     window.draw(a1, 2, sf::Lines);
//     window.draw(a2, 2, sf::Lines);
// }

// ============================================================================
// Constructor
// ============================================================================

HashVisualizer::HashVisualizer(sf::Font& mainFont, sf::Font& monoFont)
    : font_(mainFont), monoFont_(monoFont), hashBackend_(7),
      speedSlider_(0.f, 0.f, 580.f, 0.f, 10.f, 1.f, mainFont),
      btnChangeCanvas_("Change Canvas Size", mainFont),
      btnMoveControls_("Move Controls", mainFont)
{
    initLayout();
    buildPseudocode();

    // Menu buttons (left floating panel)
    std::vector<std::string> menuLabels = {
        "New Hash Table", "Add Node", "Delete Node", "Update Node", "Search"
    };
    for (size_t i = 0; i < menuLabels.size(); ++i) {
        Button btn(menuLabels[i], font_);
        btn.setSize(menuPanelRect_.width - 24.f, 36.f);
        btn.setPosition(menuPanelRect_.left + 12.f,
                        menuPanelRect_.top + 12.f + i * 44.f);
        menuButtons_.push_back(btn);
    }

    // Control bar buttons
    std::vector<std::string> ctrlLabels = {
        "Skip Back", "Step Back", "Pause", "Step Forward", "Skip Forward"
    };
    for (size_t i = 0; i < ctrlLabels.size(); ++i) {
        Button btn(ctrlLabels[i], font_);
        btn.setSize(100.f, 28.f);
        btn.setPosition(controlBarRect_.left + 8.f + i * 108.f,
                        controlBarRect_.top + (controlBarRect_.height - 28.f) * 0.5f);
        controlButtons_.push_back(btn);
    }

    btnChangeCanvas_.setSize(130.f, 28.f);
    btnMoveControls_.setSize(110.f, 28.f);

    sf::Color whiteGlass(255, 255, 255, 200); // Trắng trong suốt
    sf::Color lightBorder(200, 200, 200);    // Viền xám nhạt
    sf::Color darkText(50, 50, 50);          // Chữ xám đậm cho dễ đọc

    for (auto& btn : menuButtons_) {
        btn.setCustomColor(whiteGlass, lightBorder, darkText);
    }
    for (auto& btn : controlButtons_) {
        btn.setCustomColor(whiteGlass, lightBorder, darkText);
    }
    btnChangeCanvas_.setCustomColor(whiteGlass, lightBorder, darkText);
    btnMoveControls_.setCustomColor(whiteGlass, lightBorder, darkText);

    hashBackend_.generateRandom(5);
    syncStepsWithAnimation();
}

// ============================================================================
// Layout
// ============================================================================

void HashVisualizer::initLayout() {
    float W = config::kWindowWidth;
    float H = config::kWindowHeight;
    float barH = 44.f;

    controlBarRect_ = {0.f, H - barH, W, barH};
    canvasRect_     = {0.f, 0.f, W, H - barH};

    // Floating menu panel: top-left
    menuPanelRect_ = {20.f, 20.f, 160.f, 5 * 44.f + 24.f};

    // Step panel: top-right
    float stepW = 260.f, stepH = 70.f;
    stepPanelRect_ = {W - stepW - 20.f, 20.f, stepW, stepH};

    // Pseudocode panel: right, below step panel
    float pseudoW = 260.f;
    float pseudoTop = stepPanelRect_.top + stepPanelRect_.height + 10.f;
    pseudoPanelRect_ = {W - pseudoW - 20.f, pseudoTop, pseudoW,
                        canvasRect_.height - pseudoTop - 10.f};

    // Speed slider position (inside control bar)
    float sliderX = controlBarRect_.left + 560.f;
    float sliderY = controlBarRect_.top + (controlBarRect_.height - 4.f) * 0.5f;
    speedSlider_ = Slider(sliderX, sliderY, 280.f, 0.f, 10.f, 1.f, font_);

    // // 
    float btnY = controlBarRect_.top + (controlBarRect_.height - 28.f) * 0.5f;
    float rightMargin = 20.f; // Cách lề phải 20px
    // btnChangeCanvas_.setPosition(controlBarRect_.left + controlBarRect_.width - 260.f, btnY);
    // btnMoveControls_.setPosition(controlBarRect_.left + controlBarRect_.width - 122.f, btnY);

    // Nút Move Controls nằm ngoài cùng bên phải
    btnMoveControls_.setSize(110.f, 28.f);
    btnMoveControls_.setPosition(W - 110.f - rightMargin, btnY);

    // Change Canvas Size / Move Controls buttons
    btnChangeCanvas_.setSize(140.f, 28.f);
    btnChangeCanvas_.setPosition(W - 110.f - 140.f - rightMargin - 10.f, btnY);
}

void HashVisualizer::buildPseudocode() {
    pseudocodeLines_ = {
        "1. hash_val = key % size",
        "2. node = create_node(key)",
        "3. head = table[hash_val]",
        "4. if head is NULL:",
        "5.   table[hash_val] = node",
        "6. else:",
        "7.   node.next = head",
        "8.   table[hash_val] = node",
        "...",
    };
}

void HashVisualizer::syncStepsWithAnimation() {
    animation_.clear();
    for (const auto& b : hashBackend_.getSteps()) {
        Step s;
        s.description = b.description;
        if (b.lineID != -1) s.pseudocodeLines.push_back(b.lineID);
        s.highlightedNodes.push_back(b.targetBucket);
        s.highlightedNodes.push_back(b.targetVal);
        animation_.addStep(s);
    }
    playing_ = false;
    animationProgress_ = 0.f;
}

// ============================================================================
// Events
// ============================================================================

void HashVisualizer::handleEvent(sf::Event& event, sf::RenderWindow& window) {
    sf::Vector2f mp = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left) {

        // Menu buttons
        for (size_t i = 0; i < menuButtons_.size(); ++i) {
            if (menuButtons_[i].contains(mp)) {
                menuButtons_[i].flash();
                if      (i == 0) onActionRandom();
                else if (i == 1) { pendingAction_ = 1; enteringValue_ = true; inputBuffer_ = ""; }
                else if (i == 2) { pendingAction_ = 2; enteringValue_ = true; inputBuffer_ = ""; }
                else if (i == 3) { pendingAction_ = 4; enteringValue_ = true; inputBuffer_ = ""; }
                else if (i == 4) { pendingAction_ = 3; enteringValue_ = true; inputBuffer_ = ""; }
                break;
            }
        }

        // Control buttons
        for (size_t i = 0; i < controlButtons_.size(); ++i) {
            if (controlButtons_[i].contains(mp)) {
                controlButtons_[i].flash();
                if      (i == 0) animation_.moveToStart();
                else if (i == 1) animation_.movePrev();
                else if (i == 2) {
                    playing_ = !playing_;
                    controlButtons_[2].setLabel(playing_ ? "Pause" : "Play");
                    playClock_.restart();
                }
                else if (i == 3) animation_.moveNext();
                else if (i == 4) animation_.moveToEnd();
                break;
            }
        }

        if (speedSlider_.contains(mp)) speedSlider_.setActive(true);

    } else if (event.type == sf::Event::MouseButtonReleased) {
        speedSlider_.setActive(false);
    } else if (event.type == sf::Event::MouseMoved) {
        speedSlider_.onMouseMoved(mp);
    }

    // Text input
    if (enteringValue_ && event.type == sf::Event::TextEntered) {
        if (event.text.unicode == 13) {
            if (!inputBuffer_.empty()) {
                try {
                    int val = std::stoi(inputBuffer_);
                    if      (pendingAction_ == 1) hashBackend_.add(val);
                    else if (pendingAction_ == 2) hashBackend_.deleteNode(val);
                    else if (pendingAction_ == 3) hashBackend_.search(val);
                    else if (pendingAction_ == 4) {
                        oldKey_ = val; pendingAction_ = 5; inputBuffer_ = ""; return;
                    }
                    else if (pendingAction_ == 5) hashBackend_.update(oldKey_, val);
                    syncStepsWithAnimation();
                } catch (...) {}
            }
            enteringValue_ = false;
        } else if (event.text.unicode == 8 && !inputBuffer_.empty()) {
            inputBuffer_.pop_back();
        } else if ((event.text.unicode >= '0' && event.text.unicode <= '9') ||
                    event.text.unicode == '-') {
            inputBuffer_ += static_cast<char>(event.text.unicode);
        }
    }
}

// ============================================================================
// Update
// ============================================================================

void HashVisualizer::update(sf::Time dt) {
    speed_ = speedSlider_.getValue();

    if (playing_) {
        float dur = (speed_ < 0.1f) ? 99999.f : 1.f / speed_;
        if (playClock_.getElapsedTime().asSeconds() >= dur) {
            if (!animation_.moveNext()) {
                playing_ = false;
                controlButtons_[2].setLabel("Play");
            }
            playClock_.restart();
        }
    }

    animationProgress_ += dt.asSeconds() * std::max(speed_, 0.1f);
    if (animationProgress_ > 1.f) animationProgress_ = 0.f;
}

// ============================================================================
// Render: Menu Panel (top-left floating, pink)
// ============================================================================

void HashVisualizer::renderMenuPanel(sf::RenderWindow& window) {
    // Panel background: pink pastel
    drawRoundedRect(window, menuPanelRect_, 12.f,
                    sf::Color(255, 230, 238, 230),
                    sf::Color(249, 150, 180, 200), 2.f);

    for (auto& btn : menuButtons_) btn.draw(window);

    // Input prompt overlay
    if (enteringValue_) {
        std::string prompt = "> " + inputBuffer_;
        if ((int)(animationProgress_ * 2) % 2 == 0) prompt += "_";
        sf::Text t(prompt, font_, 13);
        t.setFillColor(sf::Color(120, 40, 80));
        t.setPosition(menuPanelRect_.left + 12.f,
                      menuPanelRect_.top + menuPanelRect_.height + 8.f);
        window.draw(t);
    }
}

// ============================================================================
// Render: Step Panel (top-right)
// ============================================================================

void HashVisualizer::renderStepPanel(sf::RenderWindow& window, const Step* cur) {
    // Outer panel: light purple border
    drawRoundedRect(window, stepPanelRect_, 10.f,
                    sf::Color(255, 255, 255, 230),
                    sf::Color(180, 160, 220, 200), 2.f);

    sf::Text title("Step", font_, 13);
    title.setFillColor(sf::Color(100, 70, 160));
    title.setPosition(stepPanelRect_.left + 12.f, stepPanelRect_.top + 8.f);
    window.draw(title);

    if (cur && !cur->description.empty()) {
        // Inner white box for step description
        sf::FloatRect inner(stepPanelRect_.left + 8.f, stepPanelRect_.top + 28.f,
                            stepPanelRect_.width - 16.f, stepPanelRect_.height - 36.f);
        drawRoundedRect(window, inner, 6.f,
                        sf::Color(255, 255, 255, 240),
                        sf::Color(200, 185, 235, 180), 1.5f);

        sf::Text desc(cur->description, font_, 12);
        desc.setFillColor(sf::Color(60, 40, 100));
        desc.setPosition(inner.left + 8.f, inner.top + 6.f);
        window.draw(desc);
    }
}

// ============================================================================
// Render: Pseudocode Panel (right)
// ============================================================================

void HashVisualizer::renderPseudocodePanel(sf::RenderWindow& window, const Step* cur) {
    // Outer panel: light purple
    drawRoundedRect(window, pseudoPanelRect_, 10.f,
                    sf::Color(248, 244, 255, 230),
                    sf::Color(180, 160, 220, 200), 2.f);

    sf::Text title("Pseudocode", font_, 13);
    title.setFillColor(sf::Color(100, 70, 160));
    title.setPosition(pseudoPanelRect_.left + 12.f, pseudoPanelRect_.top + 10.f);
    window.draw(title);

    // Inner white box
    sf::FloatRect inner(pseudoPanelRect_.left + 8.f, pseudoPanelRect_.top + 32.f,
                        pseudoPanelRect_.width - 16.f, pseudoPanelRect_.height - 42.f);
    drawRoundedRect(window, inner, 6.f,
                    sf::Color(255, 255, 255, 240),
                    sf::Color(200, 185, 235, 150), 1.5f);

    int highlightLine = (cur && !cur->pseudocodeLines.empty()) ? cur->pseudocodeLines[0] : -1;

    for (size_t i = 0; i < pseudocodeLines_.size(); ++i) {
        bool hl = ((int)i == highlightLine);
        float lineY = inner.top + 8.f + i * 18.f;

        if (hl) {
            sf::RectangleShape bg(sf::Vector2f(inner.width - 8.f, 17.f));
            bg.setPosition(inner.left + 4.f, lineY - 1.f);
            bg.setFillColor(sf::Color(255, 225, 140, 200));
            window.draw(bg);
        }

        sf::Text line(pseudocodeLines_[i], monoFont_, 11);
        line.setFillColor(hl ? sf::Color(100, 60, 0) : sf::Color(80, 60, 120));
        line.setPosition(inner.left + 8.f, lineY);
        window.draw(line);
    }
}

// ============================================================================
// Render: Buckets (horizontal row, lower-center of canvas)
// ============================================================================

void HashVisualizer::renderBuckets(sf::RenderWindow& window) {
    const Step* cur = animation_.currentStep();
    int targetBucket = (cur && !cur->highlightedNodes.empty()) ? cur->highlightedNodes[0] : -1;

    int n = hashBackend_.getSize();
    bucketW_   = 80.f;
    bucketH_   = 58.f;
    bucketGapX_= 88.f;

    float totalW  = n * bucketGapX_ - (bucketGapX_ - bucketW_);
    bucketStartX_ = canvasRect_.left + (canvasRect_.width - totalW) * 0.5f;
    bucketRowY_   = canvasRect_.top + canvasRect_.height - bucketH_ - 55.f;

    for (int i = 0; i < n; ++i) {
        float px = bucketStartX_ + i * bucketGapX_;
        float py = bucketRowY_;

        bool isTarget = (i == targetBucket);
        sf::Color fill   = isTarget ? sf::Color(255, 235, 180, 230) : sf::Color(195, 218, 240, 200);
        sf::Color border = isTarget ? sf::Color(220, 150, 60, 220)  : sf::Color(130, 170, 210, 200);

        drawRoundedRect(window, sf::FloatRect(px, py, bucketW_, bucketH_),
                        10.f, fill, border, 2.f);

        // "H{i}" label inside bucket
        sf::Text lbl("H" + std::to_string(i), font_, 13);
        lbl.setFillColor(sf::Color(60, 80, 130));
        sf::FloatRect lb = lbl.getLocalBounds();
        lbl.setPosition(px + (bucketW_ - lb.width)  * 0.5f - lb.left,
                        py + (bucketH_ - lb.height) * 0.5f - lb.top);
        window.draw(lbl);

        // Index label below
        sf::Text idx(std::to_string(i), font_, 11);
        idx.setFillColor(sf::Color(120, 140, 180));
        sf::FloatRect ib = idx.getLocalBounds();
        idx.setPosition(px + (bucketW_ - ib.width) * 0.5f - ib.left, py + bucketH_ + 4.f);
        window.draw(idx);
    }
}

// ============================================================================
// Render: Nodes (upward from each bucket)
// ============================================================================

void HashVisualizer::renderNodes(sf::RenderWindow& window, const Step* cur) {
    auto table = hashBackend_.getTable();
    int targetBucket = (cur && !cur->highlightedNodes.empty()) ? cur->highlightedNodes[0] : -1;
    int targetVal    = (cur && cur->highlightedNodes.size() > 1)  ? cur->highlightedNodes[1] : -1;

    float nodeW = bucketW_;
    float nodeH = 46.f;
    float gapY  = 72.f;

    for (int i = 0; i < hashBackend_.getSize(); ++i) {
        Node* curr = table[i];
        int depth  = 0;
        float bx   = bucketStartX_ + i * bucketGapX_;
        float by   = bucketRowY_;

        while (curr) {
            float px = bx;
            float py = by - (depth + 1) * gapY;

            // Assign color
            if (valueToColorIdx_.find(curr->val) == valueToColorIdx_.end())
                valueToColorIdx_[curr->val] = valueToColorIdx_.size() % colorPalettes_.size();
            auto [c1, c2] = colorPalettes_[valueToColorIdx_[curr->val]];

            bool isTarget = (curr->val == targetVal && i == targetBucket);
            if (isTarget) { c1 = sf::Color(255,170,170,230); c2 = sf::Color(255,200,185,230); }
            else if (i == targetBucket) { c1.a = 240; c2.a = 240; }

            sf::Color nodeOutline = isTarget ? sf::Color(210,80,80,200) : sf::Color(160,180,220,180);

            // Draw node as rounded rect with soft gradient (two halves)
            sf::FloatRect left(px, py, nodeW * 0.5f, nodeH);
            sf::FloatRect right(px + nodeW * 0.5f, py, nodeW * 0.5f, nodeH);
            drawRoundedRect(window, sf::FloatRect(px, py, nodeW, nodeH),
                            10.f, c1, nodeOutline, 1.5f);
            // Soft right-half tint
            sf::RectangleShape tint(sf::Vector2f(nodeW * 0.5f - 2.f, nodeH - 4.f));
            tint.setPosition(px + nodeW * 0.5f, py + 2.f);
            tint.setFillColor(sf::Color(c2.r, c2.g, c2.b, 120));
            window.draw(tint);

            // Value text
            sf::Text val(std::to_string(curr->val), font_, 14);
            val.setFillColor(sf::Color(40, 40, 80));
            sf::FloatRect vb = val.getLocalBounds();
            val.setPosition(px + (nodeW - vb.width)  * 0.5f - vb.left,
                            py + (nodeH - vb.height) * 0.5f - vb.top);
            window.draw(val);

            // Arrow from bucket top (or prev node bottom) to this node bottom
            sf::Color arrowCol = isTarget ? sf::Color(200,80,80,180) : sf::Color(140,170,210,180);
            float padding = 1.0f;
            if (depth == 0) {
                drawArrowUp(window,
                            sf::Vector2f(bx + nodeW * 0.5f, by), 
                            sf::Vector2f(px + nodeW * 0.5f, py + nodeH + padding),
                            arrowCol);
            } else {
                float prevPy = by - depth * gapY;
                drawArrowUp(window,
                            sf::Vector2f(px + nodeW * 0.5f, prevPy), 
                            sf::Vector2f(px + nodeW * 0.5f, py + nodeH + padding),
                            arrowCol);
            }

            curr = curr->next;
            depth++;
        }
    }
}

// ============================================================================
// Render: Control Bar (bottom, yellow)
// ============================================================================

void HashVisualizer::renderControlBar(sf::RenderWindow& window) {
    // Yellow pastel bar
    drawRoundedRect(window, controlBarRect_, 0.f,
                    sf::Color(255, 250, 210, 255),
                    sf::Color(240, 200, 80, 200), 1.5f);

    // Playback buttons
    for (auto& btn : controlButtons_) btn.draw(window);

    // Speed slider
    speedSlider_.draw(window);

//     // W: label + value
//     float labelX = controlBarRect_.left + 560.f + 290.f;
//     float labelY = controlBarRect_.top + 8.f;

//     sf::Text wLabel("W :", font_, 12);
//     wLabel.setFillColor(sf::Color(80, 60, 20));
//     wLabel.setPosition(labelX, labelY);
//     window.draw(wLabel);

//     sf::FloatRect wBox(labelX + 28.f, labelY, 60.f, 26.f);
//     drawRoundedRect(window, wBox, 4.f,
//                     sf::Color(255,255,255,220), sf::Color(200,160,60,180), 1.5f);
//     sf::Text wVal(wBuffer_, font_, 12);
//     wVal.setFillColor(sf::Color(60, 40, 10));
//     wVal.setPosition(wBox.left + 4.f, wBox.top + 5.f);
//     window.draw(wVal);

//     sf::Text hLabel("H :", font_, 12);
//     hLabel.setFillColor(sf::Color(80, 60, 20));
//     hLabel.setPosition(labelX + 100.f, labelY);
//     window.draw(hLabel);

//     sf::FloatRect hBox(labelX + 128.f, labelY, 60.f, 26.f);
//     drawRoundedRect(window, hBox, 4.f,
//                     sf::Color(255,255,255,220), sf::Color(200,160,60,180), 1.5f);
//     sf::Text hVal(hBuffer_, font_, 12);
//     hVal.setFillColor(sf::Color(60, 40, 10));
//     hVal.setPosition(hBox.left + 4.f, hBox.top + 5.f);
//     window.draw(hVal);
    // Neo tọa độ vào lề phải để dù màn hình to hay nhỏ nó vẫn né cái nút ra
    float rightAnchor = controlBarRect_.width - 290.f; // Bắt đầu nhóm W, H cách lề phải 290px
    float labelY = controlBarRect_.top + 8.f;

    // Vẽ nhãn W và ô nhập W
    sf::Text wLabel("W :", font_, 12);
    wLabel.setFillColor(sf::Color(80, 60, 20));
    wLabel.setPosition(rightAnchor - 180.f, labelY); // Dịch về bên trái
    window.draw(wLabel);

    sf::FloatRect wBox(rightAnchor - 150.f, labelY, 60.f, 26.f);
    drawRoundedRect(window, wBox, 4.f, sf::Color(255,255,255,220), sf::Color(200,160,60,180), 1.5f);
    sf::Text wVal(wBuffer_, font_, 12);
    wVal.setFillColor(sf::Color(60, 40, 10));
    wVal.setPosition(wBox.left + 4.f, wBox.top + 5.f);
    window.draw(wVal);

    // Vẽ nhãn H và ô nhập H
    sf::Text hLabel("H :", font_, 12);
    hLabel.setFillColor(sf::Color(80, 60, 20));
    hLabel.setPosition(rightAnchor - 80.f, labelY);
    window.draw(hLabel);

    sf::FloatRect hBox(rightAnchor - 50.f, labelY, 60.f, 26.f);
    drawRoundedRect(window, hBox, 4.f, sf::Color(255,255,255,220), sf::Color(200,160,60,180), 1.5f);
    sf::Text hVal(hBuffer_, font_, 12);
    hVal.setFillColor(sf::Color(60, 40, 10));
    hVal.setPosition(hBox.left + 4.f, hBox.top + 5.f);
    window.draw(hVal);

    btnChangeCanvas_.draw(window);
    btnMoveControls_.draw(window);

    // Step counter
    std::string stepInfo = "Step " + std::to_string(animation_.currentIndex() + 1)
                         + " / " + std::to_string(animation_.totalSteps());
    sf::Text stepTxt(stepInfo, font_, 11);
    stepTxt.setFillColor(sf::Color(100, 80, 20));
    stepTxt.setPosition(controlBarRect_.left + controlBarRect_.width * 0.5f - 30.f,
                        controlBarRect_.top + controlBarRect_.height - 16.f);
    window.draw(stepTxt);
}

// ============================================================================
// Draw (main entry)
// ============================================================================

void HashVisualizer::draw(sf::RenderWindow& window) {
    // Canvas background: very light blue-white
    sf::RectangleShape bg(sf::Vector2f(canvasRect_.width, canvasRect_.height));
    bg.setPosition(canvasRect_.left, canvasRect_.top);
    bg.setFillColor(sf::Color(242, 247, 255, 255));
    window.draw(bg);

    renderBuckets(window);
    renderNodes(window, animation_.currentStep());
    renderMenuPanel(window);
    renderStepPanel(window, animation_.currentStep());
    renderPseudocodePanel(window, animation_.currentStep());
    renderControlBar(window);
}

// ============================================================================
// Actions
// ============================================================================

void HashVisualizer::onActionRandom() {
    hashBackend_.clear();
    hashBackend_.generateRandom(5);
    syncStepsWithAnimation();
    valueToColorIdx_.clear();
}

void HashVisualizer::onActionClear() {
    hashBackend_.clear();
    syncStepsWithAnimation();
    valueToColorIdx_.clear();
    animation_.moveToStart();
}