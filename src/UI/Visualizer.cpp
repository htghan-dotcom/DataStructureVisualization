#include "Visualizer.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <optional>
#include <random>
#include <sstream>
#include <system_error>
#include <unordered_map>

#include "ThemeManager.h"
#include "../config/Config.h"
#include "../data/GraphLoader.h"

Visualizer::Visualizer()
    : backButton_("", font_), speedSlider_(0.f, 0.f, 100.f, 0.f, 10.f, 0.f, font_) {}

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
            const float leftSideW = std::clamp(380.0f, 280.0f, maxSideByWindow); // Ép rộng thêm khu vực bên trái
            const float rightSideW = std::clamp(248.0f, 180.0f, maxSideByWindow);
            const float bottomH = 80.0f;
            const float toggleW = 24.0f;

            UILayout l;
            l.bottomPanel = sf::FloatRect({margin, h - margin - bottomH}, {std::max(100.0f, w - 2.0f * margin), bottomH});

            const float headerH = 44.0f;
            const float panelTop = margin + headerH;
            const float sideH = std::max(120.0f, l.bottomPanel.position.y - panelTop - gutter);
            l.leftPanel = sf::FloatRect({margin, panelTop}, {leftSideW, sideH});
            l.rightPanel = sf::FloatRect({w - margin - rightSideW, panelTop}, {rightSideW, sideH});

    float leftCollapseX = showLeft ? (l.leftPanel.position.x + l.leftPanel.size.x) : 0.0f;
            l.leftCollapseBtn = sf::FloatRect({leftCollapseX,
                                            l.leftPanel.position.y + (l.leftPanel.size.y - kLeftCollapseBtnH) * 0.5f},
                                            {kLeftCollapseBtnW,
                                            kLeftCollapseBtnH});
                                            
            const float rightContentW = l.rightPanel.size.x;
            l.rightStepBox = sf::FloatRect({l.rightPanel.position.x, l.rightPanel.position.y}, {rightContentW, 218.0f});
            l.rightPseudoBox = sf::FloatRect({l.rightPanel.position.x,
                                            l.rightPanel.position.y + 230.0f},
                                            {rightContentW,
                                            std::max(120.0f, l.rightPanel.size.y - 230.0f)});

    float stepToggleX = showRightStep ? (l.rightPanel.position.x - toggleW) : (w - toggleW);
            l.rightStepToggleBtn = sf::FloatRect({stepToggleX,
                                                l.rightStepBox.position.y + (l.rightStepBox.size.y - kRightStepBtnH) * 0.5f},
                                                {toggleW,
                                                kRightStepBtnH});

    float pseudoToggleX = showRightPseudo ? (l.rightPanel.position.x - toggleW) : (w - toggleW);
            l.rightPseudoToggleBtn = sf::FloatRect({pseudoToggleX,
                                                l.rightPseudoBox.position.y + (l.rightPseudoBox.size.y - 80.0f) * 0.5f},
                                                {toggleW,
                                                80.0f});

            float graphMinX = leftCollapseX + kLeftCollapseBtnW + gutter;
            float graphMaxX = std::min(stepToggleX, pseudoToggleX) - gutter;
            float graphMinY = panelTop;
            float graphMaxY = l.bottomPanel.position.y - gutter;

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

    l.graphViewport = sf::FloatRect({graphMinX + offsetX, graphMinY + offsetY}, {finalW, finalH});

            const float pad = 12.0f;
            const float buttonsW = std::clamp(l.bottomPanel.size.x * 0.42f, 360.0f, 560.0f);
            l.bottomButtonsArea = sf::FloatRect({l.bottomPanel.position.x + pad, l.bottomPanel.position.y + pad}, {buttonsW, l.bottomPanel.size.y - 2.0f * pad});
            l.bottomStatusArea = sf::FloatRect({l.bottomButtonsArea.position.x + l.bottomButtonsArea.size.x + 18.0f,
                                            l.bottomPanel.position.y + pad},
                                            {std::max(120.0f, l.bottomPanel.size.x - buttonsW - pad * 2.0f - 18.0f),
                                            l.bottomPanel.size.y - 2.0f * pad});

            const float sliderGap = 16.0f;
            const float sliderW = std::clamp(l.bottomStatusArea.size.x * 0.34f, 180.0f, 260.0f);
            l.bottomSliderArea = sf::FloatRect({l.bottomStatusArea.position.x + l.bottomStatusArea.size.x - sliderW,
                                            l.bottomStatusArea.position.y},
                                            {sliderW,
                                            l.bottomStatusArea.size.y});
            l.bottomStatusTextArea = sf::FloatRect({l.bottomStatusArea.position.x,
                                                l.bottomStatusArea.position.y},
                                                {std::max(80.0f, l.bottomStatusArea.size.x - sliderW - sliderGap),
                                                l.bottomStatusArea.size.y});
            return l;
        }

        sf::FloatRect gGraphViewport({kGraphMinX, kGraphMinY}, {kGraphMaxX - kGraphMinX, kGraphMaxY - kGraphMinY});

        sf::Vector2f graphToScreen(float nx, float ny) {
            return sf::Vector2f(gGraphViewport.position.x + nx * gGraphViewport.size.x,
                                gGraphViewport.position.y + ny * gGraphViewport.size.y);
        }

        sf::Vector2f matrixOriginForSize(int n) {
            const float cell = std::clamp(kMatrixBaseCell - std::max(0, n - 10) * 0.55f, 14.0f, kMatrixBaseCell);
            const float tableW = cell * static_cast<float>(n + 1);
            const float tableH = cell * static_cast<float>(n + 1);
            const float x = gGraphViewport.position.x + (gGraphViewport.size.x - tableW) * 0.5f;
            const float y = gGraphViewport.position.y + (gGraphViewport.size.y - tableH) * 0.5f;
            return sf::Vector2f(x, y);
        }

        float matrixCellForSize(int n) {
            return std::clamp(kMatrixBaseCell - std::max(0, n - 10) * 0.45f, 16.0f, kMatrixBaseCell);
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

        bool loadFontFromCandidates(sf::Font& font, const std::vector<std::string>& candidates) {
            for (const auto& path : candidates) {
                std::error_code ec;
                if (!std::filesystem::exists(path, ec) || ec) {
                    continue;
                }
                if (font.openFromFile(path)) {
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

            if (text.getLocalBounds().size.x <= maxWidth) {
                return;
            }

            const std::string ellipsis = "...";
            std::string trimmed = original;
            text.setString(trimmed + ellipsis);
            while (!trimmed.empty() && text.getLocalBounds().size.x > maxWidth) {
                trimmed.pop_back();
                text.setString(trimmed + ellipsis);
            }

            if (trimmed.empty() && text.getLocalBounds().size.x > maxWidth) {
                text.setString(ellipsis);
            }
        }

        std::vector<std::string> wrapTextToWidth(const sf::Font& font,
                                                const std::string& content,
                                                unsigned int charSize,
                                                float maxWidth) {
            sf::Text measure(font, "", charSize);
            auto fits = [&](const std::string& s) {
                measure.setString(s);
                return measure.getLocalBounds().size.x <= maxWidth;
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

            pendingMatrixRow_ = row;
            pendingMatrixCol_ = col;
            enteringEdgeWeight_ = true;
            edgeWeightInput_ = std::to_string(adjacencyMatrix_[static_cast<size_t>(row)][static_cast<size_t>(col)]);
            if (edgeWeightInput_ == "0") {
                edgeWeightInput_.clear();
            }
            return true;
        }

        bool Visualizer::isInsideGraphViewport(const sf::Vector2f& mousePos) const {
            return gGraphViewport.contains(mousePos); // sf::FloatRect::contains still works with sf::Vector2f
        }

        sf::Vector2f Visualizer::screenToGraphNormalized(const sf::Vector2f& mousePos) const {
            const float nx = (mousePos.x - gGraphViewport.position.x) / std::max(1.0f, gGraphViewport.size.x);
            const float ny = (mousePos.y - gGraphViewport.position.y) / std::max(1.0f, gGraphViewport.size.y);
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
            enteringRandomCount_ = false;
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
            pendingMatrixRow_ = -1;
            pendingMatrixCol_ = -1;
            enteringEdgeWeight_ = false;
            edgeWeightInput_.clear();
            enteringRandomCount_ = false;
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
            pendingMatrixRow_ = -1;
            pendingMatrixCol_ = -1;
            enteringEdgeWeight_ = false;
            edgeWeightInput_.clear();
            enteringRandomCount_ = false;
    zoomLevel_ = 1.0f;
    panOffset_ = {0.0f, 0.0f};
            rebuildTimeline();
        }

        void Visualizer::onActionRandom() {
            pushUndoState();
            int nodeCount = targetRandomCount_ > 0 ? targetRandomCount_ : 5 + (std::rand() % 10);
            nodeCount = std::clamp(nodeCount, 2, 26);
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
            pendingMatrixRow_ = -1;
            pendingMatrixCol_ = -1;
            enteringEdgeWeight_ = false;
            edgeWeightInput_.clear();
            enteringRandomCount_ = false;
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
            pendingMatrixRow_ = -1;
            pendingMatrixCol_ = -1;
            enteringEdgeWeight_ = false;
            edgeWeightInput_.clear();
            enteringRandomCount_ = false;
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
            pendingMatrixRow_ = -1;
            pendingMatrixCol_ = -1;
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
            pendingMatrixRow_ = -1;
            pendingMatrixCol_ = -1;
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
            pendingMatrixRow_ = -1;
            pendingMatrixCol_ = -1;
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
            pendingMatrixRow_ = -1;
            pendingMatrixCol_ = -1;
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
            pendingMatrixRow_ = -1;
            pendingMatrixCol_ = -1;
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
            pendingMatrixRow_ = -1;
            pendingMatrixCol_ = -1;
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

void Visualizer::onActionResetView() {
    zoomLevel_ = 1.0f;
    panOffset_ = {0.0f, 0.0f};
}

void Visualizer::run(sf::RenderWindow& window) {
            std::cout << "[Debug] Bắt đầu chạy Visualizer::run()..." << std::endl;
            ThemeManager::setTheme(ThemeType::DEFAULT);

            const std::vector<std::string> uiFontCandidates = {
                "assets/fonts/Inter-Regular.ttf",
                "../assets/fonts/Inter-Regular.ttf",
                "C:/Windows/Fonts/segoeui.ttf",
                "C:/Windows/Fonts/arial.ttf"};

            const std::vector<std::string> monoFontCandidates = {
                "assets/fonts/Inter-Bold.ttf",
                "../assets/fonts/Inter-Bold.ttf",
                "C:/Windows/Fonts/consola.ttf",
                "C:/Windows/Fonts/cour.ttf"};

            std::cout << "[Debug] Đang tải UI Font..." << std::endl;
            if (!loadFontFromCandidates(font_, uiFontCandidates)) {
                std::cerr << "Error: Không tìm thấy UI Font! Ứng dụng sẽ thoát. Nhấn Enter để thoát.\n";
                std::cin.get(); // Dừng màn hình để đọc lỗi
                return;
            }

            std::cout << "[Debug] Đang tải Mono Font..." << std::endl;
            if (!loadFontFromCandidates(monoFont_, monoFontCandidates)) {
                std::cerr << "Error: Không tìm thấy Mono Font! Ứng dụng sẽ thoát. Nhấn Enter để thoát.\n";
                std::cin.get(); // Dừng màn hình để đọc lỗi
                return;
            }

            std::cout << "[Debug] Đang tải hình ảnh (Textures)..." << std::endl;
            sf::Texture homeIconTex;
            sf::Texture playIconTex;
            sf::Texture pauseIconTex;
            sf::Texture skipBackIconTex;
            sf::Texture skipForwardIconTex;
            sf::Texture bgLogoTex;

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
            loadTextureFromCandidates(bgLogoTex, makeIconCandidates("logo.png")); // Tên tệp ảnh nền của bạn

            // TẢI ẢNH ICON MỚI CỦA BẠN TẠI ĐÂY (Sửa tên tệp .png tương ứng nhé)
            sf::Texture iconAddNode, iconDeleteNode, iconEditEdges, iconRandom;
            loadTextureFromCandidates(iconAddNode, makeIconCandidates("addButton.png"));
            loadTextureFromCandidates(iconDeleteNode, makeIconCandidates("deleteButton.png"));
            loadTextureFromCandidates(iconEditEdges, makeIconCandidates("editButton.png"));
            loadTextureFromCandidates(iconRandom, makeIconCandidates("randomButton.png"));

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
                Button("New MST", font_),
                Button("Undo", font_),
                Button("Add Node", font_),
                Button("Delete Node", font_),
                Button("Edit Edges", font_),
                Button("Random", font_),
                Button("BUILD", font_),
                Button("View: MST", font_),
                Button("Kruskal", font_),
        Button("Prim", font_),
        Button("Reset View", font_),
        Button("Nodes: Auto", font_)};

            // Gắn ảnh vừa tải vào các nút bấm tương ứng
            controlButtons_[7].setIcon(&iconAddNode);
            controlButtons_[8].setIcon(&iconDeleteNode);
            controlButtons_[9].setIcon(&iconEditEdges);
            controlButtons_[10].setIcon(&iconRandom);

            backButton_ = Button("Back To Menu", font_);
            backButton_.setLabel("");
            backButton_.setSize(40.f, 40.f);
            backButton_.setPosition(20.f, 20.f);

            // Khởi tạo các nút ban đầu (Dọn sạch rác code, chia 2 cột, lùn lại)
            const float initButtonW = 165.f; // Tăng chiều rộng tối đa
            const float initButtonH = 30.f;  // Tăng chiều cao để chữ có padding tốt hơn
            const float initLeftX = 28.f;
            const float initGraphSetupY = 110.f;
            const float initRowGap = 40.f;
            const float initColGap = 10.f;

        // Khởi tạo vị trí ban đầu (Group 1: System)
        controlButtons_[5].setSize(initButtonW, initButtonH);
        controlButtons_[5].setPosition(initLeftX, initGraphSetupY);
        controlButtons_[6].setSize(initButtonW, initButtonH);
        controlButtons_[6].setPosition(initLeftX + initButtonW + initColGap, initGraphSetupY);
        controlButtons_[10].setSize(initButtonW, initButtonH);
        controlButtons_[10].setPosition(initLeftX, initGraphSetupY + initRowGap);
        controlButtons_[12].setSize(initButtonW, initButtonH);
        controlButtons_[12].setPosition(initLeftX + initButtonW + initColGap, initGraphSetupY + initRowGap);

        // Group 2: Edit
        controlButtons_[7].setSize(initButtonW, initButtonH);
        controlButtons_[7].setPosition(initLeftX, initGraphSetupY + initRowGap * 2.5f);
        controlButtons_[8].setSize(initButtonW, initButtonH);
        controlButtons_[8].setPosition(initLeftX + initButtonW + initColGap, initGraphSetupY + initRowGap * 2.5f);
        controlButtons_[9].setSize(initButtonW * 2 + initColGap, initButtonH);
        controlButtons_[9].setPosition(initLeftX, initGraphSetupY + initRowGap * 3.5f);

        // Group 3: Build Action
        controlButtons_[11].setSize(initButtonW * 2 + initColGap, 40.f);
        controlButtons_[11].setPosition(initLeftX, initGraphSetupY + initRowGap * 5.0f);

            const float initAlgoY = initGraphSetupY + 4 * initRowGap + 20.f;
            controlButtons_[13].setSize(initButtonW, initButtonH);
            controlButtons_[13].setPosition(initLeftX, initAlgoY);
            controlButtons_[14].setSize(initButtonW, initButtonH);
            controlButtons_[14].setPosition(initLeftX + initButtonW + initColGap, initAlgoY);

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

            speedSlider_ = Slider(940.f, 650.f, 150.f, 1.f, 10.f, 2.f, font_);

            controlButtons_[0].setStyleRole(Button::StyleRole::Pill);
            controlButtons_[1].setStyleRole(Button::StyleRole::Pill);
            controlButtons_[2].setStyleRole(Button::StyleRole::Pill);
            controlButtons_[3].setStyleRole(Button::StyleRole::Pill);
            controlButtons_[4].setStyleRole(Button::StyleRole::Pill);

            sf::Vector2u prevWindowSize = window.getSize();

    // Ép trạng thái ban đầu là Visualization, nhảy thẳng vào đồ thị thay vì hiện Menu nội bộ
    currentScreen_ = Screen::Visualization;
    selectedStructure_ = RenderViewKind::MST;
    setupDefaultGraph();
    rebuildTimeline();

            std::cout << "[Debug] Vào vòng lặp chính (Main loop)..." << std::endl;
            while (window.isOpen()) {
                const sf::Vector2u logicalSize(static_cast<unsigned int>(window.getView().getSize().x), static_cast<unsigned int>(window.getView().getSize().y));
                const UILayout layout = computeUILayout(logicalSize, showLeftActions_, showRightStepPanel_, showRightPseudocodePanel_);
        
        float cx = layout.graphViewport.position.x + layout.graphViewport.size.x * 0.5f + panOffset_.x;
        float cy = layout.graphViewport.position.y + layout.graphViewport.size.y * 0.5f + panOffset_.y;
        float newW = layout.graphViewport.size.x * zoomLevel_;
        float newH = layout.graphViewport.size.y * zoomLevel_;
        gGraphViewport = sf::FloatRect({cx - newW * 0.5f, cy - newH * 0.5f}, {newW, newH});

                while (const std::optional<sf::Event> eventOpt = window.pollEvent()) {
                    const sf::Event& event = *eventOpt;

                    if (event.is<sf::Event::Closed>()) {
                        window.close();
                    }

                    if (const auto* resized = event.getIf<sf::Event::Resized>()) {
                        sf::View view = window.getView(); // Lấy view hiện tại (1440x960)
                        float windowWidth = static_cast<float>(resized->size.x);
                        float windowHeight = static_cast<float>(std::max(1u, resized->size.y));
                        float windowRatio = windowWidth / windowHeight;
                        float viewRatio = view.getSize().x / view.getSize().y;
                        float sizeX = 1.0f, sizeY = 1.0f;
                        float posX = 0.0f, posY = 0.0f;
                        
                        if (windowRatio > viewRatio) {
                            sizeX = viewRatio / windowRatio;
                            posX = (1.0f - sizeX) / 2.0f;
                        } else {
                            sizeY = windowRatio / viewRatio;
                            posY = (1.0f - sizeY) / 2.0f;
                        }
                        
                        view.setViewport(sf::FloatRect({posX, posY}, {sizeX, sizeY}));
                        window.setView(view);
                    }

            if (const auto* mouseWheelScrolled = event.getIf<sf::Event::MouseWheelScrolled>()) {
                if (currentScreen_ == Screen::Visualization && isMstSelected() && canvasMode_ == MstCanvasMode::Graph) {
                    if (mouseWheelScrolled->wheel == sf::Mouse::Wheel::Vertical) {
                        const sf::Vector2f mousePos = window.mapPixelToCoords(mouseWheelScrolled->position);
                        if (layout.graphViewport.contains(mousePos)) {
                            float zoomFactor = (mouseWheelScrolled->delta > 0) ? 1.15f : (1.0f / 1.15f);
                            float nextZoom = std::clamp(zoomLevel_ * zoomFactor, 0.2f, 5.0f);
                            
                            float oldCx = layout.graphViewport.position.x + layout.graphViewport.size.x * 0.5f + panOffset_.x;
                            float oldCy = layout.graphViewport.position.y + layout.graphViewport.size.y * 0.5f + panOffset_.y;
                            float dx = mousePos.x - oldCx;
                            float dy = mousePos.y - oldCy;
                            
                            // Giữ trọng tâm vị trí con trỏ chuột khi zoom
                            panOffset_.x -= dx * (nextZoom / zoomLevel_ - 1.0f);
                            panOffset_.y -= dy * (nextZoom / zoomLevel_ - 1.0f);
                            zoomLevel_ = nextZoom;
                            
                            const float maxPanX = layout.graphViewport.size.x * zoomLevel_ * 0.75f;
                            const float maxPanY = layout.graphViewport.size.y * zoomLevel_ * 0.75f;
                            panOffset_.x = std::clamp(panOffset_.x, -maxPanX, maxPanX);
                            panOffset_.y = std::clamp(panOffset_.y, -maxPanY, maxPanY);

                            float ncx = layout.graphViewport.position.x + layout.graphViewport.size.x * 0.5f + panOffset_.x;
                            float ncy = layout.graphViewport.position.y + layout.graphViewport.size.y * 0.5f + panOffset_.y;
                            gGraphViewport = sf::FloatRect({ncx - (layout.graphViewport.size.x * zoomLevel_) * 0.5f, 
                                                           ncy - (layout.graphViewport.size.y * zoomLevel_) * 0.5f}, 
                                                           {layout.graphViewport.size.x * zoomLevel_, 
                                                           layout.graphViewport.size.y * zoomLevel_});
                        }
                    }
                }
            }

            if (const auto* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseButtonPressed->button == sf::Mouse::Button::Right) {
                    if (currentScreen_ == Screen::Visualization && isMstSelected() && canvasMode_ == MstCanvasMode::Graph) {
                        const sf::Vector2f mousePos = window.mapPixelToCoords(mouseButtonPressed->position);
                        if (layout.graphViewport.contains(mousePos)) {
                            panning_ = true;
                            lastMousePos_ = mousePos;
                        }
                    }
                } else if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
                    const sf::Vector2f mousePos = window.mapPixelToCoords(mouseButtonPressed->position);

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
                        playing_ = false;
                        return; // Thoát khỏi hàm run() để trả quyền điều khiển về cho MainMenu của App
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
                            if (enteringRandomCount_) {
                                if (controlButtons_.size() > 16 && !controlButtons_[16].contains(mousePos)) {
                                    enteringRandomCount_ = false;
                                    targetRandomCount_ = randomCountInput_.empty() ? 0 : std::stoi(randomCountInput_);
                                }
                            }

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
                                    case 15: onActionResetView(); break;
                                    case 16:
                                        enteringRandomCount_ = true;
                                        randomCountInput_ = targetRandomCount_ > 0 ? std::to_string(targetRandomCount_) : "";
                                        break;
                                    default: break;
                                }
                                consumed = true;
                                break;
                            }

                        if (!consumed && canvasMode_ == MstCanvasMode::Matrix) {
                            if (handleAdjacencyMatrixClick(mousePos)) {
                                consumed = true;
                            } else if (enteringEdgeWeight_) {
                                enteringEdgeWeight_ = false;
                                pendingMatrixRow_ = -1;
                                pendingMatrixCol_ = -1;
                                edgeWeightInput_.clear();
                            }
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
            }

            if (const auto* mouseButtonReleased = event.getIf<sf::Event::MouseButtonReleased>()) {
                if (mouseButtonReleased->button == sf::Mouse::Button::Right) {
                    panning_ = false;
                } else if (mouseButtonReleased->button == sf::Mouse::Button::Left) {
                    speedSlider_.setActive(false);
                    draggingNode_ = false;
                    draggingNodeId_ = -1;
                }
            }

            if (const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>()) {
                const sf::Vector2f mousePos = window.mapPixelToCoords(mouseMoved->position);

                if (panning_) {
                    panOffset_ += (mousePos - lastMousePos_);
                    lastMousePos_ = mousePos;
                    
                    const float maxPanX = layout.graphViewport.size.x * zoomLevel_ * 0.75f;
                    const float maxPanY = layout.graphViewport.size.y * zoomLevel_ * 0.75f;
                    panOffset_.x = std::clamp(panOffset_.x, -maxPanX, maxPanX);
                    panOffset_.y = std::clamp(panOffset_.y, -maxPanY, maxPanY);

                    float cx = layout.graphViewport.position.x + layout.graphViewport.size.x * 0.5f + panOffset_.x;
                    float cy = layout.graphViewport.position.y + layout.graphViewport.size.y * 0.5f + panOffset_.y;
                    gGraphViewport = sf::FloatRect({cx - (layout.graphViewport.size.x * zoomLevel_) * 0.5f, 
                                                   cy - (layout.graphViewport.size.y * zoomLevel_) * 0.5f}, 
                                                   {layout.graphViewport.size.x * zoomLevel_, 
                                                   layout.graphViewport.size.y * zoomLevel_});
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

            if (const auto* textEntered = event.getIf<sf::Event::TextEntered>()) {
                if (currentScreen_ == Screen::Visualization && isMstSelected()) {
                    if (enteringRandomCount_) {
                        const std::uint32_t uni = textEntered->unicode;
                        if (uni >= '0' && uni <= '9') {
                            if (randomCountInput_.size() < 2) {
                                randomCountInput_.push_back(static_cast<char>(uni));
                            }
                        } else if (uni == 8) {
                            if (!randomCountInput_.empty()) {
                                randomCountInput_.pop_back();
                            }
                        } else if (uni == 13) {
                            targetRandomCount_ = randomCountInput_.empty() ? 0 : std::stoi(randomCountInput_);
                            enteringRandomCount_ = false;
                        } else if (uni == 27) {
                            enteringRandomCount_ = false;
                        }
                    } else if (enteringEdgeWeight_) {
                        const std::uint32_t uni = textEntered->unicode;
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
                }
            }

            if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
                if (currentScreen_ == Screen::Visualization && isMstSelected()) {
                    if (enteringRandomCount_ && keyPressed->code == sf::Keyboard::Key::Escape) {
                        enteringRandomCount_ = false;
                    }
                    if (enteringEdgeWeight_ && keyPressed->code == sf::Keyboard::Key::Escape) {
                        pendingEdgeFrom_ = -1;
                        pendingEdgeTo_ = -1;
                        enteringEdgeWeight_ = false;
                        edgeWeightInput_.clear();
                    }

                    if (!enteringEdgeWeight_ && keyPressed->control && keyPressed->code == sf::Keyboard::Key::Z) {
                        onActionUndo();
                    }

                    if (!enteringEdgeWeight_ && (keyPressed->code == sf::Keyboard::Key::Space || keyPressed->code == sf::Keyboard::Key::Right)) {
                        if (!timelineDirty_ && !animation_.empty()) {
                            playing_ = false;
                            onActionNext();
                        }
                    }
                }
            }
        } // Kết thúc vòng lặp while (pollEvent)

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

                    const float sidePad = 12.0f; // Giãn lề 2 bên rộng hơn chút
                    const float colGap = 12.0f;  // Khoảng cách ngang giữa các nút
                    const float leftX = layout.leftPanel.position.x + sidePad;
                    const float buttonH = 30.0f; // Nút cao hơn để không bị tù túng
                    const float topY = layout.leftPanel.position.y;

                    // Hàm Lambda giúp tự động xếp các nút thành hàng ngang và chia đều chiều rộng
                    auto placeRow = [&](const std::vector<int>& btnIndices, float startY, float h) {
                        if (btnIndices.empty()) return;
                        float w = (layout.leftPanel.size.x - sidePad * 2.0f - colGap * (btnIndices.size() - 1)) / btnIndices.size();
                        for (size_t k = 0; k < btnIndices.size(); ++k) {
                            controlButtons_[btnIndices[k]].setSize(w, h);
                            controlButtons_[btnIndices[k]].setPosition(leftX + k * (w + colGap), topY + startY);
                        }
                    };

                    // Left panel: System controls
                    placeRow({5, 6}, 46.0f, buttonH);                 // New Graph, Undo
                    placeRow({10, 16, 12}, 88.0f, buttonH);           // Random, Nodes, View Matrix
                    placeRow({15}, 130.0f, buttonH);                  // Reset View
                    // Left panel: Edit controls
                    placeRow({7, 8}, 206.0f, buttonH);                // Add Node, Delete Node
                    placeRow({9}, 248.0f, buttonH);                   // Edit Edges (Trải dài 1 cột)
                    // Left panel: Build Action
                    placeRow({11}, 298.0f, 40.0f);                    // Nút Build (Bự hơn, cao 40)
                    // Left panel: Algorithms controls
                    placeRow({13, 14}, 394.0f, buttonH);              // Kruskal, Prim

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

                // Task Bar layout with smooth collapse animation
                float w0 = smoothLerp(48.0f, 0.0f, taskBarAnimProgress_);
                float w1 = smoothLerp(165.0f, 0.0f, taskBarAnimProgress_);
                float w3 = smoothLerp(165.0f, 0.0f, taskBarAnimProgress_);
                float w4 = smoothLerp(48.0f, 0.0f, taskBarAnimProgress_);
                float gap = smoothLerp(12.0f, 0.0f, taskBarAnimProgress_);

                const float hBtn = 49.0f;
                const float totalW = w0 + w1 + 48.0f + w3 + w4 + gap * 4;
                float x = layout.bottomPanel.position.x + (layout.bottomPanel.size.x - totalW) / 2.0f;
                const float y = layout.bottomPanel.position.y + (layout.bottomPanel.size.y - hBtn) / 2.0f;
                
                controlButtons_[0].setSize(std::max(0.1f, w0), hBtn); 
                controlButtons_[1].setSize(std::max(0.1f, w1), hBtn); 
                controlButtons_[2].setSize(48.0f, hBtn); 
                controlButtons_[3].setSize(std::max(0.1f, w3), hBtn); 
                controlButtons_[4].setSize(std::max(0.1f, w4), hBtn); 

                controlButtons_[0].setPosition(x, y); x += w0 + gap;
                controlButtons_[1].setPosition(x, y); x += w1 + gap;
                controlButtons_[2].setPosition(x, y); x += 48.0f + gap;
                controlButtons_[3].setPosition(x, y); x += w3 + gap;
                controlButtons_[4].setPosition(x, y);

                if (taskBarAnimProgress_ > 0.05f) {
                    controlButtons_[1].setLabel("");
                    controlButtons_[3].setLabel("");
                } else {
                    controlButtons_[1].setLabel("Step back");
                    controlButtons_[3].setLabel("Step forward");
                }

                // Slider positioned strictly to the far right
                const float sliderX = layout.bottomPanel.position.x + layout.bottomPanel.size.x - 160.f;
                const float sliderY = layout.bottomPanel.position.y + layout.bottomPanel.size.y * 0.5f;
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
                    controlButtons_[11].setEnabled(!playing_);   // Build (Primary action)
                    controlButtons_[12].setEnabled(!playing_);   // View
                    controlButtons_[13].setEnabled(!playing_);   // Kruskal
                    controlButtons_[14].setEnabled(!playing_);   // Prim
                    controlButtons_[15].setEnabled(canvasMode_ == MstCanvasMode::Graph); // Reset View
                    controlButtons_[16].setEnabled(!playing_);   // Nodes: Auto

                    if (!showLeftActions_) {
                        for (int i = 5; i <= 16; ++i) {
                            controlButtons_[i].setEnabled(false);
                        }
                    }

                    controlButtons_[12].setLabel(canvasMode_ == MstCanvasMode::Graph ? "View: Matrix" : "View: MST");
                    controlButtons_[11].setStyleRole(Button::StyleRole::Primary);
                    controlButtons_[11].setLabel(timelineDirty_ ? "BUILD *" : "BUILD");

                    if (enteringRandomCount_) {
                        controlButtons_[16].setLabel(randomCountInput_ + "_");
                    } else {
                        controlButtons_[16].setLabel(targetRandomCount_ > 0 ? "Nodes: " + std::to_string(targetRandomCount_) : "Nodes: Auto");
                    }

                    controlButtons_[7].setSelected(placingNode_);
                    controlButtons_[11].setSelected(timelineDirty_);
                    controlButtons_[8].setSelected(deletingNodeMode_);
                    controlButtons_[9].setSelected(addingEdgeMode_ || enteringEdgeWeight_ || pendingEdgeFrom_ >= 0);
                    controlButtons_[12].setSelected(canvasMode_ == MstCanvasMode::Matrix);
                    controlButtons_[13].setSelected(algorithmType_ == algo::AlgorithmType::Kruskal);
                    controlButtons_[14].setSelected(algorithmType_ == algo::AlgorithmType::Prim);
                }

                // Đổ màu viền Letterbox bằng màu xám đen để các Panel nhìn "sát góc"
                window.clear(sf::Color(40, 40, 40));
                sf::RectangleShape appBg(sf::Vector2f(window.getView().getSize().x, window.getView().getSize().y));
                appBg.setPosition({0.f, 0.f});
                appBg.setFillColor(ThemeManager::current.screenBg);
                window.draw(appBg);

                if (currentScreen_ == Screen::Menu) {
                    sf::Text title(font_, "Choose Data Structure", 34);
                    title.setPosition({430.f, 130.f});
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
                        // Vẽ logo/icon nền mờ (Watermark) ở giữa Graph Viewport
                        if (bgLogoTex.getSize().x > 0) {
                            sf::Sprite bgSprite(bgLogoTex);
                            // Tính tỷ lệ để ảnh chiếm khoảng 60% khung nhìn
                            float scaleX = layout.graphViewport.size.x / static_cast<float>(bgLogoTex.getSize().x);
                            float scaleY = layout.graphViewport.size.y / static_cast<float>(bgLogoTex.getSize().y);
                            float scale = std::min(scaleX, scaleY) * 0.6f;
                            bgSprite.setScale({scale, scale});
                            bgSprite.setOrigin({static_cast<float>(bgLogoTex.getSize().x) * 0.5f, static_cast<float>(bgLogoTex.getSize().y) * 0.5f});
                            bgSprite.setPosition({layout.graphViewport.position.x + layout.graphViewport.size.x * 0.5f, 
                                                 layout.graphViewport.position.y + layout.graphViewport.size.y * 0.5f});
                            bgSprite.setColor(sf::Color(ThemeManager::current.textColor.r, ThemeManager::current.textColor.g, ThemeManager::current.textColor.b, 20)); // Watermark
                            window.draw(bgSprite);
                        }
                        Renderer::draw(window, vm, font_);

                        if (isMstSelected() && canvasMode_ == MstCanvasMode::Graph) {
                            if (!enteringEdgeWeight_) {
                                std::string modeMsg = "";
                                sf::Color modeColor = sf::Color::Transparent;
                                if (placingNode_) {
                                    modeMsg = "Add Node Mode: Click empty space to place node";
                                    modeColor = sf::Color(46, 175, 100, 230); // Xanh lá
                                } else if (deletingNodeMode_) {
                                    modeMsg = "Delete Node Mode: Click a node to remove it";
                                    modeColor = sf::Color(197, 100, 100, 230); // Đỏ
                                } else if (addingEdgeMode_) {
                                    modeMsg = (pendingEdgeFrom_ < 0) ? "Edit Edges Mode: Select the FIRST node" : "Edit Edges Mode: Select the SECOND node";
                                    modeColor = sf::Color(65, 125, 195, 230); // Xanh lam
                                }

                                if (!modeMsg.empty()) {
                                    sf::Text modeText(font_, modeMsg, 18);
                                    sf::FloatRect mtb = modeText.getLocalBounds();
                                    
                                    sf::RectangleShape modeRect;
                                    modeRect.setSize({mtb.size.x + 40.f, 44.f});
                                    modeRect.setPosition({layout.graphViewport.position.x + layout.graphViewport.size.x * 0.5f - modeRect.getSize().x * 0.5f,
                                                          layout.graphViewport.position.y + 20.f});
                                    modeRect.setFillColor(modeColor);
                                    modeRect.setOutlineThickness(2.f);
                                    modeRect.setOutlineColor(sf::Color(255, 255, 255, 150));
                                    window.draw(modeRect);
                                    
                                    modeText.setFillColor(sf::Color::White);
                                    modeText.setPosition({std::round(modeRect.getPosition().x + 20.f), std::round(modeRect.getPosition().y + 10.f)});
                                    window.draw(modeText);
                                }
                            }
                            
                            // POPUP DẠNG TỐI MÀN HÌNH CHẮN GIỮA WINDOW
                            if (enteringEdgeWeight_) {
                                sf::RectangleShape modalBg;
                                modalBg.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
                                modalBg.setFillColor(sf::Color(0, 0, 0, 180)); // Đen mờ làm chìm cảnh vật
                                window.draw(modalBg);

                                sf::ConvexShape popupRect = createRoundedRect({380.f, 160.f}, 20.f);
                                popupRect.setPosition({(static_cast<float>(window.getSize().x) - 380.f) * 0.5f, (static_cast<float>(window.getSize().y) - 160.f) * 0.5f});
                                popupRect.setFillColor(ThemeManager::current.bg);
                                popupRect.setOutlineThickness(2.f);
                                popupRect.setOutlineColor(ThemeManager::current.primary);
                                window.draw(popupRect);
                                
                                std::string titleStr = "EDIT EDGE WEIGHT";
                                sf::Text titleText(font_, titleStr, 20);
                                titleText.setFillColor(sf::Color(150, 200, 255));
                                sf::FloatRect ttb = titleText.getLocalBounds();
                                titleText.setPosition({popupRect.getPosition().x + (380.f - ttb.size.x) * 0.5f - ttb.position.x,
                                                       popupRect.getPosition().y + 15.f});
                                window.draw(titleText);

                                sf::Text promptText(font_, "Node " + std::to_string(pendingEdgeFrom_) + " <--> Node " + std::to_string(pendingEdgeTo_), 16);
                                promptText.setFillColor(ThemeManager::current.textColor);
                                sf::FloatRect ptb = promptText.getLocalBounds();
                                promptText.setPosition({popupRect.getPosition().x + (380.f - ptb.size.x) * 0.5f - ptb.position.x,
                                                        popupRect.getPosition().y + 55.f});
                                window.draw(promptText);

                                sf::Text guideText(font_, "(Enter weight or 0 to remove edge)", 13);
                                guideText.setFillColor(sf::Color(ThemeManager::current.textColor.r, ThemeManager::current.textColor.g, ThemeManager::current.textColor.b, 150));
                                sf::FloatRect gtb = guideText.getLocalBounds();
                                guideText.setPosition({popupRect.getPosition().x + (380.f - gtb.size.x) * 0.5f - gtb.position.x,
                                                       popupRect.getPosition().y + 80.f});
                                window.draw(guideText);

                                sf::Text inputText(font_, edgeWeightInput_ + "_", 26);
                                inputText.setFillColor(ThemeManager::current.primary);
                                sf::FloatRect itb = inputText.getLocalBounds();
                                inputText.setPosition({popupRect.getPosition().x + (380.f - itb.size.x) * 0.5f - itb.position.x,
                                                       popupRect.getPosition().y + 110.f});
                                window.draw(inputText);
                            }
                        }
                    }

                    if (isMstSelected()) {
                        const float rightContentW = layout.rightStepBox.size.x;
                        const float rightTextMax = rightContentW - 24.0f;
                        const float rightInnerX = layout.rightPanel.position.x + kRightContentInset;
                        const sf::FloatRect rightStepBox = layout.rightStepBox;
                        const sf::FloatRect rightPseudoBox = layout.rightPseudoBox;

                        if (showRightStepPanel_) {
                            // Làm cho box background tròn nhẹ hơn
                            sf::ConvexShape stepBg = createRoundedRect(rightStepBox.size, 14.0f);
                            stepBg.setPosition(rightStepBox.position);
                            stepBg.setFillColor(ThemeManager::current.secondary);
                            window.draw(stepBg);
                        }

                        if (showRightPseudocodePanel_) {
                            sf::ConvexShape pseudoBg = createRoundedRect(rightPseudoBox.size, 14.0f);
                            pseudoBg.setPosition(rightPseudoBox.position);
                            pseudoBg.setFillColor(ThemeManager::current.secondary);
                            window.draw(pseudoBg);
                        }

                        for (size_t i = 0; i < controlButtons_.size(); ++i) {
                            if (!showLeftActions_ && i >= 5 && i <= 16) {
                                continue;
                            }
                            // Hide completely when collapsed to prevent drawing artifacts
                            if (taskBarAnimProgress_ >= 0.95f && (i == 0 || i == 1 || i == 3 || i == 4)) {
                                continue;
                            }
                            controlButtons_[i].draw(window);
                        }
                        if (playing_) {
                            speedSlider_.draw(window);
                        }

                        // Task bar step text
                        std::string actionText = "Step 0 / 0";
                        if (!animation_.empty()) {
                            actionText = "Step " + std::to_string(animation_.currentIndex() + 1) + " / " + std::to_string(animation_.totalSteps());
                        }
                        sf::Text statusAction(font_, actionText, 18);
                        sf::FloatRect saBounds = statusAction.getLocalBounds();
                        statusAction.setOrigin({saBounds.position.x + saBounds.size.x / 2.f, saBounds.position.y + saBounds.size.y / 2.f});
                        float statusY = controlButtons_[0].bounds().position.y - 30.f;
                        statusAction.setPosition({layout.bottomPanel.position.x + layout.bottomPanel.size.x / 2.f, statusY});
                        statusAction.setFillColor(ThemeManager::current.textColor);
                        window.draw(statusAction);

                        // Left collapse control.
                        sf::ConvexShape leftCollapseShape = createRoundedRect(layout.leftCollapseBtn.size, 14.f);
                        leftCollapseShape.setPosition(layout.leftCollapseBtn.position);
                        leftCollapseShape.setFillColor(ThemeManager::current.secondary);
                        window.draw(leftCollapseShape);

                        sf::Text collapseLabel(monoFont_, showLeftActions_ ? "<" : ">", 16);
                        const sf::FloatRect collapseBounds = collapseLabel.getLocalBounds();
                        collapseLabel.setPosition({layout.leftCollapseBtn.position.x + (layout.leftCollapseBtn.size.x - collapseBounds.size.x) * 0.5f - collapseBounds.position.x,
                                                layout.leftCollapseBtn.position.y + (layout.leftCollapseBtn.size.y - collapseBounds.size.y) * 0.5f - collapseBounds.position.y - 2.f});
                        collapseLabel.setFillColor(ThemeManager::current.textColor);
                        window.draw(collapseLabel);

                        // Right side toggles: step info and pseudocode visibility.
                        sf::ConvexShape rightStepToggleShape = createRoundedRect(layout.rightStepToggleBtn.size, 14.f);
                        rightStepToggleShape.setPosition(layout.rightStepToggleBtn.position);
                        rightStepToggleShape.setFillColor(showRightStepPanel_ ? ThemeManager::current.primary : ThemeManager::current.secondary);
                        window.draw(rightStepToggleShape);

                        sf::Text stepToggle(monoFont_, showRightStepPanel_ ? ">" : "<", 16);
                        stepToggle.setFillColor(showRightStepPanel_ ? ThemeManager::current.bg : ThemeManager::current.textColor);
                        sf::FloatRect stepBounds = stepToggle.getLocalBounds();
                        stepToggle.setPosition({layout.rightStepToggleBtn.position.x + (layout.rightStepToggleBtn.size.x - stepBounds.size.x) * 0.5f - stepBounds.position.x,
                                               layout.rightStepToggleBtn.position.y + (layout.rightStepToggleBtn.size.y - stepBounds.size.y) * 0.5f - stepBounds.position.y - 2.f});
                        window.draw(stepToggle);

                        sf::ConvexShape rightPseudoToggleShape = createRoundedRect(layout.rightPseudoToggleBtn.size, 14.f);
                        rightPseudoToggleShape.setPosition(layout.rightPseudoToggleBtn.position);
                        rightPseudoToggleShape.setFillColor(showRightPseudocodePanel_ ? ThemeManager::current.primary : ThemeManager::current.secondary);
                        window.draw(rightPseudoToggleShape);

                        sf::Text pseudoToggle(monoFont_, showRightPseudocodePanel_ ? ">" : "<", 16);
                        pseudoToggle.setFillColor(showRightPseudocodePanel_ ? ThemeManager::current.bg : ThemeManager::current.textColor);
                        sf::FloatRect pseudoBounds = pseudoToggle.getLocalBounds();
                        pseudoToggle.setPosition({layout.rightPseudoToggleBtn.position.x + (layout.rightPseudoToggleBtn.size.x - pseudoBounds.size.x) * 0.5f - pseudoBounds.position.x,
                                                 layout.rightPseudoToggleBtn.position.y + (layout.rightPseudoToggleBtn.size.y - pseudoBounds.size.y) * 0.5f - pseudoBounds.position.y - 2.f});
                        window.draw(pseudoToggle);

                        // Icon overlays for image-enhanced controls.
                        auto drawIconInButton = [&](const sf::Texture& tex, const Button& btn, const sf::Color& tint) {
                            if (tex.getSize().x == 0 || tex.getSize().y == 0) {
                                return;
                            }
                            const sf::FloatRect rect = btn.bounds();
                            // Hide icons during animation to prevent overflow
                            if (rect.size.x < 12.0f) return;
                            sf::Sprite sp(tex);
                            const float targetH = std::max(12.0f, rect.size.y * 0.7f); // Phóng to icon để nổi bật
                            const float scale = targetH / static_cast<float>(tex.getSize().y);
                            sp.setScale({scale, scale});
                            const float w = static_cast<float>(tex.getSize().x) * scale;
                            const float h = static_cast<float>(tex.getSize().y) * scale;
                            sp.setPosition({rect.position.x + (rect.size.x - w) * 0.5f, rect.position.y + (rect.size.y - h) * 0.5f});
                            sf::Color finalTint = btn.isEnabled() ? tint : sf::Color(180, 180, 180, 150);
                            sp.setColor(btn.isEnabled() ? ThemeManager::current.textColor : sf::Color(180, 180, 180, 150));
                            window.draw(sp);
                        };

                        drawIconInButton(skipBackIconTex, controlButtons_[0], ThemeManager::current.textColor);
                        drawIconInButton(playing_ ? pauseIconTex : playIconTex, controlButtons_[2], ThemeManager::current.textColor);
                        drawIconInButton(skipForwardIconTex, controlButtons_[4], ThemeManager::current.textColor);

                        const sf::FloatRect backRect = backButton_.bounds();
                        sf::ConvexShape backButtonShape = createRoundedRect(backRect.size, 12.f);
                        backButtonShape.setPosition(backRect.position);
                        backButtonShape.setFillColor(ThemeManager::current.secondary);
                        window.draw(backButtonShape);

                        // Re-draw icon on top
                        backButton_.setIcon(&homeIconTex);
                        drawIconInButton(homeIconTex, backButton_, ThemeManager::current.primary);

                        sf::Text mainTitle(monoFont_, "Data Structure Visualization", 22);
                        mainTitle.setFillColor(ThemeManager::current.textColor);
                        mainTitle.setPosition({76.f, 26.f});
                        window.draw(mainTitle);

                        // 04 Minimum Spanning Tree Title matching exactly RBTVisualizer sizes & positions
                        sf::Text numText(monoFont_, "04", 30);
                        numText.setFillColor(ThemeManager::current.textColor);
                        numText.setPosition({67.f, 663.f});
                        window.draw(numText);

                        sf::Text titleLine1(monoFont_, "Minimum Spanning", 70);
                        titleLine1.setFillColor(ThemeManager::current.textColor);
                        titleLine1.setPosition({67.f, 695.f});
                        window.draw(titleLine1);

                        sf::Text titleLine2(monoFont_, "Tree", 70);
                        titleLine2.setFillColor(ThemeManager::current.primary);
                        titleLine2.setPosition({67.f, 765.f});
                        window.draw(titleLine2);

                        if (showLeftActions_) {
                            sf::Text setupTitle(font_, "System & Tools", 16);
                            setupTitle.setPosition({layout.leftPanel.position.x + 16.0f, layout.leftPanel.position.y + 16.0f});
                            setupTitle.setFillColor(ThemeManager::current.textColor); 
                            window.draw(setupTitle);

                            sf::Text editTitle(font_, "MST Actions", 16);
                            editTitle.setPosition({layout.leftPanel.position.x + 16.0f, layout.leftPanel.position.y + 176.0f});
                            editTitle.setFillColor(ThemeManager::current.textColor);
                            window.draw(editTitle);

                            // Vẽ đường phân cách ngang (Separator line)
                            sf::RectangleShape separator(sf::Vector2f(layout.leftPanel.size.x - 32.0f, 1.0f));
                            separator.setPosition({layout.leftPanel.position.x + 16.0f, layout.leftPanel.position.y + 356.0f});
                            separator.setFillColor(ThemeManager::current.secondary);
                            window.draw(separator);

                            sf::Text algoTitle(font_, "Algorithms", 16);
                            algoTitle.setPosition({layout.leftPanel.position.x + 16.0f, layout.leftPanel.position.y + 364.0f});
                            algoTitle.setFillColor(ThemeManager::current.textColor);
                            window.draw(algoTitle);
                        }

                        float rightStepInfoBottomY = rightStepBox.position.y + 104.0f;
                        if (showRightStepPanel_) {
                            float rightInfoY = rightStepBox.position.y + 24.0f;
                            sf::Text buildInfo(font_, timelineDirty_ ? "Action Required: Click BUILD to update."
                                                            : "Build is up to date.", 14);
                            buildInfo.setFillColor(timelineDirty_ ? sf::Color(245, 158, 11) : ThemeManager::current.primary);
                            const auto buildInfoLines =
                                wrapTextToWidth(font_, buildInfo.getString().toAnsiString(), 14, rightTextMax);
                            for (size_t i = 0; i < std::min<size_t>(3, buildInfoLines.size()); ++i) {
                                sf::Text line(font_, buildInfoLines[i], 14);
                                line.setPosition({rightInnerX, rightInfoY + static_cast<float>(i) * 20.f});
                                line.setFillColor(timelineDirty_ ? sf::Color(245, 158, 11) : ThemeManager::current.primary);
                                window.draw(line);
                            }
                            rightInfoY += static_cast<float>(std::min<size_t>(3, buildInfoLines.size())) * 20.f + 8.f;

                            const std::string workflowText = "Workflow: New Tree -> Add Node -> Edit Edges -> Build";
                            const auto workflowLines = wrapTextToWidth(font_, workflowText, 13, rightTextMax);
                            for (const auto& workflowLine : workflowLines) {
                                sf::Text line(font_, workflowLine, 13);
                                line.setPosition({rightInnerX, rightInfoY});
                                line.setFillColor(sf::Color(ThemeManager::current.textColor.r, ThemeManager::current.textColor.g, ThemeManager::current.textColor.b, 180));
                                window.draw(line);
                                rightInfoY += 20.f;
                            }
                            rightInfoY += 8.f;

                            sf::Text selectedNodeText(font_,
                                "Selected: " + std::string(selectedNodeId_ >= 0 ? "Node " + std::to_string(selectedNodeId_) : "None"), 14);
                            selectedNodeText.setPosition({rightInnerX, rightInfoY});
                            selectedNodeText.setFillColor(sf::Color(246, 225, 132));
                            clampTextToWidth(selectedNodeText, rightTextMax);
                            window.draw(selectedNodeText);
                            rightStepInfoBottomY = rightInfoY + 26.f;
                        }

                        const int n = static_cast<int>(adjacencyMatrix_.size());
                        const float matrixCell = matrixCellForSize(n);
                        const sf::Vector2f matrixOrigin = matrixOriginForSize(n);

                        if (canvasMode_ == MstCanvasMode::Matrix) {
                            sf::Text matrixHelp(font_, "Adjacency Matrix: click a cell to edit weight (enter 0 to remove)", 14);
                            matrixHelp.setPosition({matrixOrigin.x, matrixOrigin.y - 34.f});
                            matrixHelp.setFillColor(ThemeManager::current.textColor);
                            clampTextToWidth(matrixHelp, gGraphViewport.position.x + gGraphViewport.size.x - matrixOrigin.x - 8.0f);
                            window.draw(matrixHelp);

                            int deletedCount = 0;
                            for (bool alive : nodeAlive_) {
                                if (!alive) {
                                    ++deletedCount;
                                }
                            }

                            if (deletedCount > 0) {
                                sf::Text matrixLegend(monoFont_, "x = deleted node, disabled row/column (" +
                                                        std::to_string(deletedCount) + " removed)", 12);
                                matrixLegend.setPosition({matrixOrigin.x, matrixOrigin.y - 16.f});
                                matrixLegend.setFillColor(sf::Color(170, 96, 96));
                                clampTextToWidth(matrixLegend, gGraphViewport.position.x + gGraphViewport.size.x - matrixOrigin.x - 8.0f);
                                window.draw(matrixLegend);
                            }

                            for (int r = -1; r < n; ++r) {
                                for (int c = -1; c < n; ++c) {
                                    sf::FloatRect cellRect({matrixOrigin.x + (static_cast<float>(c + 1) * matrixCell) + 1.0f,
                                                            matrixOrigin.y + (static_cast<float>(r + 1) * matrixCell) + 1.0f},
                                                           {matrixCell - 2.0f, matrixCell - 2.0f});
                                    sf::Color cellFill;
                                    float outlineThick = 0.0f;
                                    sf::Color cellOutline = sf::Color::Transparent;

                                    if (r == -1 || c == -1) {
                                        const bool inactiveHeader =
                                            (r == -1 && c >= 0 && c < n && c < static_cast<int>(nodeAlive_.size()) && !nodeAlive_[static_cast<size_t>(c)]) ||
                                            (c == -1 && r >= 0 && r < n && r < static_cast<int>(nodeAlive_.size()) && !nodeAlive_[static_cast<size_t>(r)]);
                                        cellFill = inactiveHeader ? sf::Color(220, 196, 196) : sf::Color(183, 192, 201);
                                        cellFill = ThemeManager::current.secondary;
                                    } else if (r == c) {
                                        const bool inactive = r < static_cast<int>(nodeAlive_.size()) && !nodeAlive_[static_cast<size_t>(r)];
                                        cellFill = inactive ? sf::Color(220, 196, 196) : sf::Color(ThemeManager::current.secondary.r, ThemeManager::current.secondary.g, ThemeManager::current.secondary.b, 100);
                                    } else {
                                        const bool inactive =
                                            r < static_cast<int>(nodeAlive_.size()) && c < static_cast<int>(nodeAlive_.size()) &&
                                            (!nodeAlive_[static_cast<size_t>(r)] || !nodeAlive_[static_cast<size_t>(c)]);
                                        if (inactive) {
                                            cellFill = sf::Color(ThemeManager::current.secondary.r, ThemeManager::current.secondary.g, ThemeManager::current.secondary.b, 50);
                                        } else {
                                            if (enteringEdgeWeight_ && ((r == pendingMatrixRow_ && c == pendingMatrixCol_) || (r == pendingMatrixCol_ && c == pendingMatrixRow_))) {
                                                cellFill = ThemeManager::current.primary;
                                                outlineThick = 2.0f;
                                                cellOutline = ThemeManager::current.primaryLight;
                                            } else {
                                                cellFill = adjacencyMatrix_[static_cast<size_t>(r)][static_cast<size_t>(c)] > 0
                                                                    ? ThemeManager::current.primaryLight
                                                                    : ThemeManager::current.bg;
                                            }
                                        }
                                    }
                                    sf::ConvexShape cellShape = createRoundedRect(cellRect.size, 4.f);
                                    cellShape.setPosition(cellRect.position);
                                    cellShape.setFillColor(cellFill);
                                    cellShape.setOutlineColor(outlineThick > 0.f ? cellOutline : sf::Color::Transparent);
                                    cellShape.setOutlineThickness(outlineThick);
                                    window.draw(cellShape);

                                    sf::Text t(font_, "", 13);
                                    t.setFillColor(ThemeManager::current.textColor);
                                    if (r == -1 && c >= 0) {
                                        if (c < static_cast<int>(nodeAlive_.size()) && !nodeAlive_[static_cast<size_t>(c)]) {
                                            t.setString("x");
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
                                        } else {
                                            if (enteringEdgeWeight_ && ((r == pendingMatrixRow_ && c == pendingMatrixCol_) || (r == pendingMatrixCol_ && c == pendingMatrixRow_))) {
                                                t.setString(edgeWeightInput_ + "_");
                                                t.setFillColor(sf::Color(20, 20, 20));
                                            } else {
                                                t.setString(std::to_string(adjacencyMatrix_[static_cast<size_t>(r)][static_cast<size_t>(c)]));
                                            }
                                        }
                                    }
                                sf::FloatRect textBounds = t.getLocalBounds();
                                t.setPosition({cellRect.position.x + (cellRect.size.x - textBounds.size.x) * 0.5f - textBounds.position.x,
                                              cellRect.position.y + (cellRect.size.y - textBounds.size.y) * 0.5f - textBounds.position.y});
                                    window.draw(t);
                                }
                            }
                        }

                        if (showRightStepPanel_) {
                            const std::string stepSummary =
                                (step != nullptr) ? step->description : "⚪ Status: Waiting to start. Press Build.";
                            const float descStartY = std::max(rightStepBox.position.y + 114.0f, rightStepInfoBottomY + 8.0f);
                            const float descMaxY = rightStepBox.position.y + rightStepBox.size.y - 14.0f;
                            const auto descLines = wrapTextToWidth(font_, stepSummary, 15, rightTextMax);
                            const size_t maxByHeight =
                                static_cast<size_t>(std::max(0.0f, std::floor((descMaxY - descStartY) / 24.0f)));
                            const size_t shownDescLines = std::min(descLines.size(), maxByHeight);
                            for (size_t i = 0; i < shownDescLines; ++i) {
                                sf::Text desc(font_, descLines[i], 15);
                                desc.setPosition({rightInnerX, descStartY + static_cast<float>(i) * 24.0f});
                                desc.setFillColor(ThemeManager::current.textColor);
                                window.draw(desc);
                            }
                        }

                        if (showRightPseudocodePanel_) {
                            const float pseudoY = rightPseudoBox.position.y + 14.0f;
                            sf::Text pseudoTitle(font_, "Pseudocode", 18);
                            pseudoTitle.setStyle(sf::Text::Bold);
                            pseudoTitle.setPosition({rightInnerX, pseudoY + 4.f});
                            pseudoTitle.setFillColor(sf::Color(246, 225, 132));
                            clampTextToWidth(pseudoTitle, rightTextMax);
                            window.draw(pseudoTitle);

                            float pseudoLineY = pseudoY + 34.f;
                            const float pseudoMaxY = rightPseudoBox.position.y + rightPseudoBox.size.y - 18.f;
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
                                const auto wrappedPseudo = wrapTextToWidth(font_, pseudocode_[i], 14, rightTextMax - 16.0f);
                                for (size_t w = 0; w < wrappedPseudo.size(); ++w) {
                                    if (pseudoLineY > pseudoMaxY) {
                                        break;
                                    }
                                    sf::Color textColor = highlighted ? ThemeManager::current.bg : ThemeManager::current.textColor;
                                    const float indentX = (w == 0) ? 0.0f : 16.0f;
                                    if (highlighted) {
                                        sf::RectangleShape lineBg({rightTextMax + 12.0f, 22.0f});
                                        lineBg.setPosition({rightInnerX - 6.0f, pseudoLineY - 2.0f});
                                        lineBg.setFillColor(ThemeManager::current.primary);
                                        window.draw(lineBg);
                                    }
                                    sf::Text line(font_, wrappedPseudo[w], 14);
                                    line.setPosition({rightInnerX + indentX, pseudoLineY});
                                    line.setFillColor(textColor);
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

                        sf::Text selectedTitle(font_, selectedName, 28);
                        selectedTitle.setPosition({430.f, 18.f});
                        selectedTitle.setFillColor(ThemeManager::current.textColor);
                        window.draw(selectedTitle);
                    }
                }

                window.display();
            }
        }
