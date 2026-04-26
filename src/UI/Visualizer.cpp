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

constexpr float kMatrixOriginX = 620.0f;
constexpr float kMatrixOriginY = 140.0f;
constexpr float kMatrixBaseCell = 34.0f;

constexpr float kLeftPanelX = 12.0f;
constexpr float kLeftPanelY = 12.0f;
constexpr float kLeftPanelW = 248.0f;
constexpr float kLeftPanelH = 560.0f;

constexpr float kRightPanelX = 948.0f;
constexpr float kRightPanelY = 12.0f;
constexpr float kRightPanelW = 320.0f;
constexpr float kRightPanelH = 560.0f;

constexpr float kBottomPanelX = 12.0f;
constexpr float kBottomPanelY = 628.0f;
constexpr float kBottomPanelW = 1256.0f;
constexpr float kBottomPanelH = 80.0f;

constexpr float kGraphMinX = 274.0f;
constexpr float kGraphMaxX = 934.0f;
constexpr float kGraphMinY = 110.0f;
constexpr float kGraphMaxY = 560.0f;

sf::Vector2f graphToScreen(float nx, float ny) {
    return sf::Vector2f(kGraphMinX + nx * (kGraphMaxX - kGraphMinX),
                        kGraphMinY + ny * (kGraphMaxY - kGraphMinY));
}

sf::Vector2f matrixOriginForSize(int n) {
    const float cell = std::clamp(kMatrixBaseCell - std::max(0, n - 10) * 0.55f, 14.0f, kMatrixBaseCell);
    const float tableW = cell * static_cast<float>(n + 1);
    const float tableH = cell * static_cast<float>(n + 1);
    const float centerAreaW = kGraphMaxX - kGraphMinX;
    const float centerAreaH = kGraphMaxY - kGraphMinY;
    const float x = kGraphMinX + (centerAreaW - tableW) * 0.5f;
    const float y = kGraphMinY + (centerAreaH - tableH) * 0.5f;
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
    graph_.addNode(0, 0.30f, 0.35f);
    graph_.addNode(1, 0.50f, 0.22f);
    graph_.addNode(2, 0.72f, 0.34f);
    graph_.addNode(3, 0.66f, 0.58f);
    graph_.addNode(4, 0.42f, 0.66f);
    graph_.addNode(5, 0.23f, 0.54f);

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
    return mousePos.x >= kGraphMinX && mousePos.x <= kGraphMaxX && mousePos.y >= kGraphMinY && mousePos.y <= kGraphMaxY;
}

sf::Vector2f Visualizer::screenToGraphNormalized(const sf::Vector2f& mousePos) const {
    const float nx = (mousePos.x - kGraphMinX) / (kGraphMaxX - kGraphMinX);
    const float ny = (mousePos.y - kGraphMinY) / (kGraphMaxY - kGraphMinY);
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
            "1. Pick start node and push frontier edges",
            "2. While priority queue is not empty",
            "3. Pop minimum edge E",
            "4. If destination is already in tree, reject E",
            "5. Else accept E",
            "6. Add destination frontier edges",
            "7. Done"};
    }
}

void Visualizer::onActionNewGraph() {
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
    rebuildTimeline();
}

