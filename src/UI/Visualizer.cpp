        #include "Visualizer.h"

        #include <cmath>
        #include <filesystem>
        #include <sstream>

        #include "../config/Config.h"
        #include "../data/GraphLoader.h"

        namespace {
        int randomWeight() {
            static std::mt19937 rng(static_cast<unsigned int>(std::random_device{}()));
            static std::uniform_int_distribution<int> dist(1, 30);
            return dist(rng);
        }

    float smoothLerp(float a, float b, float t) {
        float easeT = t * t * (3.0f - 2.0f * t);
        return a + (b - a) * easeT;
    }

        constexpr float kMatrixBaseCell = 34.0f;

        constexpr float kRightContentInset = 12.0f;
        constexpr float kLeftCollapseBtnW = 24.0f;
        constexpr float kLeftCollapseBtnH = 80.0f;
        constexpr float kGraphMinX = 274.0f;
        constexpr float kGraphMaxX = 934.0f;
        constexpr float kGraphMinY = 110.0f;
        constexpr float kGraphMaxY = 560.0f;

        constexpr float kRightToggleBtnW = 24.0f;
        constexpr float kRightStepBtnH = 80.0f;

        struct UILayout {
            sf::FloatRect leftPanel;
            sf::FloatRect rightPanel;
            sf::FloatRect graphViewport;
            sf::FloatRect bottomPanel;
            sf::FloatRect leftCollapseBtn;
            sf::FloatRect rightStepToggleBtn;
            sf::FloatRect rightPseudoToggleBtn;
            sf::FloatRect rightStepBox;
            sf::FloatRect rightPseudoBox;
            sf::FloatRect bottomButtonsArea;
            sf::FloatRect bottomStatusArea;
            sf::FloatRect bottomStatusTextArea;
            sf::FloatRect bottomSliderArea;
        };

        UILayout computeUILayout(const sf::Vector2u& winSize, bool showLeft, bool showRightStep, bool showRightPseudo) {
            const float w = static_cast<float>(winSize.x);
            const float h = static_cast<float>(winSize.y);
            const float margin = 20.0f;
            const float gutter = 20.0f;
            const float minCenterW = 420.0f;
            const float maxSideByWindow = std::max(180.0f, (w - 2.0f * margin - 2.0f * gutter - minCenterW) * 0.5f);
            const float sideW = std::clamp(248.0f, 180.0f, maxSideByWindow);
            const float bottomH = 80.0f;
            const float toggleW = 24.0f;

            UILayout l;
            l.bottomPanel = sf::FloatRect(margin, h - margin - bottomH, std::max(100.0f, w - 2.0f * margin), bottomH);

            const float headerH = 44.0f;
            const float panelTop = margin + headerH;
            const float sideH = std::max(120.0f, l.bottomPanel.top - panelTop - gutter);
            l.leftPanel = sf::FloatRect(margin, panelTop, sideW, sideH);
            l.rightPanel = sf::FloatRect(w - margin - sideW, panelTop, sideW, sideH);

    float leftCollapseX = showLeft ? (l.leftPanel.left + l.leftPanel.width) : 0.0f;
            l.leftCollapseBtn = sf::FloatRect(leftCollapseX,
                                            l.leftPanel.top + (l.leftPanel.height - kLeftCollapseBtnH) * 0.5f,
                                            kLeftCollapseBtnW,
                                            kLeftCollapseBtnH);

    float stepToggleX = showRightStep ? (l.rightPanel.left - toggleW) : (w - toggleW);
            l.rightStepToggleBtn = sf::FloatRect(stepToggleX,
                                                l.rightPanel.top + 12.0f,
                                                toggleW,
                                                kRightStepBtnH);

    float pseudoToggleX = showRightPseudo ? (l.rightPanel.left - toggleW) : (w - toggleW);
            l.rightPseudoToggleBtn = sf::FloatRect(pseudoToggleX,
                                                l.rightPanel.top + 230.0f,
                                                toggleW,
                                                80.0f);

            const float rightContentW = l.rightPanel.width;
            l.rightStepBox = sf::FloatRect(l.rightPanel.left, l.rightPanel.top, rightContentW, 218.0f);
            l.rightPseudoBox = sf::FloatRect(l.rightPanel.left,
                                            l.rightPanel.top + 230.0f,
                                            rightContentW,
                                            std::max(120.0f, l.rightPanel.height - 230.0f));

            float graphMinX = leftCollapseX + kLeftCollapseBtnW + gutter;
            float graphMaxX = std::min(stepToggleX, pseudoToggleX) - gutter;
            float graphMinY = panelTop;
            float graphMaxY = l.bottomPanel.top - gutter;

    float availW = std::max(10.0f, graphMaxX - graphMinX);
    float availH = std::max(10.0f, graphMaxY - graphMinY);
    
    // Giữ tỷ lệ khung hình 4:3 (1.333) để đồ thị luôn cân đối và nằm giữa khi full-screen
    float targetAspect = 1.333f;
    float finalW = availW;
    float finalH = availH;
    if (availW / availH > targetAspect) {
        finalW = availH * targetAspect;
    } else {
        finalH = availW / targetAspect;
    }
    
    float offsetX = (availW - finalW) * 0.5f;
    float offsetY = (availH - finalH) * 0.5f;

    l.graphViewport = sf::FloatRect(graphMinX + offsetX, graphMinY + offsetY, finalW, finalH);

            const float pad = 12.0f;
            const float buttonsW = std::clamp(l.bottomPanel.width * 0.42f, 360.0f, 560.0f);
            l.bottomButtonsArea = sf::FloatRect(l.bottomPanel.left + pad, l.bottomPanel.top + pad, buttonsW, l.bottomPanel.height - 2.0f * pad);
            l.bottomStatusArea = sf::FloatRect(l.bottomButtonsArea.left + l.bottomButtonsArea.width + 18.0f,
                                            l.bottomPanel.top + pad,
                                            std::max(120.0f, l.bottomPanel.width - buttonsW - pad * 2.0f - 18.0f),
                                            l.bottomPanel.height - 2.0f * pad);

            const float sliderGap = 16.0f;
            const float sliderW = std::clamp(l.bottomStatusArea.width * 0.34f, 180.0f, 260.0f);
            l.bottomSliderArea = sf::FloatRect(l.bottomStatusArea.left + l.bottomStatusArea.width - sliderW,
                                            l.bottomStatusArea.top,
                                            sliderW,
                                            l.bottomStatusArea.height);
            l.bottomStatusTextArea = sf::FloatRect(l.bottomStatusArea.left,
                                                l.bottomStatusArea.top,
                                                std::max(80.0f, l.bottomStatusArea.width - sliderW - sliderGap),
                                                l.bottomStatusArea.height);
            return l;
        }

        sf::FloatRect gGraphViewport(kGraphMinX, kGraphMinY, kGraphMaxX - kGraphMinX, kGraphMaxY - kGraphMinY);

        sf::Vector2f graphToScreen(float nx, float ny) {
            return sf::Vector2f(gGraphViewport.left + nx * gGraphViewport.width,
                                gGraphViewport.top + ny * gGraphViewport.height);
        }

        sf::Vector2f matrixOriginForSize(int n) {
            const float cell = std::clamp(kMatrixBaseCell - std::max(0, n - 10) * 0.55f, 14.0f, kMatrixBaseCell);
            const float tableW = cell * static_cast<float>(n + 1);
            const float tableH = cell * static_cast<float>(n + 1);
            const float x = gGraphViewport.left + (gGraphViewport.width - tableW) * 0.5f;
            const float y = gGraphViewport.top + (gGraphViewport.height - tableH) * 0.5f;
            return sf::Vector2f(x, y);
        }

        float matrixCellForSize(int n) {
            return std::clamp(kMatrixBaseCell - std::max(0, n - 10) * 0.45f, 16.0f, kMatrixBaseCell);
        }

        void drawRoundedFill(sf::RenderWindow& window, const sf::FloatRect& rect, float radius, const sf::Color& color) {
            if (rect.width <= 0.f || rect.height <= 0.f) {
                return;
            }

            const float r = std::clamp(radius, 0.0f, std::min(rect.width, rect.height) * 0.5f);
            if (r <= 0.01f) {
                sf::RectangleShape box(sf::Vector2f(rect.width, rect.height));
                box.setPosition(rect.left, rect.top);
                box.setFillColor(color);
                window.draw(box);
                return;
            }

            sf::RectangleShape center(sf::Vector2f(rect.width - 2.f * r, rect.height));
            center.setPosition(rect.left + r, rect.top);
            center.setFillColor(color);
            window.draw(center);

            sf::RectangleShape middle(sf::Vector2f(rect.width, rect.height - 2.f * r));
            middle.setPosition(rect.left, rect.top + r);
            middle.setFillColor(color);
            window.draw(middle);

            sf::CircleShape corner(r);
            corner.setFillColor(color);

            corner.setPosition(rect.left, rect.top);
            window.draw(corner);

            corner.setPosition(rect.left + rect.width - 2.f * r, rect.top);
            window.draw(corner);

            corner.setPosition(rect.left, rect.top + rect.height - 2.f * r);
            window.draw(corner);

            corner.setPosition(rect.left + rect.width - 2.f * r, rect.top + rect.height - 2.f * r);
            window.draw(corner);
        }

        void drawRoundedBox(sf::RenderWindow& window,
                            const sf::FloatRect& rect,
                            float radius,
                            float outlineThickness,
                            const sf::Color& fill,
                            const sf::Color& outline) {
            drawRoundedFill(window, rect, radius, outline);

            if (outlineThickness <= 0.01f) {
                return;
            }

            const float inset = outlineThickness;
            const sf::FloatRect inner(rect.left + inset,
                                    rect.top + inset,
                                    std::max(0.f, rect.width - inset * 2.f),
                                    std::max(0.f, rect.height - inset * 2.f));
            drawRoundedFill(window, inner, std::max(0.f, radius - inset), fill);
        }

        sf::ConvexShape createRoundedRect(sf::Vector2f size, float radius) {
            int pointsPerCorner = 15;
            sf::ConvexShape shape(pointsPerCorner * 4);
            int index = 0;
            const float PI = 3.14159265358979323846f;
            
            for(int i = 0; i < pointsPerCorner; ++i){
                float angle = i * (PI / 2) / (pointsPerCorner - 1);
                shape.setPoint(index++, sf::Vector2f(size.x - radius + radius * std::sin(angle), radius - radius * std::cos(angle)));
            }
            for(int i = 0; i < pointsPerCorner; ++i){
                float angle = PI / 2 + i * (PI / 2) / (pointsPerCorner - 1);
                shape.setPoint(index++, sf::Vector2f(size.x - radius + radius * std::sin(angle), size.y - radius - radius * std::cos(angle)));
            }
            for(int i = 0; i < pointsPerCorner; ++i){
                float angle = PI + i * (PI / 2) / (pointsPerCorner - 1);
                shape.setPoint(index++, sf::Vector2f(radius + radius * std::sin(angle), size.y - radius - radius * std::cos(angle)));
            }
            for(int i = 0; i < pointsPerCorner; ++i){
                float angle = 3 * PI / 2 + i * (PI / 2) / (pointsPerCorner - 1);
                shape.setPoint(index++, sf::Vector2f(radius + radius * std::sin(angle), radius - radius * std::cos(angle)));
            }
            return shape;
        }

        sf::VertexArray createGradientRoundedRect(sf::Vector2f size, float radius, sf::Vector2f pos, sf::Color cBottomLeft, sf::Color cTopRight) {
            int pointsPerCorner = 15;
            int totalPoints = pointsPerCorner * 4;
            
            sf::VertexArray va(sf::PrimitiveType::Triangles, totalPoints * 3);
            sf::Vector2f center = size / 2.0f;
            
            auto lerpColor = [](sf::Color a, sf::Color b, float t) {
                t = std::max(0.0f, std::min(1.0f, t));
                return sf::Color(
                    static_cast<uint8_t>(a.r + (b.r - a.r) * t),
                    static_cast<uint8_t>(a.g + (b.g - a.g) * t),
                    static_cast<uint8_t>(a.b + (b.b - a.b) * t),
                    static_cast<uint8_t>(a.a + (b.a - a.a) * t)
                );
            };

            sf::Color centerColor = lerpColor(cBottomLeft, cTopRight, 0.5f);
            std::vector<sf::Vector2f> p(totalPoints);
            int index = 0;
            const float PI = 3.14159265358979323846f;
            
            for(int i = 0; i < pointsPerCorner; ++i){
                float angle = i * (PI / 2) / (pointsPerCorner - 1);
                p[index++] = sf::Vector2f(size.x - radius + radius * std::sin(angle), radius - radius * std::cos(angle));
            }
            for(int i = 0; i < pointsPerCorner; ++i){
                float angle = PI / 2 + i * (PI / 2) / (pointsPerCorner - 1);
                p[index++] = sf::Vector2f(size.x - radius + radius * std::sin(angle), size.y - radius - radius * std::cos(angle));
            }
            for(int i = 0; i < pointsPerCorner; ++i){
                float angle = PI + i * (PI / 2) / (pointsPerCorner - 1);
                p[index++] = sf::Vector2f(radius + radius * std::sin(angle), size.y - radius - radius * std::cos(angle));
            }
            for(int i = 0; i < pointsPerCorner; ++i){
                float angle = 3 * PI / 2 + i * (PI / 2) / (pointsPerCorner - 1);
                p[index++] = sf::Vector2f(radius + radius * std::sin(angle), radius - radius * std::cos(angle));
            }

            auto calcColor = [&](sf::Vector2f pt) {
                float t = (pt.x + (size.y - pt.y)) / (size.x + size.y);
                return lerpColor(cBottomLeft, cTopRight, t);
            };

            for (int i = 0; i < totalPoints; ++i) {
                int next = (i + 1) % totalPoints;
                va[i * 3].position = pos + center;
                va[i * 3].color = centerColor;
                va[i * 3 + 1].position = pos + p[i];
                va[i * 3 + 1].color = calcColor(p[i]);
                va[i * 3 + 2].position = pos + p[next];
                va[i * 3 + 2].color = calcColor(p[next]);
            }
            
            return va;
        }

        bool loadFontFromCandidates(sf::Font& font, const std::vector<std::string>& candidates) {
            for (const auto& path : candidates) {
                std::error_code ec;
                if (!std::filesystem::exists(path, ec) || ec) {
                    continue;
                }
                if (font.loadFromFile(path)) {
                    return true;
                }
            }
            return false;
        }

        bool loadTextureFromCandidates(sf::Texture& texture, const std::vector<std::string>& candidates) {
            for (const auto& path : candidates) {
                std::error_code ec;
                if (!std::filesystem::exists(path, ec) || ec) {
                    continue;
                }
                if (texture.loadFromFile(path)) {
                    return true;
                }
            }
            return false;
        }

        void clampTextToWidth(sf::Text& text, float maxWidth) {
            const std::string original = text.getString().toAnsiString();
            if (original.empty()) {
                return;
            }

            if (text.getLocalBounds().width <= maxWidth) {
                return;
            }

            const std::string ellipsis = "...";
            std::string trimmed = original;
            text.setString(trimmed + ellipsis);
            while (!trimmed.empty() && text.getLocalBounds().width > maxWidth) {
                trimmed.pop_back();
                text.setString(trimmed + ellipsis);
            }

            if (trimmed.empty() && text.getLocalBounds().width > maxWidth) {
                text.setString(ellipsis);
            }
        }

        std::vector<std::string> wrapTextToWidth(const sf::Font& font,
                                                const std::string& content,
                                                unsigned int charSize,
                                                float maxWidth) {
            sf::Text measure("", font, charSize);
            auto fits = [&](const std::string& s) {
                measure.setString(s);
                return measure.getLocalBounds().width <= maxWidth;
            };

            std::vector<std::string> lines;
            std::istringstream iss(content);
            std::string word;
            std::string current;

            auto flushCurrent = [&]() {
                if (!current.empty()) {
                    lines.push_back(current);
                    current.clear();
                }
            };

            auto appendLongWord = [&](const std::string& longWord) {
                std::string chunk;
                for (char ch : longWord) {
                    const std::string candidate = chunk + ch;
                    if (!chunk.empty() && !fits(candidate)) {
                        lines.push_back(chunk);
                        chunk = std::string(1, ch);
                    } else {
                        chunk = candidate;
                    }
                }
                return chunk;
            };

            while (iss >> word) {
                if (current.empty()) {
                    if (fits(word)) {
                        current = word;
                    } else {
                        current = appendLongWord(word);
                    }
                    continue;
                }

                const std::string candidate = current + " " + word;
                if (fits(candidate)) {
                    current = candidate;
                } else {
                    flushCurrent();
                    if (fits(word)) {
                        current = word;
                    } else {
                        current = appendLongWord(word);
                    }
                }
            }

            flushCurrent();
            if (lines.empty()) {
                lines.push_back("");
            }
            return lines;
        }

        }

        bool Visualizer::isMstSelected() const {
            return selectedStructure_ == RenderViewKind::MST;
        }

        void Visualizer::setupDefaultGraph() {
            graph_.clear();
            // Hardcoded 6-node layout tuned to reduce edge crossings in the default sample graph.
            graph_.addNode(0, 0.20f, 0.30f);
            graph_.addNode(1, 0.42f, 0.20f);
            graph_.addNode(2, 0.74f, 0.28f);
            graph_.addNode(3, 0.78f, 0.60f);
            graph_.addNode(4, 0.55f, 0.74f);
            graph_.addNode(5, 0.24f, 0.62f);

            graph_.addEdge(0, 1, 4);
            graph_.addEdge(1, 2, 7);
            graph_.addEdge(2, 3, 3);
            graph_.addEdge(3, 4, 6);
            graph_.addEdge(4, 5, 5);
            graph_.addEdge(5, 0, 8);
            graph_.addEdge(0, 2, 9);
            graph_.addEdge(1, 4, 10);

            selectedNodeId_ = -1;
            syncAdjacencyMatrixFromGraph();
        }

        void Visualizer::syncAdjacencyMatrixFromGraph() {
            int maxId = -1;
            for (const auto& node : graph_.getNodes()) {
                maxId = std::max(maxId, node.id);
            }

            const int n = maxId + 1;
            adjacencyMatrix_.assign(static_cast<size_t>(n), std::vector<int>(static_cast<size_t>(n), 0));
            nodeAlive_.assign(static_cast<size_t>(n), false);

            for (const auto& node : graph_.getNodes()) {
                if (node.id >= 0 && node.id < n) {
                    nodeAlive_[static_cast<size_t>(node.id)] = true;
                }
            }

            for (const auto& edge : graph_.getEdges()) {
                if (edge.from >= 0 && edge.to >= 0 && edge.from < n && edge.to < n) {
                    adjacencyMatrix_[static_cast<size_t>(edge.from)][static_cast<size_t>(edge.to)] = edge.weight;
                    adjacencyMatrix_[static_cast<size_t>(edge.to)][static_cast<size_t>(edge.from)] = edge.weight;
                }
            }
        }

        void Visualizer::pushUndoState() {
            GraphSnapshot snap;
            snap.graph = graph_;
            snap.adjacency = adjacencyMatrix_;
            snap.alive = nodeAlive_;
            snap.selectedNodeId = selectedNodeId_;
            undoStack_.push_back(std::move(snap));
            if (undoStack_.size() > 64) {
                undoStack_.erase(undoStack_.begin());
            }
        }

        void Visualizer::rebuildGraphFromAdjacencyMatrix() {
            const int n = static_cast<int>(adjacencyMatrix_.size());
            if (static_cast<int>(nodeAlive_.size()) < n) {
                nodeAlive_.resize(static_cast<size_t>(n), true);
            }

            const auto oldNodes = graph_.getNodes();
            std::unordered_map<int, sf::Vector2f> oldPos;
            oldPos.reserve(oldNodes.size());
            for (const auto& node : oldNodes) {
                oldPos[node.id] = sf::Vector2f(node.x, node.y);
            }

            Graph rebuilt;
            for (int i = 0; i < n; ++i) {
                if (!nodeAlive_[static_cast<size_t>(i)]) {
                    continue;
                }

                auto it = oldPos.find(i);
                if (it != oldPos.end()) {
                    rebuilt.addNode(i, it->second.x, it->second.y);
                } else {
                    const float angle = 0.7f * static_cast<float>(i);
                    float x = 0.55f + 0.28f * std::cos(angle);
                    float y = 0.58f + 0.28f * std::sin(angle);
                    rebuilt.addNode(i, std::clamp(x, 0.18f, 0.92f), std::clamp(y, 0.22f, 0.90f));
                }
            }

            for (int i = 0; i < n; ++i) {
                if (!nodeAlive_[static_cast<size_t>(i)]) {
                    continue;
                }
                for (int j = i + 1; j < n; ++j) {
                    if (!nodeAlive_[static_cast<size_t>(j)]) {
                        continue;
                    }
                    const int w = adjacencyMatrix_[static_cast<size_t>(i)][static_cast<size_t>(j)];
                    if (w > 0) {
                        rebuilt.addEdge(i, j, w);
                    }
                }
            }

            graph_ = rebuilt;
        }

        int Visualizer::hitTestNode(const sf::Vector2f& mousePos) const {
            const float radius = config::kNodeRadius;
            for (const auto& node : graph_.getNodes()) {
                const sf::Vector2f screenPos = graphToScreen(node.x, node.y);
                const float nx = screenPos.x;
                const float ny = screenPos.y;
                const float dx = mousePos.x - nx;
                const float dy = mousePos.y - ny;
                if ((dx * dx + dy * dy) <= (radius * radius)) {
                    return node.id;
                }
            }
            return -1;
        }

        bool Visualizer::handleAdjacencyMatrixClick(const sf::Vector2f& mousePos) {
            const int n = static_cast<int>(adjacencyMatrix_.size());
            if (n <= 0) {
                return false;
            }

            const float matrixCell = matrixCellForSize(n);
            const sf::Vector2f origin = matrixOriginForSize(n);
            const float tableW = matrixCell * static_cast<float>(n + 1);
            const float tableH = matrixCell * static_cast<float>(n + 1);
            if (mousePos.x < origin.x || mousePos.y < origin.y || mousePos.x > (origin.x + tableW) ||
                mousePos.y > (origin.y + tableH)) {
                return false;
            }

            const int col = static_cast<int>((mousePos.x - origin.x) / matrixCell) - 1;
            const int row = static_cast<int>((mousePos.y - origin.y) / matrixCell) - 1;
            if (row < 0 || col < 0 || row >= n || col >= n || row == col) {
                return false;
            }

            if (static_cast<int>(nodeAlive_.size()) < n) {
                nodeAlive_.resize(static_cast<size_t>(n), true);
            }
            if (!nodeAlive_[static_cast<size_t>(row)] || !nodeAlive_[static_cast<size_t>(col)]) {
                return false;
            }

            pushUndoState();

            int& weightCell = adjacencyMatrix_[static_cast<size_t>(row)][static_cast<size_t>(col)];
            if (weightCell > 0) {
                weightCell = 0;
                adjacencyMatrix_[static_cast<size_t>(col)][static_cast<size_t>(row)] = 0;
            } else {
                const int w = randomWeight();
                weightCell = w;
                adjacencyMatrix_[static_cast<size_t>(col)][static_cast<size_t>(row)] = w;
            }

            rebuildGraphFromAdjacencyMatrix();
            animation_.clear();
            playing_ = false;
            state_ = AppState::Paused;
            timelineDirty_ = true;
            return true;
        }

        bool Visualizer::isInsideGraphViewport(const sf::Vector2f& mousePos) const {
            return gGraphViewport.contains(mousePos);
        }

        sf::Vector2f Visualizer::screenToGraphNormalized(const sf::Vector2f& mousePos) const {
            const float nx = (mousePos.x - gGraphViewport.left) / std::max(1.0f, gGraphViewport.width);
            const float ny = (mousePos.y - gGraphViewport.top) / std::max(1.0f, gGraphViewport.height);
            return sf::Vector2f(std::clamp(nx, 0.05f, 0.95f), std::clamp(ny, 0.05f, 0.95f));
        }

        void Visualizer::buildPseudocode() {
            if (algorithmType_ == algo::AlgorithmType::Kruskal) {
                pseudocode_ = {
                    "1. Sort edges by weight",
                    "2. For each edge E in sorted edges",
                    "3. Mark E as candidate",
                    "4. If endpoints of E are in different sets",
                    "5. Accept E and union sets",
                    "6. Else reject E",
                    "7. Done"};
            } else {
                pseudocode_ = {
                    "1. Choose start node, add it to tree, push frontier edges",
                    "2. While priority queue is not empty",
                    "3. Pop minimum edge (u, v)",
                    "4. If v is already in tree, reject edge",
                    "5. Else accept edge and add v to tree",
                    "6. Push frontier edges from v",
                    "7. Done"};
            }
        }

        void Visualizer::onActionNewGraph() {
            pushUndoState();
            graph_.clear();
            adjacencyMatrix_.clear();
            nodeAlive_.clear();
            selectedNodeId_ = -1;
            placingNode_ = false;
            deletingNodeMode_ = false;
            addingEdgeMode_ = false;
            draggingNode_ = false;
            draggingNodeId_ = -1;
            pendingEdgeFrom_ = -1;
            pendingEdgeTo_ = -1;
            enteringEdgeWeight_ = false;
            edgeWeightInput_.clear();
            canvasMode_ = MstCanvasMode::Graph;
    zoomLevel_ = 1.0f;
    panOffset_ = {0.0f, 0.0f};
            animation_.clear();
            playing_ = false;
            state_ = AppState::Paused;
            timelineDirty_ = true;
        }

        void Visualizer::rebuildTimeline() {
            if (!isMstSelected()) {
                animation_.clear();
                pseudocode_.clear();
                playing_ = false;
                state_ = AppState::Paused;
                return;
            }

            buildPseudocode();
            int startNode = 0;
            bool hasSelected = false;
            for (const auto& node : graph_.getNodes()) {
                if (node.id == selectedNodeId_) {
                    hasSelected = true;
                    break;
                }
            }
            if (hasSelected) {
                startNode = selectedNodeId_;
            } else if (!graph_.getNodes().empty()) {
                startNode = graph_.getNodes().front().id;
            }

            const auto steps = algo::AlgorithmFactory::buildSteps(algorithmType_, graph_, startNode);
            animation_.setSteps(steps);
            playing_ = false;
            state_ = steps.empty() ? AppState::Error : AppState::Paused;
            timelineDirty_ = false;
        }

        void Visualizer::onSelectStructure(RenderViewKind kind) {
            selectedStructure_ = kind;
            currentScreen_ = Screen::Visualization;
            placingNode_ = false;
            deletingNodeMode_ = false;
            addingEdgeMode_ = false;
            draggingNode_ = false;
            draggingNodeId_ = -1;
            pendingEdgeFrom_ = -1;
            pendingEdgeTo_ = -1;
            enteringEdgeWeight_ = false;
            edgeWeightInput_.clear();
            canvasMode_ = MstCanvasMode::Graph;
    zoomLevel_ = 1.0f;
    panOffset_ = {0.0f, 0.0f};
            if (isMstSelected()) {
                setupDefaultGraph();
            }
            rebuildTimeline();
        }

        void Visualizer::onActionSample() {
            setupDefaultGraph();
            canvasMode_ = MstCanvasMode::Graph;
            placingNode_ = false;
            deletingNodeMode_ = false;
            addingEdgeMode_ = false;
            draggingNode_ = false;
            draggingNodeId_ = -1;
            pendingEdgeFrom_ = -1;
            pendingEdgeTo_ = -1;
            enteringEdgeWeight_ = false;
            edgeWeightInput_.clear();
    zoomLevel_ = 1.0f;
    panOffset_ = {0.0f, 0.0f};
            rebuildTimeline();
        }

        void Visualizer::onActionRandom() {
            pushUndoState();
            const int nodeCount = 5 + (std::rand() % 10);  // 5..14 nodes
            graph_ = GraphLoader::createRandomGraph(nodeCount, 30);
            selectedNodeId_ = -1;
            syncAdjacencyMatrixFromGraph();
            canvasMode_ = MstCanvasMode::Graph;
            placingNode_ = false;
            deletingNodeMode_ = false;
            addingEdgeMode_ = false;
            draggingNode_ = false;
            draggingNodeId_ = -1;
            pendingEdgeFrom_ = -1;
            pendingEdgeTo_ = -1;
            enteringEdgeWeight_ = false;
            edgeWeightInput_.clear();
    zoomLevel_ = 1.0f;
    panOffset_ = {0.0f, 0.0f};
            rebuildTimeline();
        }

        void Visualizer::onActionUndo() {
            if (undoStack_.empty() || playing_) {
                return;
            }

            GraphSnapshot snap = undoStack_.back();
            undoStack_.pop_back();

            graph_ = std::move(snap.graph);
            adjacencyMatrix_ = std::move(snap.adjacency);
            nodeAlive_ = std::move(snap.alive);
            selectedNodeId_ = snap.selectedNodeId;

            placingNode_ = false;
            deletingNodeMode_ = false;
            addingEdgeMode_ = false;
            pendingEdgeFrom_ = -1;
            pendingEdgeTo_ = -1;
            enteringEdgeWeight_ = false;
            edgeWeightInput_.clear();
            draggingNode_ = false;
            draggingNodeId_ = -1;

            animation_.clear();
            playing_ = false;
            state_ = AppState::Paused;
            timelineDirty_ = true;
        }

        void Visualizer::onActionAddNode() {
            const bool wasActive = placingNode_;
            placingNode_ = !wasActive;
            deletingNodeMode_ = false;
            draggingNode_ = false;
            draggingNodeId_ = -1;
            addingEdgeMode_ = false;
            pendingEdgeFrom_ = -1;
            pendingEdgeTo_ = -1;
            enteringEdgeWeight_ = false;
            edgeWeightInput_.clear();
            canvasMode_ = MstCanvasMode::Graph;
        }

        void Visualizer::onActionAddEdgeMode() {
            const bool wasActive = addingEdgeMode_ || enteringEdgeWeight_ || pendingEdgeFrom_ >= 0;
            addingEdgeMode_ = !wasActive;
            deletingNodeMode_ = false;
            draggingNode_ = false;
            draggingNodeId_ = -1;
            placingNode_ = false;
            pendingEdgeFrom_ = -1;
            pendingEdgeTo_ = -1;
            enteringEdgeWeight_ = false;
            edgeWeightInput_.clear();
            canvasMode_ = MstCanvasMode::Graph;
        }

        void Visualizer::onActionRemoveNode() {
            const bool wasActive = deletingNodeMode_;
            deletingNodeMode_ = !wasActive;
            placingNode_ = false;
            addingEdgeMode_ = false;
            pendingEdgeFrom_ = -1;
            pendingEdgeTo_ = -1;
            enteringEdgeWeight_ = false;
            edgeWeightInput_.clear();
            draggingNode_ = false;
            draggingNodeId_ = -1;
            canvasMode_ = MstCanvasMode::Graph;
        }

        void Visualizer::onActionKruskal() {
            algorithmType_ = algo::AlgorithmType::Kruskal;
            placingNode_ = false;
            deletingNodeMode_ = false;
            addingEdgeMode_ = false;
            pendingEdgeFrom_ = -1;
            pendingEdgeTo_ = -1;
            enteringEdgeWeight_ = false;
            edgeWeightInput_.clear();
            rebuildTimeline();
        }

        void Visualizer::onActionPrim() {
            algorithmType_ = algo::AlgorithmType::Prim;
            placingNode_ = false;
            deletingNodeMode_ = false;
            addingEdgeMode_ = false;
            pendingEdgeFrom_ = -1;
            pendingEdgeTo_ = -1;
            enteringEdgeWeight_ = false;
            edgeWeightInput_.clear();
            rebuildTimeline();
        }

        void Visualizer::onActionToggleAlgorithm() {
            if (algorithmType_ == algo::AlgorithmType::Kruskal) {
                onActionPrim();
            } else {
                onActionKruskal();
            }
        }

        void Visualizer::onActionBuild() {
            placingNode_ = false;
            deletingNodeMode_ = false;
            addingEdgeMode_ = false;
            pendingEdgeFrom_ = -1;
            pendingEdgeTo_ = -1;
            enteringEdgeWeight_ = false;
            edgeWeightInput_.clear();
            rebuildTimeline();
        }

        void Visualizer::onActionPrev() {
            animation_.movePrev();
            state_ = AppState::Paused;
            playing_ = false;
        }

        void Visualizer::onActionNext() {
            if (!animation_.moveNext()) {
                state_ = AppState::Finished;
                playing_ = false;
                return;
            }
            state_ = AppState::Paused;
        }

        void Visualizer::onActionPlayPause() {
            if (animation_.empty()) {
                return;
            }

            // If user presses Play at the end, restart from beginning for immediate replay.
            if (!playing_ && animation_.currentIndex() >= animation_.totalSteps() - 1) {
                animation_.moveToStart();
            }

            playing_ = !playing_;
            playbackMode_ = PlaybackMode::RunAtOnce;
            state_ = playing_ ? AppState::Animating : AppState::Paused;

            // Remove initial wait: advance one step immediately when entering Play.
            if (playing_) {
                if (!animation_.moveNext()) {
                    playing_ = false;
                    state_ = AppState::Finished;
                }
            }

            playClock_.restart();
        }

        void Visualizer::onActionEnd() {
            animation_.moveToEnd();
            playing_ = false;
            state_ = AppState::Finished;
        }

        void Visualizer::run() {
            const std::vector<std::string> uiFontCandidates = {
                "assets/fonts/JetBrainsMono-2.304/fonts/ttf/JetBrainsMono-Regular.ttf",
                "../assets/fonts/JetBrainsMono-2.304/fonts/ttf/JetBrainsMono-Regular.ttf",
                "../../assets/fonts/JetBrainsMono-2.304/fonts/ttf/JetBrainsMono-Regular.ttf",
                "C:/Windows/Fonts/segoeui.ttf",
                "C:/Windows/Fonts/arial.ttf"};

            const std::vector<std::string> monoFontCandidates = {
                "assets/fonts/JetBrainsMono-2.304/fonts/ttf/JetBrainsMono-Regular.ttf",
                "../assets/fonts/JetBrainsMono-2.304/fonts/ttf/JetBrainsMono-Regular.ttf",
                "../../assets/fonts/JetBrainsMono-2.304/fonts/ttf/JetBrainsMono-Regular.ttf",
                "C:/Windows/Fonts/consola.ttf",
                "C:/Windows/Fonts/cour.ttf"};

            if (!loadFontFromCandidates(font_, uiFontCandidates)) {
                return;
            }

            if (!loadFontFromCandidates(monoFont_, monoFontCandidates)) {
                return;
            }

            sf::Texture homeIconTex;
            sf::Texture playIconTex;
            sf::Texture pauseIconTex;
            sf::Texture skipBackIconTex;
            sf::Texture skipForwardIconTex;

            const auto makeIconCandidates = [](const std::string& file) {
                return std::vector<std::string>{
                    "assets/images/" + file,
                    "../assets/images/" + file,
                    "../../assets/images/" + file};
            };

            loadTextureFromCandidates(homeIconTex, makeIconCandidates("homeButton.png"));
            loadTextureFromCandidates(playIconTex, makeIconCandidates("startButton.png"));
            loadTextureFromCandidates(pauseIconTex, makeIconCandidates("pauseButton.png"));
            loadTextureFromCandidates(skipBackIconTex, makeIconCandidates("skipbackButton.png"));
            loadTextureFromCandidates(skipForwardIconTex, makeIconCandidates("skipforwardButton.png"));

            menuButtons_ = {
                Button("Doubly Linked List", font_),
                Button("Hash Table", font_),
                Button("Red-Black Tree", font_),
                Button("Minimum Spanning Tree", font_)};

            for (size_t i = 0; i < menuButtons_.size(); ++i) {
                menuButtons_[i].setSize(360.f, 44.f);
                menuButtons_[i].setPosition(420.f, 220.f + 58.f * static_cast<float>(i));
            }

            controlButtons_ = {
                Button("Skip Back", font_),
                Button("Step Back", font_),
                Button("Play/Pause", font_),
                Button("Step Forward", font_),
                Button("Skip Forward", font_),
                Button("New Graph", font_),
                Button("Undo", font_),
                Button("Add Node", font_),
                Button("Delete Node", font_),
                Button("Add/Modify Edges", font_),
                Button("Random", font_),
                Button("Build", font_),
                Button("View: Graph", font_),
                Button("Kruskal", font_),
                Button("Prim", font_)};

            backButton_ = Button("Back To Menu", font_);
            backButton_.setLabel("");
            backButton_.setSize(40.f, 40.f);
            backButton_.setPosition(20.f, 20.f);

            const float buttonW = 220.f;
            const float buttonH = 28.f;
            const float leftX = 26.f;
            const float graphSetupY = 76.f;
            const float graphGap = 36.f;

            // Left panel: Graph Setup controls.
            controlButtons_[5].setSize(buttonW, buttonH);
            controlButtons_[5].setPosition(leftX, graphSetupY + graphGap * 0.f);
            controlButtons_[6].setSize(buttonW, buttonH);
            controlButtons_[6].setPosition(leftX, graphSetupY + graphGap * 1.f);
            controlButtons_[7].setSize(buttonW, buttonH);
            controlButtons_[7].setPosition(leftX, graphSetupY + graphGap * 2.f);
            controlButtons_[8].setSize(buttonW, buttonH);
            controlButtons_[8].setPosition(leftX, graphSetupY + graphGap * 3.f);
            controlButtons_[9].setSize(buttonW, buttonH);
            controlButtons_[9].setPosition(leftX, graphSetupY + graphGap * 4.f);
            controlButtons_[10].setSize(buttonW, buttonH);
            controlButtons_[10].setPosition(leftX, graphSetupY + graphGap * 5.f);
            controlButtons_[11].setSize(buttonW, buttonH);
            controlButtons_[11].setPosition(leftX, graphSetupY + graphGap * 6.f);
            controlButtons_[12].setSize(buttonW, buttonH);
            controlButtons_[12].setPosition(leftX, graphSetupY + graphGap * 7.f);

            // Left panel: Algorithms controls.40
            controlButtons_[13].setSize(buttonW, buttonH);
            controlButtons_[13].setPosition(leftX, 436.f);
            controlButtons_[13].setPosition(leftX, 400.f);
            controlButtons_[14].setSize(buttonW, buttonH);
            controlButtons_[14].setPosition(leftX, 390.f);
            controlButtons_[14].setPosition(leftX, 436.f);

            // Bottom panel: Playback bar controls.
            const float pbY = 642.f;
            const float pbW = 134.f;
            const float pbGap = 10.f;
            const float pbStartX = 24.f;
            for (int i = 0; i <= 4; ++i) {
                controlButtons_[i].setSize(pbW, 28.f);
                controlButtons_[i].setPosition(pbStartX + static_cast<float>(i) * (pbW + pbGap), pbY);
            }

            controlButtons_[0].setLabel("");
            controlButtons_[1].setLabel("Step back");
            controlButtons_[2].setLabel("");
            controlButtons_[3].setLabel("Step forward");
            controlButtons_[4].setLabel("");

            speedSlider_ = SpeedSlider(940.f, 650.f, 150.f, 1.f, 10.f, 2.f, font_);

            controlButtons_[2].setStyleRole(Button::StyleRole::Play);
            controlButtons_[8].setStyleRole(Button::StyleRole::Danger);
            controlButtons_[13].setStyleRole(Button::StyleRole::Algorithm);
            controlButtons_[14].setStyleRole(Button::StyleRole::Algorithm);

        sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
        unsigned int winW = desktop.width;
        unsigned int winH = desktop.height > 80 ? desktop.height - 80 : desktop.height;

        sf::RenderWindow window(sf::VideoMode(winW, winH), "Data Structure Visualization");
            window.setVerticalSyncEnabled(true);
            window.setFramerateLimit(60);
            window.setView(
                sf::View(sf::FloatRect(0.0f, 0.0f, static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y))));

            sf::Vector2u prevWindowSize = window.getSize();

            while (window.isOpen()) {
                const UILayout layout = computeUILayout(window.getSize(), showLeftActions_, showRightStepPanel_, showRightPseudocodePanel_);
        
        float cx = layout.graphViewport.left + layout.graphViewport.width * 0.5f + panOffset_.x;
        float cy = layout.graphViewport.top + layout.graphViewport.height * 0.5f + panOffset_.y;
        float newW = layout.graphViewport.width * zoomLevel_;
        float newH = layout.graphViewport.height * zoomLevel_;
        gGraphViewport = sf::FloatRect(cx - newW * 0.5f, cy - newH * 0.5f, newW, newH);

                sf::Event event;
                while (window.pollEvent(event)) {
                    if (event.type == sf::Event::Closed) {
                        window.close();
                    }

                    if (event.type == sf::Event::Resized) {
                        const sf::Vector2u newSize(event.size.width, event.size.height);
                        window.setView(sf::View(sf::FloatRect(
                            0.0f, 0.0f, static_cast<float>(event.size.width), static_cast<float>(event.size.height))));

                        if (newSize.x > prevWindowSize.x || newSize.y > prevWindowSize.y) {
                            showRightStepPanel_ = true;
                            showRightPseudocodePanel_ = true;
                        }
                        prevWindowSize = newSize;
                    }

            if (event.type == sf::Event::MouseWheelScrolled && currentScreen_ == Screen::Visualization && isMstSelected() && canvasMode_ == MstCanvasMode::Graph) {
                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                    const sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseWheelScroll.x, event.mouseWheelScroll.y));
                    if (layout.graphViewport.contains(mousePos)) {
                        float zoomFactor = (event.mouseWheelScroll.delta > 0) ? 1.15f : (1.0f / 1.15f);
                        float nextZoom = std::clamp(zoomLevel_ * zoomFactor, 0.2f, 5.0f);
                        
                        float oldCx = layout.graphViewport.left + layout.graphViewport.width * 0.5f + panOffset_.x;
                        float oldCy = layout.graphViewport.top + layout.graphViewport.height * 0.5f + panOffset_.y;
                        float dx = mousePos.x - oldCx;
                        float dy = mousePos.y - oldCy;
                        
                        // Giữ trọng tâm vị trí con trỏ chuột khi zoom
                        panOffset_.x -= dx * (nextZoom / zoomLevel_ - 1.0f);
                        panOffset_.y -= dy * (nextZoom / zoomLevel_ - 1.0f);
                        zoomLevel_ = nextZoom;
                        
                        float ncx = layout.graphViewport.left + layout.graphViewport.width * 0.5f + panOffset_.x;
                        float ncy = layout.graphViewport.top + layout.graphViewport.height * 0.5f + panOffset_.y;
                        gGraphViewport = sf::FloatRect(ncx - (layout.graphViewport.width * zoomLevel_) * 0.5f, 
                                                       ncy - (layout.graphViewport.height * zoomLevel_) * 0.5f, 
                                                       layout.graphViewport.width * zoomLevel_, 
                                                       layout.graphViewport.height * zoomLevel_);
                    }
                }
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
                if (currentScreen_ == Screen::Visualization && isMstSelected() && canvasMode_ == MstCanvasMode::Graph) {
                    const sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                    if (layout.graphViewport.contains(mousePos)) {
                        panning_ = true;
                        lastMousePos_ = mousePos;
                    }
                }
            }

            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Right) {
                panning_ = false;
            }

                    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                        const sf::Vector2f mousePos =
                            window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

                        if (currentScreen_ == Screen::Menu) {
                            for (size_t i = 0; i < menuButtons_.size(); ++i) {
                                if (!menuButtons_[i].contains(mousePos)) {
                                    continue;
                                }
                                menuButtons_[i].flash();
                                switch (i) {
                                    case 0: onSelectStructure(RenderViewKind::DoublyLinkedList); break;
                                    case 1: onSelectStructure(RenderViewKind::HashTable); break;
                                    case 2: onSelectStructure(RenderViewKind::RedBlackTree); break;
                                    case 3: onSelectStructure(RenderViewKind::MST); break;
                                    default: break;
                                }
                                break;
                            }
                        } else {
                            if (backButton_.contains(mousePos)) {
                                backButton_.flash();
                                currentScreen_ = Screen::Menu;
                                playing_ = false;
                                continue;
                            }

                            if (isMstSelected()) {
                                const sf::FloatRect leftCollapseBtn = layout.leftCollapseBtn;
                                const sf::FloatRect rightStepBtn = layout.rightStepToggleBtn;
                                const sf::FloatRect rightPseudoBtn = layout.rightPseudoToggleBtn;

                                if (leftCollapseBtn.contains(mousePos)) {
                                    showLeftActions_ = !showLeftActions_;
                                    continue;
                                }

                                if (rightStepBtn.contains(mousePos)) {
                                    showRightStepPanel_ = !showRightStepPanel_;
                                    continue;
                                }

                                if (rightPseudoBtn.contains(mousePos)) {
                                    showRightPseudocodePanel_ = !showRightPseudocodePanel_;
                                    continue;
                                }

                                if (playing_ && speedSlider_.contains(mousePos)) {
                                    speedSlider_.setActive(true);
                                    speedSlider_.onMouseMoved(mousePos);
                                    speed_ = speedSlider_.getValue();
                                    if (playing_) {
                                        playClock_.restart();
                                    }
                                    continue;
                                }

                                bool consumed = false;
                                for (size_t i = 0; i < controlButtons_.size(); ++i) {
                                    if (!controlButtons_[i].contains(mousePos)) {
                                        continue;
                                    }

                                    controlButtons_[i].flash();
                                    switch (i) {
                                        case 0:
                                            animation_.moveToStart();
                                            playing_ = false;
                                            state_ = AppState::Paused;
                                            break;
                                        case 1: onActionPrev(); break;
                                        case 2: onActionPlayPause(); break;
                                        case 3: onActionNext(); break;
                                        case 4: onActionEnd(); break;
                                        case 5: onActionNewGraph(); break;
                                        case 6: onActionUndo(); break;
                                        case 7: onActionAddNode(); break;
                                        case 8: onActionRemoveNode(); break;
                                        case 9: onActionAddEdgeMode(); break;
                                        case 10: onActionRandom(); break;
                                        case 11: onActionBuild(); break;
                                        case 12:
                                            canvasMode_ = (canvasMode_ == MstCanvasMode::Graph) ? MstCanvasMode::Matrix
                                                                                                : MstCanvasMode::Graph;
                                            break;
                                        case 13: onActionKruskal(); break;
                                        case 14: onActionPrim(); break;
                                        default: break;
                                    }
                                    consumed = true;
                                    break;
                                }

                                if (!consumed && !enteringEdgeWeight_ && canvasMode_ == MstCanvasMode::Matrix &&
                                    handleAdjacencyMatrixClick(mousePos)) {
                                    consumed = true;
                                }

                                if (!consumed && !enteringEdgeWeight_ && canvasMode_ == MstCanvasMode::Graph) {
                                    if (placingNode_ && isInsideGraphViewport(mousePos)) {
                                        pushUndoState();
                                        const sf::Vector2f npos = screenToGraphNormalized(mousePos);
                                        const int n = static_cast<int>(adjacencyMatrix_.size());
                                        for (auto& row : adjacencyMatrix_) {
                                            row.push_back(0);
                                        }
                                        adjacencyMatrix_.push_back(std::vector<int>(static_cast<size_t>(n + 1), 0));
                                        nodeAlive_.push_back(true);

                                        rebuildGraphFromAdjacencyMatrix();
                                        graph_.setNodePosition(n, npos.x, npos.y);
                                        selectedNodeId_ = n;
                                        placingNode_ = false;
                                        animation_.clear();
                                        playing_ = false;
                                        state_ = AppState::Paused;
                                        timelineDirty_ = true;
                                        consumed = true;
                                    }
                                }

                                if (!consumed && !enteringEdgeWeight_ && canvasMode_ == MstCanvasMode::Graph) {
                                    const int hitNode = hitTestNode(mousePos);
                                    if (deletingNodeMode_) {
                                        const int aliveCount = static_cast<int>(graph_.getNodes().size());
                                        if (hitNode > 0 && aliveCount > 1 && hitNode < static_cast<int>(adjacencyMatrix_.size()) &&
                                            hitNode < static_cast<int>(nodeAlive_.size()) &&
                                            nodeAlive_[static_cast<size_t>(hitNode)]) {
                                            pushUndoState();
                                            nodeAlive_[static_cast<size_t>(hitNode)] = false;
                                            for (int i = 0; i < static_cast<int>(adjacencyMatrix_.size()); ++i) {
                                                adjacencyMatrix_[static_cast<size_t>(hitNode)][static_cast<size_t>(i)] = 0;
                                                adjacencyMatrix_[static_cast<size_t>(i)][static_cast<size_t>(hitNode)] = 0;
                                            }
                                            selectedNodeId_ = -1;
                                            pendingEdgeFrom_ = -1;
                                            pendingEdgeTo_ = -1;
                                            enteringEdgeWeight_ = false;
                                            edgeWeightInput_.clear();
                                            rebuildGraphFromAdjacencyMatrix();
                                            animation_.clear();
                                            playing_ = false;
                                            state_ = AppState::Paused;
                                            timelineDirty_ = true;
                                        }
                                        consumed = true;
                                    } else if (addingEdgeMode_) {
                                        if (hitNode >= 0) {
                                            if (pendingEdgeFrom_ < 0) {
                                                pendingEdgeFrom_ = hitNode;
                                                selectedNodeId_ = hitNode;
                                            } else if (pendingEdgeFrom_ != hitNode) {
                                                pendingEdgeTo_ = hitNode;
                                                enteringEdgeWeight_ = true;
                                                edgeWeightInput_.clear();
                                            }
                                        }
                                        consumed = true;
                                    } else {
                                        selectedNodeId_ = hitNode;
                                        if (hitNode >= 0 && !playing_) {
                                            draggingNode_ = true;
                                            draggingNodeId_ = hitNode;
                                            placingNode_ = false;
                                        }
                                    }
                                }
                            }
                        }
                    }

                    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                        speedSlider_.setActive(false);
                        draggingNode_ = false;
                        draggingNodeId_ = -1;
                    }

                    if (event.type == sf::Event::MouseMoved) {
                const sf::Vector2f mousePos =
                    window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));

                if (panning_) {
                    panOffset_ += (mousePos - lastMousePos_);
                    lastMousePos_ = mousePos;
                    
                    float cx = layout.graphViewport.left + layout.graphViewport.width * 0.5f + panOffset_.x;
                    float cy = layout.graphViewport.top + layout.graphViewport.height * 0.5f + panOffset_.y;
                    gGraphViewport = sf::FloatRect(cx - (layout.graphViewport.width * zoomLevel_) * 0.5f, 
                                                   cy - (layout.graphViewport.height * zoomLevel_) * 0.5f, 
                                                   layout.graphViewport.width * zoomLevel_, 
                                                   layout.graphViewport.height * zoomLevel_);
                }

                        if (currentScreen_ == Screen::Visualization && isMstSelected()) {
                            const float prevSpeed = speed_;
                            if (playing_) {
                                speedSlider_.onMouseMoved(mousePos);
                                speed_ = speedSlider_.getValue();
                                if (std::abs(speed_ - prevSpeed) > 0.001f) {
                                    playClock_.restart();
                                }
                            }

                            if (canvasMode_ == MstCanvasMode::Graph && draggingNode_ && draggingNodeId_ >= 0 && !playing_) {
                                if (isInsideGraphViewport(mousePos)) {
                                    const sf::Vector2f npos = screenToGraphNormalized(mousePos);
                                    graph_.setNodePosition(draggingNodeId_, npos.x, npos.y);
                                }
                            }
                        }
                    }

                    if (event.type == sf::Event::TextEntered && currentScreen_ == Screen::Visualization && isMstSelected() &&
                        enteringEdgeWeight_) {
                        const sf::Uint32 uni = event.text.unicode;
                        if (uni >= '0' && uni <= '9') {
                            if (edgeWeightInput_.size() < 3) {
                                edgeWeightInput_.push_back(static_cast<char>(uni));
                            }
                        } else if (uni == 8) {
                            if (!edgeWeightInput_.empty()) {
                                edgeWeightInput_.pop_back();
                            }
                        } else if (uni == 13) {
                            int w = 1;
                            if (!edgeWeightInput_.empty()) {
                                w = std::stoi(edgeWeightInput_);
                            }
                            w = std::clamp(w, 1, 999);

                            const int n = static_cast<int>(adjacencyMatrix_.size());
                            if (pendingEdgeFrom_ >= 0 && pendingEdgeTo_ >= 0 && pendingEdgeFrom_ < n && pendingEdgeTo_ < n &&
                                pendingEdgeFrom_ != pendingEdgeTo_) {
                                pushUndoState();
                                adjacencyMatrix_[static_cast<size_t>(pendingEdgeFrom_)][static_cast<size_t>(pendingEdgeTo_)] = w;
                                adjacencyMatrix_[static_cast<size_t>(pendingEdgeTo_)][static_cast<size_t>(pendingEdgeFrom_)] = w;
                                rebuildGraphFromAdjacencyMatrix();
                                animation_.clear();
                                playing_ = false;
                                state_ = AppState::Paused;
                                timelineDirty_ = true;
                            }

                            pendingEdgeFrom_ = -1;
                            pendingEdgeTo_ = -1;
                            addingEdgeMode_ = false;
                            enteringEdgeWeight_ = false;
                            edgeWeightInput_.clear();
                        } else if (uni == 27) {
                            pendingEdgeFrom_ = -1;
                            pendingEdgeTo_ = -1;
                            enteringEdgeWeight_ = false;
                            edgeWeightInput_.clear();
                        }
                    }

                    if (event.type == sf::Event::KeyPressed && currentScreen_ == Screen::Visualization && isMstSelected() &&
                        enteringEdgeWeight_ && event.key.code == sf::Keyboard::Escape) {
                        pendingEdgeFrom_ = -1;
                        pendingEdgeTo_ = -1;
                        enteringEdgeWeight_ = false;
                        edgeWeightInput_.clear();
                    }

                    if (event.type == sf::Event::KeyPressed && currentScreen_ == Screen::Visualization && isMstSelected() &&
                        !enteringEdgeWeight_ && event.key.control && event.key.code == sf::Keyboard::Z) {
                        onActionUndo();
                    }

                    if (event.type == sf::Event::KeyPressed && currentScreen_ == Screen::Visualization && isMstSelected() &&
                        !enteringEdgeWeight_ && (event.key.code == sf::Keyboard::Space || event.key.code == sf::Keyboard::Right)) {
                        if (!timelineDirty_ && !animation_.empty()) {
                            playing_ = false;
                            onActionNext();
                        }
                    }
                }

                if (currentScreen_ == Screen::Visualization && isMstSelected() && playing_ && !animation_.empty() &&
                    playClock_.getElapsedTime().asSeconds() >= (1.0f / speed_)) {
                    playClock_.restart();
                    if (!animation_.moveNext()) {
                        playing_ = false;
                        state_ = AppState::Finished;
                    }
                }

                const Step* step = animation_.currentStep();

                if (currentScreen_ == Screen::Visualization && isMstSelected() && controlButtons_.size() >= 5) {
                    const bool hasTimeline = !animation_.empty();
                    const bool atStart = !hasTimeline || animation_.currentIndex() <= 0;
                    const bool atEnd = !hasTimeline || animation_.currentIndex() >= animation_.totalSteps() - 1;
                    const int n = static_cast<int>(graph_.getNodes().size());
                    const bool canDeleteAny = n > 1;

                    const float sidePad = 18.0f; // Tăng lề (margin) 2 bên để nút gọn vào giữa hơn
                    const float leftX = layout.leftPanel.left + sidePad;
                    const float buttonW = layout.leftPanel.width - sidePad * 2.0f;
                    const float buttonH = 28.0f; // Tăng chiều cao để nút trông thanh thoát
                    const float graphSetupY = layout.leftPanel.top + 52.0f;
                    const float graphGap = 36.0f; // Tăng khoảng trắng (padding) giữa các nút

                    // Left panel: Graph Setup controls.
                    for (int i = 5; i <= 12; ++i) {
                        controlButtons_[i].setSize(buttonW, buttonH);
                        controlButtons_[i].setPosition(leftX, graphSetupY + graphGap * static_cast<float>(i - 5));
                    }

                    // Left panel: Algorithms controls.
                    controlButtons_[13].setSize(buttonW, buttonH);
                    controlButtons_[13].setPosition(leftX, layout.leftPanel.top + 394.0f);
                    controlButtons_[14].setSize(buttonW, buttonH);
                    controlButtons_[14].setPosition(leftX, layout.leftPanel.top + 430.0f);

                // Animate task bar progress (0.0 = Paused/Detailed, 1.0 = Playing/Compact)
                if (playing_) {
                    if (taskBarAnimProgress_ < 1.0f) {
                        taskBarAnimProgress_ += 0.08f;
                        if (taskBarAnimProgress_ > 1.0f) taskBarAnimProgress_ = 1.0f;
                    }
                } else {
                    if (taskBarAnimProgress_ > 0.0f) {
                        taskBarAnimProgress_ -= 0.08f;
                        if (taskBarAnimProgress_ < 0.0f) taskBarAnimProgress_ = 0.0f;
                    }
                }

                    // Bottom task bar morphs between compact run mode and detailed pause mode.
                const float smallW = 54.0f;
                const float longW = 130.0f;
                const float hBtn = 28.0f;
                const float baseGap = 10.0f;
                const float y = layout.bottomButtonsArea.top + (layout.bottomButtonsArea.height - hBtn) * 0.5f;
                float x = layout.bottomButtonsArea.left;
                
                float t = taskBarAnimProgress_;
                
                controlButtons_[0].setSize(smallW, hBtn);
                controlButtons_[0].setPosition(x, y);
                x += smallW + baseGap;
                
                controlButtons_[2].setSize(smallW, hBtn);
                controlButtons_[2].setPosition(x, y);
                x += smallW + baseGap;
                
                float currentLongW = smoothLerp(longW, 0.0f, t);
                float currentGap = smoothLerp(baseGap, 0.0f, t);
                
                if (currentLongW > 1.0f) {
                    controlButtons_[1].setSize(currentLongW, hBtn);
                    controlButtons_[1].setPosition(x, y);
                    x += currentLongW + currentGap;
                    
                    controlButtons_[3].setSize(currentLongW, hBtn);
                    controlButtons_[3].setPosition(x, y);
                    x += currentLongW + currentGap;
                } else {
                    controlButtons_[1].setPosition(-500.f, -500.f);
                    controlButtons_[3].setPosition(-500.f, -500.f);
                }
                
                controlButtons_[4].setSize(smallW, hBtn);
                controlButtons_[4].setPosition(x, y);

                    const float sliderX = layout.bottomSliderArea.left;
                    const float sliderY = layout.bottomSliderArea.top + layout.bottomSliderArea.height * 0.5f;
                    speedSlider_.setPosition(sliderX, sliderY);

                    controlButtons_[0].setEnabled(!timelineDirty_ && hasTimeline && !atStart);          // Skip Back
                    controlButtons_[1].setEnabled(!timelineDirty_ && hasTimeline && !atStart && !playing_);  // Step Back
                    controlButtons_[2].setEnabled(!timelineDirty_ && hasTimeline);                      // Play/Pause
                    controlButtons_[3].setEnabled(!timelineDirty_ && hasTimeline && !atEnd && !playing_);  // Step Forward
                    controlButtons_[4].setEnabled(!timelineDirty_ && hasTimeline && !atEnd);            // Skip Forward

                    controlButtons_[5].setEnabled(!playing_);    // New Graph
                    controlButtons_[6].setEnabled(!playing_ && !undoStack_.empty()); // Undo
                    controlButtons_[7].setEnabled(!playing_);    // Add Node
                    controlButtons_[8].setEnabled(!playing_ && canDeleteAny); // Delete Node
                    controlButtons_[9].setEnabled(!playing_ && canvasMode_ == MstCanvasMode::Graph); // Add/Modify
                    controlButtons_[10].setEnabled(!playing_);   // Random
                    controlButtons_[11].setEnabled(!playing_);   // Build
                    controlButtons_[12].setEnabled(!playing_);   // View
                    controlButtons_[13].setEnabled(!playing_);   // Kruskal
                    controlButtons_[14].setEnabled(!playing_);   // Prim

                    if (!showLeftActions_) {
                        for (int i = 5; i <= 14; ++i) {
                            controlButtons_[i].setEnabled(false);
                        }
                    }

                    controlButtons_[12].setLabel(canvasMode_ == MstCanvasMode::Graph ? "View: Matrix" : "View: Graph");
                    controlButtons_[11].setLabel(timelineDirty_ ? "Build *" : "Build");

                    controlButtons_[7].setSelected(placingNode_);
                    controlButtons_[11].setSelected(timelineDirty_);
                    controlButtons_[8].setSelected(deletingNodeMode_);
                    controlButtons_[9].setSelected(addingEdgeMode_ || enteringEdgeWeight_ || pendingEdgeFrom_ >= 0);
                    controlButtons_[12].setSelected(canvasMode_ == MstCanvasMode::Matrix);
                    controlButtons_[13].setSelected(algorithmType_ == algo::AlgorithmType::Kruskal);
                    controlButtons_[14].setSelected(algorithmType_ == algo::AlgorithmType::Prim);
                }

                window.clear(config::kBackgroundColor);

                if (currentScreen_ == Screen::Menu) {
                    sf::Text title("Choose Data Structure", font_, 34);
                    title.setPosition(430.f, 130.f);
                    title.setFillColor(sf::Color(48, 66, 80));
                    window.draw(title);

                    for (auto& b : menuButtons_) {
                        b.draw(window);
                    }
                } else {
                    RenderViewModel vm;
                    vm.kind = selectedStructure_;
                    vm.graph = &graph_;
                    vm.selectedNodeId = selectedNodeId_;
            vm.graphViewport = gGraphViewport;
                    if (step != nullptr) {
                        vm.highlightedEdges = step->highlightedEdges;
                        vm.candidateEdges = step->candidateEdges;
                        vm.highlightedNodes = step->highlightedNodes;
                        vm.currentStepEvent = step->event;
                    }
                    if (!isMstSelected() || canvasMode_ == MstCanvasMode::Graph) {
                        Renderer::draw(window, vm, font_);
                    }

                    if (isMstSelected()) {
                        const float rightContentW = layout.rightStepBox.width;
                        const float rightTextMax = rightContentW - 24.0f;
                        const float rightInnerX = layout.rightPanel.left + kRightContentInset;
                        const sf::FloatRect rightStepBox = layout.rightStepBox;
                        const sf::FloatRect rightPseudoBox = layout.rightPseudoBox;

                        if (showLeftActions_) {
                            // Vẽ bóng đổ (Shadow) giống code WelcomeScreen
                            for (int i = 0; i < 8; ++i) {
                                sf::ConvexShape shadow = createRoundedRect(sf::Vector2f(layout.leftPanel.width + i, layout.leftPanel.height + i), 18.0f + i / 2.0f);
                                shadow.setPosition(sf::Vector2f(layout.leftPanel.left - i / 2.0f, layout.leftPanel.top + 6.0f - i / 2.0f));
                                shadow.setFillColor(sf::Color(0, 0, 0, 8));
                                window.draw(shadow);
                            }
                            // Vẽ nền Gradient
                            sf::VertexArray leftBg = createGradientRoundedRect(
                                sf::Vector2f(layout.leftPanel.width, layout.leftPanel.height),
                                18.0f,
                                sf::Vector2f(layout.leftPanel.left, layout.leftPanel.top),
                                sf::Color(45, 52, 71), // cBottomLeft (Xanh dương sẫm)
                                sf::Color(31, 37, 52)  // cTopRight (Xanh đậm hơn)
                            );
                            window.draw(leftBg);
                        }

                        if (showRightStepPanel_) {
                            for (int i = 0; i < 8; ++i) {
                                sf::ConvexShape shadow = createRoundedRect(sf::Vector2f(rightStepBox.width + i, rightStepBox.height + i), 18.0f + i / 2.0f);
                                shadow.setPosition(sf::Vector2f(rightStepBox.left - i / 2.0f, rightStepBox.top + 6.0f - i / 2.0f));
                                shadow.setFillColor(sf::Color(0, 0, 0, 8));
                                window.draw(shadow);
                            }
                            sf::VertexArray stepBg = createGradientRoundedRect(
                                sf::Vector2f(rightStepBox.width, rightStepBox.height),
                                18.0f,
                                sf::Vector2f(rightStepBox.left, rightStepBox.top),
                                sf::Color(45, 52, 71),
                                sf::Color(31, 37, 52)
                            );
                            window.draw(stepBg);
                        }

                        if (showRightPseudocodePanel_) {
                            for (int i = 0; i < 8; ++i) {
                                sf::ConvexShape shadow = createRoundedRect(sf::Vector2f(rightPseudoBox.width + i, rightPseudoBox.height + i), 18.0f + i / 2.0f);
                                shadow.setPosition(sf::Vector2f(rightPseudoBox.left - i / 2.0f, rightPseudoBox.top + 6.0f - i / 2.0f));
                                shadow.setFillColor(sf::Color(0, 0, 0, 8));
                                window.draw(shadow);
                            }
                            sf::VertexArray pseudoBg = createGradientRoundedRect(
                                sf::Vector2f(rightPseudoBox.width, rightPseudoBox.height),
                                18.0f,
                                sf::Vector2f(rightPseudoBox.left, rightPseudoBox.top),
                                sf::Color(45, 52, 71),
                                sf::Color(31, 37, 52)
                            );
                            window.draw(pseudoBg);
                        }

                        for (int i = 0; i < 8; ++i) {
                            sf::ConvexShape shadow = createRoundedRect(sf::Vector2f(layout.bottomPanel.width + i, layout.bottomPanel.height + i), 14.0f + i / 2.0f);
                            shadow.setPosition(sf::Vector2f(layout.bottomPanel.left - i / 2.0f, layout.bottomPanel.top + 6.0f - i / 2.0f));
                            shadow.setFillColor(sf::Color(0, 0, 0, 8));
                            window.draw(shadow);
                        }
                        sf::VertexArray bottomBg = createGradientRoundedRect(
                            sf::Vector2f(layout.bottomPanel.width, layout.bottomPanel.height),
                            14.0f,
                            sf::Vector2f(layout.bottomPanel.left, layout.bottomPanel.top),
                            sf::Color(45, 52, 71),
                            sf::Color(31, 37, 52)
                        );
                        window.draw(bottomBg);

                        for (size_t i = 0; i < controlButtons_.size(); ++i) {
                            if (!showLeftActions_ && i >= 5 && i <= 14) {
                                continue;
                            }
                            if (playing_ && (i == 1 || i == 3)) {
                                continue;
                            }
                            controlButtons_[i].draw(window);
                        }
                        if (playing_) {
                            speedSlider_.draw(window);
                        }

                        std::string actionText = "Build to generate algorithm steps";
                        if (step != nullptr) {
                            actionText = "Step " + std::to_string(animation_.currentIndex()) + ": " + step->description;
                        }
                        sf::Text statusAction(actionText, monoFont_, 13);
                        statusAction.setPosition(layout.bottomStatusTextArea.left,
                                                layout.bottomStatusTextArea.top + layout.bottomStatusTextArea.height * 0.5f - 9.0f);
                        statusAction.setFillColor(sf::Color::White);
                        clampTextToWidth(statusAction, layout.bottomStatusTextArea.width);
                        window.draw(statusAction);

                        // Left collapse control.
                        drawRoundedBox(window,
                                    layout.leftCollapseBtn,
                                    14.0f,
                                    1.0f,
                                    sf::Color(95, 131, 151),
                                    sf::Color(73, 106, 125));
                        sf::Text collapseLabel(showLeftActions_ ? "<" : ">", monoFont_, 16);
                        const sf::FloatRect collapseBounds = collapseLabel.getLocalBounds();
                        collapseLabel.setPosition(layout.leftCollapseBtn.left + (layout.leftCollapseBtn.width - collapseBounds.width) * 0.5f - collapseBounds.left,
                                                layout.leftCollapseBtn.top + (layout.leftCollapseBtn.height - collapseBounds.height) * 0.5f - collapseBounds.top - 2.f);
                        collapseLabel.setFillColor(sf::Color::White);
                        window.draw(collapseLabel);

                        // Right side toggles: step info and pseudocode visibility.
                        drawRoundedBox(window,
                                    layout.rightStepToggleBtn,
                                    14.0f,
                                    1.0f,
                                    showRightStepPanel_ ? sf::Color(83, 117, 136) : sf::Color(124, 150, 165),
                                    sf::Color(65, 95, 111));
                        sf::Text stepToggle(showRightStepPanel_ ? ">" : "<", monoFont_, 16);
                        stepToggle.setFillColor(sf::Color::White);
                        sf::FloatRect stepBounds = stepToggle.getLocalBounds();
                        stepToggle.setPosition(layout.rightStepToggleBtn.left + (layout.rightStepToggleBtn.width - stepBounds.width) * 0.5f - stepBounds.left,
                                               layout.rightStepToggleBtn.top + (layout.rightStepToggleBtn.height - stepBounds.height) * 0.5f - stepBounds.top - 2.f);
                        window.draw(stepToggle);

                        drawRoundedBox(window,
                                    layout.rightPseudoToggleBtn,
                                    14.0f,
                                    1.0f,
                                    showRightPseudocodePanel_ ? sf::Color(83, 117, 136) : sf::Color(124, 150, 165),
                                    sf::Color(65, 95, 111));
                        sf::Text pseudoToggle(showRightPseudocodePanel_ ? ">" : "<", monoFont_, 16);
                        pseudoToggle.setFillColor(sf::Color::White);
                        sf::FloatRect pseudoBounds = pseudoToggle.getLocalBounds();
                        pseudoToggle.setPosition(layout.rightPseudoToggleBtn.left + (layout.rightPseudoToggleBtn.width - pseudoBounds.width) * 0.5f - pseudoBounds.left,
                                                 layout.rightPseudoToggleBtn.top + (layout.rightPseudoToggleBtn.height - pseudoBounds.height) * 0.5f - pseudoBounds.top - 2.f);
                        window.draw(pseudoToggle);

                        // Icon overlays for image-enhanced controls.
                        auto drawIconInButton = [&](const sf::Texture& tex, const Button& btn, const sf::Color& tint) {
                            if (tex.getSize().x == 0 || tex.getSize().y == 0) {
                                return;
                            }
                            const sf::FloatRect rect = btn.bounds();
                            sf::Sprite sp(tex);
                            const float targetH = std::max(12.0f, rect.height * 0.62f);
                            const float scale = targetH / static_cast<float>(tex.getSize().y);
                            sp.setScale(scale, scale);
                            const float w = static_cast<float>(tex.getSize().x) * scale;
                            const float h = static_cast<float>(tex.getSize().y) * scale;
                            sp.setPosition(rect.left + (rect.width - w) * 0.5f, rect.top + (rect.height - h) * 0.5f);
                            sp.setColor(tint);
                            window.draw(sp);
                        };

                        drawIconInButton(skipBackIconTex, controlButtons_[0], sf::Color(28, 32, 36));
                        drawIconInButton(playing_ ? pauseIconTex : playIconTex, controlButtons_[2], sf::Color(28, 32, 36));
                        drawIconInButton(skipForwardIconTex, controlButtons_[4], sf::Color(28, 32, 36));

                        const sf::FloatRect backRect = backButton_.bounds();
                        drawRoundedBox(window,
                                    backRect,
                                    12.0f,
                                    1.0f,
                                    sf::Color(236, 239, 243, 245),
                                    sf::Color(195, 199, 206));
                        drawIconInButton(homeIconTex, backButton_, sf::Color(201, 63, 88));

                        sf::Text mstTitle("Minimum Spanning Tree Visualization", font_, 22);
                        mstTitle.setFillColor(sf::Color(58, 78, 94));
                        sf::FloatRect tb = mstTitle.getLocalBounds();
                        mstTitle.setPosition((window.getSize().x - tb.width) * 0.5f, 28.f);
                        window.draw(mstTitle);

                        if (showLeftActions_) {
                            sf::RectangleShape setupAccent(sf::Vector2f(layout.leftPanel.width - 36.0f, 22.0f));
                            setupAccent.setPosition(layout.leftPanel.left + 18.0f, layout.leftPanel.top + 16.0f);
                            setupAccent.setFillColor(sf::Color(235, 190, 82, 82));
                            window.draw(setupAccent);
                            sf::Text setupTitle("Graph Setup", font_, 14);
                            setupTitle.setPosition(layout.leftPanel.left + 26.0f, layout.leftPanel.top + 18.0f);
                            setupTitle.setFillColor(sf::Color::White);
                            window.draw(setupTitle);

                            // Vẽ đường phân cách ngang (Separator line) giữa Setup và Algorithms
                            sf::RectangleShape separator(sf::Vector2f(layout.leftPanel.width - 36.0f, 1.0f));
                            separator.setPosition(layout.leftPanel.left + 18.0f, layout.leftPanel.top + 346.0f);
                            separator.setFillColor(sf::Color(74, 108, 129, 150)); // Màu kẻ ngang nhạt
                            window.draw(separator);

                            sf::RectangleShape algoAccent(sf::Vector2f(layout.leftPanel.width - 36.0f, 22.0f));
                            algoAccent.setPosition(layout.leftPanel.left + 18.0f, layout.leftPanel.top + 358.0f);
                            algoAccent.setFillColor(sf::Color(235, 190, 82, 82));
                            window.draw(algoAccent);
                            sf::Text algoTitle("Algorithms", font_, 14);
                            algoTitle.setPosition(layout.leftPanel.left + 26.0f, layout.leftPanel.top + 360.0f);
                            algoTitle.setFillColor(sf::Color::White);
                            window.draw(algoTitle);
                        }

                        float rightStepInfoBottomY = rightStepBox.top + 104.0f;
                        if (showRightStepPanel_) {
                            float rightInfoY = rightStepBox.top + 24.0f;
                            sf::Text buildInfo(timelineDirty_ ? "Build is required to update visualization steps."
                                                            : "Build: regenerate algorithm steps after graph edits.",
                                            monoFont_,
                                            13);
                            buildInfo.setFillColor(sf::Color(238, 242, 248));
                            const auto buildInfoLines =
                                wrapTextToWidth(monoFont_, buildInfo.getString().toAnsiString(), 13, rightTextMax);
                            for (size_t i = 0; i < std::min<size_t>(3, buildInfoLines.size()); ++i) {
                                sf::Text line(buildInfoLines[i], monoFont_, 13);
                        line.setPosition(rightInnerX, rightInfoY + static_cast<float>(i) * 18.f);
                                line.setFillColor(sf::Color(238, 242, 248));
                                window.draw(line);
                            }
                    rightInfoY += static_cast<float>(std::min<size_t>(3, buildInfoLines.size())) * 18.f + 8.f;

                            const std::string workflowText = "Workflow: New Graph -> Add Node -> Add/Modify Edges -> Build";
                            const auto workflowLines = wrapTextToWidth(monoFont_, workflowText, 12, rightTextMax);
                            for (const auto& workflowLine : workflowLines) {
                                sf::Text line(workflowLine, monoFont_, 12);
                                line.setPosition(rightInnerX, rightInfoY);
                                line.setFillColor(sf::Color(238, 242, 248));
                                window.draw(line);
                        rightInfoY += 18.f;
                            }
                    rightInfoY += 8.f;

                            sf::Text selectedNodeText(
                                "Selected Node: " + std::string(selectedNodeId_ >= 0 ? std::to_string(selectedNodeId_) : "None"),
                                monoFont_,
                                13);
                            selectedNodeText.setPosition(rightInnerX, rightInfoY);
                            selectedNodeText.setFillColor(sf::Color(246, 225, 132));
                            clampTextToWidth(selectedNodeText, rightTextMax);
                            window.draw(selectedNodeText);
                            rightStepInfoBottomY = rightInfoY + 24.f;
                        }

                        const int n = static_cast<int>(adjacencyMatrix_.size());
                        const float matrixCell = matrixCellForSize(n);
                        const sf::Vector2f matrixOrigin = matrixOriginForSize(n);

                        if (canvasMode_ == MstCanvasMode::Matrix) {
                            sf::Text matrixHelp("Adjacency Matrix: click a cell to toggle edge", font_, 14);
                            matrixHelp.setPosition(matrixOrigin.x, matrixOrigin.y - 34.f);
                            matrixHelp.setFillColor(sf::Color(72, 90, 106));
                            clampTextToWidth(matrixHelp, gGraphViewport.left + gGraphViewport.width - matrixOrigin.x - 8.0f);
                            window.draw(matrixHelp);

                            int deletedCount = 0;
                            for (bool alive : nodeAlive_) {
                                if (!alive) {
                                    ++deletedCount;
                                }
                            }

                            if (deletedCount > 0) {
                                sf::Text matrixLegend("x = deleted node, disabled row/column (" +
                                                        std::to_string(deletedCount) + " removed)",
                                                    monoFont_,
                                                    12);
                                matrixLegend.setPosition(matrixOrigin.x, matrixOrigin.y - 16.f);
                                matrixLegend.setFillColor(sf::Color(170, 96, 96));
                                clampTextToWidth(matrixLegend, gGraphViewport.left + gGraphViewport.width - matrixOrigin.x - 8.0f);
                                window.draw(matrixLegend);
                            }

                            for (int r = -1; r < n; ++r) {
                                for (int c = -1; c < n; ++c) {
                                    sf::RectangleShape cell(sf::Vector2f(matrixCell - 1.0f, matrixCell - 1.0f));
                                    cell.setPosition(matrixOrigin.x + (static_cast<float>(c + 1) * matrixCell),
                                                    matrixOrigin.y + (static_cast<float>(r + 1) * matrixCell));

                                    if (r == -1 || c == -1) {
                                        const bool inactiveHeader =
                                            (r == -1 && c >= 0 && c < n && c < static_cast<int>(nodeAlive_.size()) && !nodeAlive_[static_cast<size_t>(c)]) ||
                                            (c == -1 && r >= 0 && r < n && r < static_cast<int>(nodeAlive_.size()) && !nodeAlive_[static_cast<size_t>(r)]);
                                        cell.setFillColor(inactiveHeader ? sf::Color(220, 196, 196) : sf::Color(183, 192, 201));
                                    } else if (r == c) {
                                        const bool inactive = r < static_cast<int>(nodeAlive_.size()) && !nodeAlive_[static_cast<size_t>(r)];
                                        cell.setFillColor(inactive ? sf::Color(220, 196, 196) : sf::Color(199, 206, 213));
                                    } else {
                                        const bool inactive =
                                            r < static_cast<int>(nodeAlive_.size()) && c < static_cast<int>(nodeAlive_.size()) &&
                                            (!nodeAlive_[static_cast<size_t>(r)] || !nodeAlive_[static_cast<size_t>(c)]);
                                        if (inactive) {
                                            cell.setFillColor(sf::Color(224, 208, 208));
                                        } else {
                                            cell.setFillColor(adjacencyMatrix_[static_cast<size_t>(r)][static_cast<size_t>(c)] > 0
                                                                ? sf::Color(173, 204, 166)
                                                                : sf::Color(216, 221, 225));
                                        }
                                    }
                                    window.draw(cell);

                                    sf::Text t("", font_, 13);
                                    t.setFillColor(sf::Color(56, 70, 82));
                                    if (r == -1 && c >= 0) {
                                        if (c < static_cast<int>(nodeAlive_.size()) && !nodeAlive_[static_cast<size_t>(c)]) {
                                            t.setString("x");
                                            t.setFillColor(sf::Color(170, 96, 96));
                                        } else {
                                            t.setString(std::to_string(c));
                                        }
                                    } else if (c == -1 && r >= 0) {
                                        if (r < static_cast<int>(nodeAlive_.size()) && !nodeAlive_[static_cast<size_t>(r)]) {
                                            t.setString("x");
                                            t.setFillColor(sf::Color(170, 96, 96));
                                        } else {
                                            t.setString(std::to_string(r));
                                        }
                                    } else if (r >= 0 && c >= 0) {
                                        if ((r < static_cast<int>(nodeAlive_.size()) && !nodeAlive_[static_cast<size_t>(r)]) ||
                                            (c < static_cast<int>(nodeAlive_.size()) && !nodeAlive_[static_cast<size_t>(c)])) {
                                            t.setString("-");
                                            t.setFillColor(sf::Color(154, 106, 106));
                                        } else {
                                            t.setString(std::to_string(adjacencyMatrix_[static_cast<size_t>(r)][static_cast<size_t>(c)]));
                                        }
                                    }
                                sf::FloatRect textBounds = t.getLocalBounds();
                                t.setPosition(cell.getPosition().x + (cell.getSize().x - textBounds.width) * 0.5f - textBounds.left,
                                              cell.getPosition().y + (cell.getSize().y - textBounds.height) * 0.5f - textBounds.top);
                                    window.draw(t);
                                }
                            }
                        }

                        if (showRightStepPanel_) {
                            const std::string stepSummary =
                                (step != nullptr) ? step->description : "No step yet. Press Build to generate the timeline.";
                            const float descStartY = std::max(rightStepBox.top + 104.0f, rightStepInfoBottomY + 8.0f);
                            const float descMaxY = rightStepBox.top + rightStepBox.height - 14.0f;
                            const auto descLines = wrapTextToWidth(monoFont_, stepSummary, 14, rightTextMax);
                            const size_t maxByHeight =
                                static_cast<size_t>(std::max(0.0f, std::floor((descMaxY - descStartY) / 22.0f)));
                            const size_t shownDescLines = std::min(descLines.size(), maxByHeight);
                            for (size_t i = 0; i < shownDescLines; ++i) {
                                sf::Text desc(descLines[i], monoFont_, 14);
                        desc.setPosition(rightInnerX, descStartY + static_cast<float>(i) * 22.0f);
                                desc.setFillColor(sf::Color::White);
                                window.draw(desc);
                            }
                        }

                        if (showRightPseudocodePanel_) {
                            const float pseudoY = rightPseudoBox.top + 14.0f;
                            sf::Text pseudoTitle("Pseudocode", monoFont_, 17);
                            pseudoTitle.setPosition(rightInnerX, pseudoY);
                            pseudoTitle.setFillColor(sf::Color(246, 225, 132));
                            clampTextToWidth(pseudoTitle, rightTextMax);
                            window.draw(pseudoTitle);

                            float pseudoLineY = pseudoY + 30.f;
                            const float pseudoMaxY = rightPseudoBox.top + rightPseudoBox.height - 18.f;
                            for (size_t i = 0; i < pseudocode_.size(); ++i) {
                                if (pseudoLineY > pseudoMaxY) {
                                    break;
                                }
                                const int lineNumber = static_cast<int>(i + 1);
                                bool highlighted =
                                    (step != nullptr) &&
                                    (std::find(step->pseudocodeLines.begin(), step->pseudocodeLines.end(), lineNumber) !=
                                    step->pseudocodeLines.end());

                                // Trừ hao 16.0f để đảm bảo thụt lề dưới đây không đẩy chữ ra ngoài rìa
                                const auto wrappedPseudo = wrapTextToWidth(monoFont_, pseudocode_[i], 13, rightTextMax - 16.0f);
                                for (size_t w = 0; w < wrappedPseudo.size(); ++w) {
                                    if (pseudoLineY > pseudoMaxY) {
                                        break;
                                    }
                                    const float indentX = (w == 0) ? 0.0f : 16.0f;
                                    if (highlighted) {
                                sf::RectangleShape hlBg(sf::Vector2f(rightTextMax + 8.0f, 20.0f));
                                hlBg.setPosition(rightInnerX - 4.0f, pseudoLineY);
                                        hlBg.setFillColor(sf::Color(245, 210, 96, 230));
                                        window.draw(hlBg);
                                    }
                                    sf::Text line(wrappedPseudo[w], monoFont_, 13);
                                    line.setPosition(rightInnerX + indentX, pseudoLineY);
                                    line.setFillColor(highlighted ? sf::Color(18, 18, 18) : sf::Color(236, 240, 246));
                                    line.setStyle(highlighted ? sf::Text::Bold : sf::Text::Regular);
                                    window.draw(line);
                            pseudoLineY += 22.f;
                                }
                                pseudoLineY += 4.f;
                            }
                        }
                    } else {
                        std::string selectedName = "";
                        if (selectedStructure_ == RenderViewKind::DoublyLinkedList) {
                            selectedName = "Doubly Linked List";
                        } else if (selectedStructure_ == RenderViewKind::HashTable) {
                            selectedName = "Hash Table";
                        } else {
                            selectedName = "Red-Black Tree";
                        }

                        sf::Text selectedTitle(selectedName, font_, 28);
                        selectedTitle.setPosition(430.f, 18.f);
                        selectedTitle.setFillColor(sf::Color(58, 78, 94));
                        window.draw(selectedTitle);
                    }
                }

                window.display();
            }
        }