void Visualizer::onActionRandom() {
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
    rebuildTimeline();
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
        Button("Add Node", font_),
        Button("Delete Node", font_),
        Button("Add/Modify Edges", font_),
        Button("Random", font_),
        Button("Build", font_),
        Button("View: Graph", font_),
        Button("Kruskal", font_),
        Button("Prim", font_)};

    backButton_ = Button("Back To Menu", font_);
    backButton_.setSize(220.f, 28.f);
    backButton_.setPosition(26.f, 22.f);

    const float buttonW = 220.f;
    const float buttonH = 24.f;
    const float leftX = 26.f;
    const float graphSetupY = 76.f;
    const float graphGap = 30.f;

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

    // Left panel: Algorithms controls.
    controlButtons_[12].setSize(buttonW, buttonH);
    controlButtons_[12].setPosition(leftX, 338.f);
    controlButtons_[13].setSize(buttonW, buttonH);
    controlButtons_[13].setPosition(leftX, 368.f);

    // Bottom panel: Playback bar controls.
    const float pbY = 642.f;
    const float pbW = 134.f;
    const float pbGap = 10.f;
    const float pbStartX = 24.f;
    for (int i = 0; i <= 4; ++i) {
        controlButtons_[i].setSize(pbW, 28.f);
        controlButtons_[i].setPosition(pbStartX + static_cast<float>(i) * (pbW + pbGap), pbY);
    }

    speedSlider_ = Slider(760.f, 650.f, 480.f, 1.f, 10.f, 2.f, font_);

    controlButtons_[2].setStyleRole(Button::StyleRole::Play);
    controlButtons_[7].setStyleRole(Button::StyleRole::Danger);
    controlButtons_[12].setStyleRole(Button::StyleRole::Algorithm);
    controlButtons_[13].setStyleRole(Button::StyleRole::Algorithm);

    sf::RenderWindow window(sf::VideoMode(config::kWindowWidth, config::kWindowHeight),
                            "Data Structure Visualization");
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
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
                        if (speedSlider_.contains(mousePos)) {
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
                                case 6: onActionAddNode(); break;
                                case 7: onActionRemoveNode(); break;
                                case 8: onActionAddEdgeMode(); break;
                                case 9: onActionRandom(); break;
                                case 10: onActionBuild(); break;
                                case 11:
                                    canvasMode_ = (canvasMode_ == MstCanvasMode::Graph) ? MstCanvasMode::Matrix
                                                                                         : MstCanvasMode::Graph;
                                    break;
                                case 12: onActionKruskal(); break;
                                case 13: onActionPrim(); break;
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
                if (currentScreen_ == Screen::Visualization && isMstSelected()) {
                    const float prevSpeed = speed_;
                    const sf::Vector2f mousePos =
                        window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
                    speedSlider_.onMouseMoved(mousePos);
                    speed_ = speedSlider_.getValue();
                    if (playing_ && std::abs(speed_ - prevSpeed) > 0.001f) {
                        playClock_.restart();
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

            controlButtons_[0].setEnabled(!timelineDirty_ && hasTimeline && !atStart);          // Skip Back
            controlButtons_[1].setEnabled(!timelineDirty_ && hasTimeline && !atStart && !playing_); // Step Back
            controlButtons_[2].setEnabled(!timelineDirty_ && hasTimeline);                      // Play/Pause
            controlButtons_[3].setEnabled(!timelineDirty_ && hasTimeline && !atEnd && !playing_);    // Step Forward
            controlButtons_[4].setEnabled(!timelineDirty_ && hasTimeline && !atEnd);            // Skip Forward
            controlButtons_[2].setLabel(playing_ ? "Pause" : "Play");

            controlButtons_[5].setEnabled(!playing_);    // New Graph
            controlButtons_[6].setEnabled(!playing_);    // Add Node
            controlButtons_[7].setEnabled(!playing_ && canDeleteAny); // Delete Node
            controlButtons_[8].setEnabled(!playing_ && canvasMode_ == MstCanvasMode::Graph); // Add/Modify
            controlButtons_[9].setEnabled(!playing_);    // Random
            controlButtons_[10].setEnabled(!playing_);   // Build
            controlButtons_[11].setEnabled(!playing_);   // View
            controlButtons_[12].setEnabled(!playing_);   // Kruskal
            controlButtons_[13].setEnabled(!playing_);   // Prim

            controlButtons_[11].setLabel(canvasMode_ == MstCanvasMode::Graph ? "View: Matrix" : "View: Graph");
            controlButtons_[10].setLabel(timelineDirty_ ? "Build *" : "Build");

            controlButtons_[6].setSelected(placingNode_);
            controlButtons_[10].setSelected(timelineDirty_);
            controlButtons_[7].setSelected(deletingNodeMode_);
            controlButtons_[8].setSelected(addingEdgeMode_ || enteringEdgeWeight_ || pendingEdgeFrom_ >= 0);
            controlButtons_[11].setSelected(canvasMode_ == MstCanvasMode::Matrix);
            controlButtons_[12].setSelected(algorithmType_ == algo::AlgorithmType::Kruskal);
            controlButtons_[13].setSelected(algorithmType_ == algo::AlgorithmType::Prim);
        }

        window.clear(config::kBackgroundColor);

        if (currentScreen_ == Screen::Menu) {
            sf::Text title("Choose Data Structure", font_, 34);
            title.setPosition(430.f, 130.f);
            title.setFillColor(sf::Color(240, 240, 245));
            window.draw(title);

            for (auto& b : menuButtons_) {
                b.draw(window);
            }
        } else {
            sf::Text topStatus(playing_ ? "Animation Running" : "Animation Paused", font_, 16);
            if (isMstSelected() && !playing_ && !animation_.empty() && animation_.currentIndex() >= animation_.totalSteps() - 1) {
                topStatus.setString("Animation Completed");
            }
            topStatus.setPosition(20.f, 116.f);
            topStatus.setFillColor(sf::Color(55, 55, 60));
            window.draw(topStatus);

            backButton_.draw(window);

            RenderViewModel vm;
            vm.kind = selectedStructure_;
            vm.graph = &graph_;
            vm.selectedNodeId = selectedNodeId_;
            if (step != nullptr) {
                vm.highlightedEdges = step->highlightedEdges;
                vm.candidateEdges = step->candidateEdges;
                vm.highlightedNodes = step->highlightedNodes;
            }
            if (!isMstSelected() || canvasMode_ == MstCanvasMode::Graph) {
                Renderer::draw(window, vm, font_);
            }

            if (isMstSelected()) {
                const float rightTextMax = kRightPanelW - 24.0f;

                drawRoundedBox(window,
                               sf::FloatRect(kLeftPanelX, kLeftPanelY, kLeftPanelW, kLeftPanelH),
                               10.0f,
                               1.0f,
                               sf::Color(20, 24, 33, 220),
                               sf::Color(70, 78, 96));

                drawRoundedBox(window,
                               sf::FloatRect(kRightPanelX, kRightPanelY, kRightPanelW, kRightPanelH),
                               10.0f,
                               1.0f,
                               sf::Color(20, 24, 33, 220),
                               sf::Color(70, 78, 96));

                drawRoundedBox(window,
                               sf::FloatRect(kBottomPanelX, kBottomPanelY, kBottomPanelW, kBottomPanelH),
                               10.0f,
                               1.0f,
                               sf::Color(20, 24, 33, 220),
                               sf::Color(70, 78, 96));

                for (auto& b : controlButtons_) {
                    b.draw(window);
                }
                speedSlider_.draw(window);

                sf::Text setupTitle("Graph Setup", font_, 16);
                setupTitle.setPosition(26.f, 56.f);
                setupTitle.setFillColor(sf::Color(236, 236, 240));
                window.draw(setupTitle);

                sf::Text algoTitle("Algorithms", font_, 16);
                algoTitle.setPosition(26.f, 312.f);
                algoTitle.setFillColor(sf::Color(236, 236, 240));
                window.draw(algoTitle);

                sf::Text playTitle("Playback", font_, 16);
                playTitle.setPosition(24.f, 590.f);
                playTitle.setFillColor(sf::Color(236, 236, 240));
                window.draw(playTitle);

                sf::Text status("Step: " + std::to_string(animation_.currentIndex()) + " / " +
                                    std::to_string(std::max(0, animation_.totalSteps() - 1)),
                                monoFont_,
                                13);
                status.setPosition(960.f, 24.f);
                status.setFillColor(sf::Color::White);
                clampTextToWidth(status, rightTextMax);
                window.draw(status);

                float rightInfoY = 44.f;
                sf::Text buildInfo(timelineDirty_ ? "Build is required to update visualization steps."
                                                  : "Build: regenerate algorithm steps after graph edits.",
                                  monoFont_,
                                  13);
                buildInfo.setFillColor(timelineDirty_ ? sf::Color(255, 221, 92) : sf::Color(180, 185, 190));
                const auto buildInfoLines = wrapTextToWidth(monoFont_, buildInfo.getString().toAnsiString(), 13, rightTextMax);
                for (size_t i = 0; i < std::min<size_t>(2, buildInfoLines.size()); ++i) {
                    sf::Text line(buildInfoLines[i], monoFont_, 13);
                    line.setPosition(960.f, rightInfoY + static_cast<float>(i) * 16.f);
                    line.setFillColor(timelineDirty_ ? sf::Color(255, 221, 92) : sf::Color(180, 185, 190));
                    window.draw(line);
                }
                rightInfoY += static_cast<float>(std::min<size_t>(2, buildInfoLines.size())) * 16.f + 4.f;

                if (timelineDirty_) {
                    sf::Text dirtyHint("Graph changed. Press Build.", monoFont_, 13);
                    dirtyHint.setPosition(960.f, rightInfoY);
                    dirtyHint.setFillColor(sf::Color(255, 221, 92));
                    clampTextToWidth(dirtyHint, rightTextMax);
                    window.draw(dirtyHint);
                    rightInfoY += 18.f;
                }

                sf::Text workflowHint("Workflow: New Graph -> Add Node", monoFont_, 12);
                workflowHint.setPosition(960.f, rightInfoY);
                workflowHint.setFillColor(sf::Color(255, 221, 92));
                clampTextToWidth(workflowHint, rightTextMax);
                window.draw(workflowHint);
                rightInfoY += 16.f;

                sf::Text workflowHint2("-> Add/Modify Edges -> Build", monoFont_, 12);
                workflowHint2.setPosition(960.f, rightInfoY);
                workflowHint2.setFillColor(sf::Color(255, 221, 92));
                clampTextToWidth(workflowHint2, rightTextMax);
                window.draw(workflowHint2);
                rightInfoY += 18.f;

                sf::Text selectedNodeText("Selected Node: " +
                                              std::string(selectedNodeId_ >= 0 ? std::to_string(selectedNodeId_) : "None"),
                                          monoFont_,
                                          13);
                selectedNodeText.setPosition(960.f, rightInfoY);
                selectedNodeText.setFillColor(sf::Color(220, 220, 220));
                clampTextToWidth(selectedNodeText, rightTextMax);
                window.draw(selectedNodeText);

                if (algorithmType_ == algo::AlgorithmType::Prim) {
                    float hintY = 404.f;
                    const auto drawHintWrapped = [&](const std::string& text, const sf::Color& color, unsigned int size) {
                        const auto lines = wrapTextToWidth(monoFont_, text, size, kLeftPanelW - 24.0f);
                        for (const auto& ln : lines) {
                            sf::Text t(ln, monoFont_, size);
                            t.setPosition(26.f, hintY);
                            t.setFillColor(color);
                            window.draw(t);
                            hintY += static_cast<float>(size) + 5.f;
                        }
                        hintY += 2.f;
                    };

                    drawHintWrapped("Prim Start Node: " +
                                        std::string(selectedNodeId_ >= 0 ? std::to_string(selectedNodeId_) : "0 (default)"),
                                    sf::Color(255, 221, 92),
                                    14);
                    drawHintWrapped("Tip: click a node, then Build to run Prim from that node", sf::Color(255, 221, 92), 13);

                    if (placingNode_) {
                        drawHintWrapped("Add Node mode: click graph area to place new node", sf::Color(255, 221, 92), 13);
                    }

                    if (deletingNodeMode_) {
                        drawHintWrapped("Delete mode: click node ID > 0 (node 0 is protected)", sf::Color(255, 221, 92), 13);
                    }

                    if (addingEdgeMode_) {
                        drawHintWrapped("Add/Modify mode: click two nodes", sf::Color(255, 221, 92), 13);
                        if (pendingEdgeFrom_ >= 0) {
                            drawHintWrapped("From node: " + std::to_string(pendingEdgeFrom_), sf::Color(255, 221, 92), 13);
                        }
                    }

                    if (enteringEdgeWeight_) {
                        drawHintWrapped("Edge weight 1-999 then Enter:", sf::Color(255, 230, 145), 13);
                        sf::Text valueLine("> " + (edgeWeightInput_.empty() ? std::string("_") : edgeWeightInput_), monoFont_, 14);
                        valueLine.setPosition(26.f, hintY);
                        valueLine.setFillColor(sf::Color(255, 245, 170));
                        window.draw(valueLine);
                    }
                }

                if (algorithmType_ != algo::AlgorithmType::Prim) {
                    float hintY = 404.f;
                    const auto drawHintWrapped = [&](const std::string& text, const sf::Color& color, unsigned int size) {
                        const auto lines = wrapTextToWidth(monoFont_, text, size, kLeftPanelW - 24.0f);
                        for (const auto& ln : lines) {
                            sf::Text t(ln, monoFont_, size);
                            t.setPosition(26.f, hintY);
                            t.setFillColor(color);
                            window.draw(t);
                            hintY += static_cast<float>(size) + 5.f;
                        }
                        hintY += 2.f;
                    };

                    if (placingNode_) {
                        drawHintWrapped("Add Node mode: click graph area to place new node", sf::Color(255, 221, 92), 13);
                    }

                    if (deletingNodeMode_) {
                        drawHintWrapped("Delete mode: click node ID > 0 (node 0 is protected)", sf::Color(255, 221, 92), 13);
                    }

                    if (addingEdgeMode_) {
                        drawHintWrapped("Add/Modify mode: click two nodes", sf::Color(255, 221, 92), 13);
                        if (pendingEdgeFrom_ >= 0) {
                            drawHintWrapped("From node: " + std::to_string(pendingEdgeFrom_), sf::Color(255, 221, 92), 13);
                        }
                    }

                    if (enteringEdgeWeight_) {
                        drawHintWrapped("Edge weight 1-999 then Enter:", sf::Color(255, 230, 145), 13);
                        sf::Text valueLine("> " + (edgeWeightInput_.empty() ? std::string("_") : edgeWeightInput_), monoFont_, 14);
                        valueLine.setPosition(26.f, hintY);
                        valueLine.setFillColor(sf::Color(255, 245, 170));
                        window.draw(valueLine);
                    }
                }

                const int n = static_cast<int>(adjacencyMatrix_.size());
                const float matrixCell = matrixCellForSize(n);
                const sf::Vector2f matrixOrigin = matrixOriginForSize(n);

                if (canvasMode_ == MstCanvasMode::Matrix) {
                    sf::Text matrixHelp("Adjacency Matrix: click a cell to toggle edge", font_, 14);
                    matrixHelp.setPosition(matrixOrigin.x, matrixOrigin.y - 34.f);
                    matrixHelp.setFillColor(sf::Color(210, 210, 210));
                    clampTextToWidth(matrixHelp, kGraphMaxX - matrixOrigin.x - 8.0f);
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
                        matrixLegend.setFillColor(sf::Color(220, 120, 120));
                        clampTextToWidth(matrixLegend, kGraphMaxX - matrixOrigin.x - 8.0f);
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
                                cell.setFillColor(inactiveHeader ? sf::Color(50, 36, 36) : sf::Color(55, 60, 72));
                            } else if (r == c) {
                                const bool inactive = r < static_cast<int>(nodeAlive_.size()) && !nodeAlive_[static_cast<size_t>(r)];
                                cell.setFillColor(inactive ? sf::Color(50, 36, 36) : sf::Color(38, 42, 50));
                            } else {
                                const bool inactive =
                                    r < static_cast<int>(nodeAlive_.size()) && c < static_cast<int>(nodeAlive_.size()) &&
                                    (!nodeAlive_[static_cast<size_t>(r)] || !nodeAlive_[static_cast<size_t>(c)]);
                                if (inactive) {
                                    cell.setFillColor(sf::Color(42, 32, 32));
                                } else {
                                    cell.setFillColor(adjacencyMatrix_[static_cast<size_t>(r)][static_cast<size_t>(c)] > 0
                                                          ? sf::Color(50, 95, 55)
                                                          : sf::Color(36, 40, 48));
                                }
                            }
                            window.draw(cell);

                            sf::Text t("", font_, 13);
                            t.setFillColor(sf::Color(232, 232, 232));
                            if (r == -1 && c >= 0) {
                                if (c < static_cast<int>(nodeAlive_.size()) && !nodeAlive_[static_cast<size_t>(c)]) {
                                    t.setString("x");
                                    t.setFillColor(sf::Color(220, 120, 120));
                                } else {
                                    t.setString(std::to_string(c));
                                }
                            } else if (c == -1 && r >= 0) {
                                if (r < static_cast<int>(nodeAlive_.size()) && !nodeAlive_[static_cast<size_t>(r)]) {
                                    t.setString("x");
                                    t.setFillColor(sf::Color(220, 120, 120));
                                } else {
                                    t.setString(std::to_string(r));
                                }
                            } else if (r >= 0 && c >= 0) {
                                if ((r < static_cast<int>(nodeAlive_.size()) && !nodeAlive_[static_cast<size_t>(r)]) ||
                                    (c < static_cast<int>(nodeAlive_.size()) && !nodeAlive_[static_cast<size_t>(c)])) {
                                    t.setString("-");
                                    t.setFillColor(sf::Color(180, 120, 120));
                                } else {
                                    t.setString(std::to_string(adjacencyMatrix_[static_cast<size_t>(r)][static_cast<size_t>(c)]));
                                }
                            }
                            t.setPosition(cell.getPosition().x + 8.f, cell.getPosition().y + 5.f);
                            window.draw(t);
                        }
                    }
                }

                if (step != nullptr) {
                    // Center status box: keep current algorithm update near graph focus area.
                    const float centerInfoW = (kGraphMaxX - kGraphMinX) - 80.f;
                    const float centerInfoX = kGraphMinX + ((kGraphMaxX - kGraphMinX) - centerInfoW) * 0.5f;
                    const float centerInfoY = 66.f;
                    const auto descLines = wrapTextToWidth(monoFont_, step->description, 20, centerInfoW - 28.f);
                    const size_t shownDescLines = std::min<size_t>(2, descLines.size());
                    const float centerInfoH = 16.f + static_cast<float>(shownDescLines) * 26.f;

                    drawRoundedBox(window,
                                   sf::FloatRect(centerInfoX, centerInfoY, centerInfoW, centerInfoH),
                                   12.0f,
                                   1.0f,
                                   sf::Color(20, 24, 33, 220),
                                   sf::Color(70, 78, 96));

                    drawRoundedFill(window,
                                    sf::FloatRect(centerInfoX + 10.f, centerInfoY + 6.f, centerInfoW - 20.f, 3.f),
                                    1.5f,
                                    sf::Color(255, 214, 107, 210));

                    for (size_t i = 0; i < shownDescLines; ++i) {
                        sf::Text desc(descLines[i], monoFont_, 20);
                        desc.setPosition(centerInfoX + 14.f, centerInfoY + 8.f + static_cast<float>(i) * 26.f);
                        desc.setFillColor(sf::Color(220, 220, 220));
                        window.draw(desc);
                    }

                    const float pseudoX = 960.f;
                    const float pseudoY = 184.f;

                    sf::RectangleShape divider(sf::Vector2f(kRightPanelW - 20.f, 1.f));
                    divider.setPosition(kRightPanelX + 10.f, pseudoY - 14.f);
                    divider.setFillColor(sf::Color(70, 78, 96));
                    window.draw(divider);

                    sf::Text pseudoTitle("Pseudocode", monoFont_, 17);
                    pseudoTitle.setPosition(pseudoX, pseudoY);
                    pseudoTitle.setFillColor(sf::Color(240, 240, 245));
                    clampTextToWidth(pseudoTitle, rightTextMax);
                    window.draw(pseudoTitle);

                    float pseudoLineY = pseudoY + 28.f;
                    for (size_t i = 0; i < pseudocode_.size(); ++i) {
                        const int lineNumber = static_cast<int>(i + 1);
                        bool highlighted =
                            std::find(step->pseudocodeLines.begin(), step->pseudocodeLines.end(), lineNumber) !=
                            step->pseudocodeLines.end();

                        const auto wrappedPseudo = wrapTextToWidth(monoFont_, pseudocode_[i], 13, rightTextMax);
                        for (size_t w = 0; w < wrappedPseudo.size(); ++w) {
                            const std::string rendered = (w == 0) ? wrappedPseudo[w] : ("   " + wrappedPseudo[w]);
                            sf::Text line(rendered, monoFont_, 13);
                            line.setPosition(pseudoX, pseudoLineY);
                            line.setFillColor(highlighted ? sf::Color(255, 215, 0) : sf::Color(185, 185, 190));
                            window.draw(line);
                            pseudoLineY += 20.f;
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
                selectedTitle.setFillColor(sf::Color(240, 240, 245));
                window.draw(selectedTitle);
            }
        }

        window.display();
    }
}
