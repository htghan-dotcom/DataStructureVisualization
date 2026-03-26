#include "Visualizer.h"

#include <cmath>

#include "../config/Config.h"
#include "../data/GraphLoader.h"

namespace {
int autoWeight(int a, int b) {
    return ((a + 3) * (b + 5)) % 19 + 1;
}

constexpr float kMatrixOriginX = 620.0f;
constexpr float kMatrixOriginY = 360.0f;
constexpr float kMatrixBaseCell = 28.0f;

constexpr float kGraphMinX = 230.0f;
constexpr float kGraphMaxX = 840.0f;
constexpr float kGraphMinY = 220.0f;
constexpr float kGraphMaxY = 680.0f;

sf::Vector2f graphToScreen(float nx, float ny) {
    return sf::Vector2f(kGraphMinX + nx * (kGraphMaxX - kGraphMinX),
                        kGraphMinY + ny * (kGraphMaxY - kGraphMinY));
}

sf::Vector2f matrixOriginForSize(int n) {
    const float cell = std::clamp(kMatrixBaseCell - std::max(0, n - 10) * 0.55f, 14.0f, kMatrixBaseCell);
    const float tableW = cell * static_cast<float>(n + 1);
    const float x = static_cast<float>(config::kWindowWidth) - tableW - 20.0f;
    return sf::Vector2f(x, kMatrixOriginY);
}

float matrixCellForSize(int n) {
    return std::clamp(kMatrixBaseCell - std::max(0, n - 10) * 0.55f, 14.0f, kMatrixBaseCell);
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
    const int n = static_cast<int>(graph_.getNodes().size());
    adjacencyMatrix_.assign(static_cast<size_t>(n), std::vector<int>(static_cast<size_t>(n), 0));
    for (const auto& edge : graph_.getEdges()) {
        if (edge.from >= 0 && edge.to >= 0 && edge.from < n && edge.to < n) {
            adjacencyMatrix_[static_cast<size_t>(edge.from)][static_cast<size_t>(edge.to)] = edge.weight;
            adjacencyMatrix_[static_cast<size_t>(edge.to)][static_cast<size_t>(edge.from)] = edge.weight;
        }
    }
}

void Visualizer::rebuildGraphFromAdjacencyMatrix() {
    const int n = static_cast<int>(adjacencyMatrix_.size());
    const auto oldNodes = graph_.getNodes();

    Graph rebuilt;
    for (int i = 0; i < n; ++i) {
        if (i < static_cast<int>(oldNodes.size())) {
            rebuilt.addNode(i, oldNodes[static_cast<size_t>(i)].x, oldNodes[static_cast<size_t>(i)].y);
        } else {
            const float angle = 0.7f * static_cast<float>(i);
            float x = 0.55f + 0.28f * std::cos(angle);
            float y = 0.58f + 0.28f * std::sin(angle);
            rebuilt.addNode(i, std::clamp(x, 0.18f, 0.92f), std::clamp(y, 0.22f, 0.90f));
        }
    }

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
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

    int& weightCell = adjacencyMatrix_[static_cast<size_t>(row)][static_cast<size_t>(col)];
    if (weightCell > 0) {
        weightCell = 0;
        adjacencyMatrix_[static_cast<size_t>(col)][static_cast<size_t>(row)] = 0;
    } else {
        const int w = autoWeight(row, col);
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
            "1. sort edges by weight",
            "2. for each edge e in sorted edges",
            "3.   mark e as candidate",
            "4.   if endpoints of e in different sets",
            "5.     accept e and union sets",
            "6.   else reject e",
            "7. done"};
    } else {
        pseudocode_ = {
            "1. pick start node and push frontier edges",
            "2. while priority queue is not empty",
            "3.   pop minimum edge e",
            "4.   if destination already in tree reject",
            "5.   else accept e",
            "6.   add destination frontier edges",
            "7. done"};
    }
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
    const auto steps = algo::AlgorithmFactory::buildSteps(algorithmType_, graph_);
    animation_.setSteps(steps);
    playing_ = false;
    state_ = steps.empty() ? AppState::Error : AppState::Paused;
    timelineDirty_ = false;
}

void Visualizer::onSelectStructure(RenderViewKind kind) {
    selectedStructure_ = kind;
    currentScreen_ = Screen::Visualization;
    placingNode_ = false;
    addingEdgeMode_ = false;
    pendingEdgeFrom_ = -1;
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
    addingEdgeMode_ = false;
    pendingEdgeFrom_ = -1;
    rebuildTimeline();
}

void Visualizer::onActionRandom() {
    graph_ = GraphLoader::createRandomGraph(10, 30);
    selectedNodeId_ = -1;
    syncAdjacencyMatrixFromGraph();
    canvasMode_ = MstCanvasMode::Graph;
    placingNode_ = false;
    addingEdgeMode_ = false;
    pendingEdgeFrom_ = -1;
    rebuildTimeline();
}

void Visualizer::onActionAddNode() {
    placingNode_ = true;
    addingEdgeMode_ = false;
    pendingEdgeFrom_ = -1;
    canvasMode_ = MstCanvasMode::Graph;
}

void Visualizer::onActionAddEdgeMode() {
    addingEdgeMode_ = true;
    placingNode_ = false;
    pendingEdgeFrom_ = -1;
    canvasMode_ = MstCanvasMode::Graph;
}

void Visualizer::onActionRemoveNode() {
    const int n = static_cast<int>(adjacencyMatrix_.size());
    if (n <= 1) {
        return;
    }

    int removeId = selectedNodeId_;
    if (removeId < 0 || removeId >= n) {
        removeId = n - 1;
    }

    adjacencyMatrix_.erase(adjacencyMatrix_.begin() + removeId);
    for (auto& row : adjacencyMatrix_) {
        row.erase(row.begin() + removeId);
    }

    selectedNodeId_ = -1;
    rebuildGraphFromAdjacencyMatrix();
    animation_.clear();
    playing_ = false;
    state_ = AppState::Paused;
    timelineDirty_ = true;
}

void Visualizer::onActionKruskal() {
    algorithmType_ = algo::AlgorithmType::Kruskal;
    rebuildTimeline();
}

void Visualizer::onActionPrim() {
    algorithmType_ = algo::AlgorithmType::Prim;
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
    if (!font_.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
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
        Button("Sample", font_),
        Button("Random", font_),
        Button("Algorithm: Kruskal", font_),
        Button("Build", font_),
        Button("Add Node", font_),
        Button("Remove Node", font_),
        Button("View: Graph", font_),
        Button("Add Edge", font_)};

    backButton_ = Button("Back To Menu", font_);
    backButton_.setSize(170.f, 30.f);
    backButton_.setPosition(14.f, 42.f);

    // Row 1: transport controls
    controlButtons_[0].setSize(140.f, 30.f);
    controlButtons_[0].setPosition(200.f, 42.f);
    controlButtons_[1].setSize(140.f, 30.f);
    controlButtons_[1].setPosition(350.f, 42.f);
    controlButtons_[2].setSize(140.f, 30.f);
    controlButtons_[2].setPosition(500.f, 42.f);
    controlButtons_[3].setSize(140.f, 30.f);
    controlButtons_[3].setPosition(650.f, 42.f);
    controlButtons_[4].setSize(140.f, 30.f);
    controlButtons_[4].setPosition(800.f, 42.f);

    // Row 2: graph/data actions
    controlButtons_[5].setSize(130.f, 30.f);
    controlButtons_[5].setPosition(200.f, 82.f);
    controlButtons_[6].setSize(130.f, 30.f);
    controlButtons_[6].setPosition(340.f, 82.f);
    controlButtons_[7].setSize(180.f, 30.f);
    controlButtons_[7].setPosition(480.f, 82.f);
    controlButtons_[8].setSize(120.f, 30.f);
    controlButtons_[8].setPosition(670.f, 82.f);
    controlButtons_[9].setSize(140.f, 30.f);
    controlButtons_[9].setPosition(800.f, 82.f);
    controlButtons_[10].setSize(170.f, 30.f);
    controlButtons_[10].setPosition(950.f, 82.f);
    controlButtons_[11].setSize(130.f, 30.f);
    controlButtons_[11].setPosition(1128.f, 82.f);
    controlButtons_[12].setSize(130.f, 30.f);
    controlButtons_[12].setPosition(200.f, 122.f);

    speedSlider_ = Slider(14.f, 170.f, 180.f, 1.f, 10.f, 4.f, font_);

    sf::RenderWindow window(sf::VideoMode(config::kWindowWidth, config::kWindowHeight),
                            "Data Structure Visualization");
    window.setFramerateLimit(120);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

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
                                case 5: onActionSample(); break;
                                case 6: onActionRandom(); break;
                                case 7: onActionToggleAlgorithm(); break;
                                case 8: onActionBuild(); break;
                                case 9: onActionAddNode(); break;
                                case 10: onActionRemoveNode(); break;
                                case 11:
                                    canvasMode_ = (canvasMode_ == MstCanvasMode::Graph) ? MstCanvasMode::Matrix
                                                                                         : MstCanvasMode::Graph;
                                    break;
                                case 12: onActionAddEdgeMode(); break;
                                default: break;
                            }
                            consumed = true;
                            break;
                        }

                        if (!consumed && canvasMode_ == MstCanvasMode::Matrix && handleAdjacencyMatrixClick(mousePos)) {
                            consumed = true;
                        }

                        if (!consumed && canvasMode_ == MstCanvasMode::Graph) {
                            if (placingNode_ && isInsideGraphViewport(mousePos)) {
                                const sf::Vector2f npos = screenToGraphNormalized(mousePos);
                                const int n = static_cast<int>(adjacencyMatrix_.size());
                                for (auto& row : adjacencyMatrix_) {
                                    row.push_back(0);
                                }
                                adjacencyMatrix_.push_back(std::vector<int>(static_cast<size_t>(n + 1), 0));

                                if (n > 0) {
                                    const int anchor = selectedNodeId_ >= 0 && selectedNodeId_ < n ? selectedNodeId_ : (n - 1);
                                    const int w = autoWeight(anchor, n);
                                    adjacencyMatrix_[static_cast<size_t>(anchor)][static_cast<size_t>(n)] = w;
                                    adjacencyMatrix_[static_cast<size_t>(n)][static_cast<size_t>(anchor)] = w;
                                }

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

                        if (!consumed && canvasMode_ == MstCanvasMode::Graph) {
                            const int hitNode = hitTestNode(mousePos);
                            if (addingEdgeMode_) {
                                if (hitNode >= 0) {
                                    if (pendingEdgeFrom_ < 0) {
                                        pendingEdgeFrom_ = hitNode;
                                        selectedNodeId_ = hitNode;
                                    } else if (pendingEdgeFrom_ != hitNode) {
                                        const int n = static_cast<int>(adjacencyMatrix_.size());
                                        if (pendingEdgeFrom_ < n && hitNode < n) {
                                            const int w = autoWeight(pendingEdgeFrom_, hitNode);
                                            adjacencyMatrix_[static_cast<size_t>(pendingEdgeFrom_)][static_cast<size_t>(hitNode)] = w;
                                            adjacencyMatrix_[static_cast<size_t>(hitNode)][static_cast<size_t>(pendingEdgeFrom_)] = w;
                                            rebuildGraphFromAdjacencyMatrix();
                                            animation_.clear();
                                            playing_ = false;
                                            state_ = AppState::Paused;
                                            timelineDirty_ = true;
                                        }
                                        pendingEdgeFrom_ = -1;
                                        addingEdgeMode_ = false;
                                    }
                                }
                                consumed = true;
                            } else {
                                selectedNodeId_ = hitNode;
                            }
                        }
                    }
                }
            }

            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                speedSlider_.setActive(false);
            }

            if (event.type == sf::Event::MouseMoved) {
                if (currentScreen_ == Screen::Visualization && isMstSelected()) {
                    speedSlider_.onMouseMoved(sf::Vector2f(static_cast<float>(event.mouseMove.x),
                                                           static_cast<float>(event.mouseMove.y)));
                    speed_ = speedSlider_.getValue();
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
            const bool canRemoveSelected = n > 1 && selectedNodeId_ >= 0 && selectedNodeId_ < n;

            controlButtons_[0].setEnabled(!timelineDirty_ && hasTimeline && !atStart);          // Skip Back
            controlButtons_[1].setEnabled(!timelineDirty_ && hasTimeline && !atStart && !playing_); // Step Back
            controlButtons_[2].setEnabled(!timelineDirty_ && hasTimeline);                      // Play/Pause
            controlButtons_[3].setEnabled(!timelineDirty_ && hasTimeline && !atEnd && !playing_);    // Step Forward
            controlButtons_[4].setEnabled(!timelineDirty_ && hasTimeline && !atEnd);            // Skip Forward
            controlButtons_[2].setLabel(playing_ ? "Pause" : "Play");
            controlButtons_[7].setLabel(algorithmType_ == algo::AlgorithmType::Kruskal ? "Algorithm: Kruskal"
                                                                                         : "Algorithm: Prim");
            controlButtons_[9].setEnabled(!playing_);
            controlButtons_[10].setEnabled(!playing_ && canRemoveSelected);
            controlButtons_[11].setEnabled(!playing_);
            controlButtons_[11].setLabel(canvasMode_ == MstCanvasMode::Graph ? "View: Matrix" : "View: Graph");
            controlButtons_[12].setEnabled(!playing_ && canvasMode_ == MstCanvasMode::Graph);
            controlButtons_[7].setSelected(true);
            controlButtons_[11].setSelected(true);
            controlButtons_[8].setSelected(timelineDirty_);
            controlButtons_[8].setLabel(timelineDirty_ ? "Build *" : "Build");
            controlButtons_[12].setSelected(addingEdgeMode_);
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
            topStatus.setPosition(14.f, 12.f);
            topStatus.setFillColor(sf::Color(55, 55, 60));
            window.draw(topStatus);

            backButton_.draw(window);

            RenderViewModel vm;
            vm.kind = selectedStructure_;
            vm.graph = &graph_;
            if (step != nullptr) {
                vm.highlightedEdges = step->highlightedEdges;
                vm.candidateEdges = step->candidateEdges;
                vm.highlightedNodes = step->highlightedNodes;
            }
            if (!isMstSelected() || canvasMode_ == MstCanvasMode::Graph) {
                Renderer::draw(window, vm, font_);
            }

            if (isMstSelected()) {
                for (auto& b : controlButtons_) {
                    b.draw(window);
                }
                speedSlider_.draw(window);

                sf::Text status("Step: " + std::to_string(animation_.currentIndex()) + " / " +
                                    std::to_string(std::max(0, animation_.totalSteps() - 1)),
                                font_,
                                13);
                status.setPosition(14.f, 168.f);
                status.setFillColor(sf::Color::White);
                window.draw(status);

                if (timelineDirty_) {
                    sf::Text dirtyHint("Graph changed. Press Build to regenerate steps.", font_, 12);
                    dirtyHint.setPosition(14.f, 210.f);
                    dirtyHint.setFillColor(sf::Color(255, 221, 92));
                    window.draw(dirtyHint);
                }

                sf::Text selectedNodeText("Selected Node: " +
                                              std::string(selectedNodeId_ >= 0 ? std::to_string(selectedNodeId_) : "None"),
                                          font_,
                                          13);
                selectedNodeText.setPosition(14.f, 232.f);
                selectedNodeText.setFillColor(sf::Color(220, 220, 220));
                window.draw(selectedNodeText);

                if (placingNode_) {
                    sf::Text placeHint("Add Node mode: click graph area to place new node", font_, 12);
                    placeHint.setPosition(14.f, 254.f);
                    placeHint.setFillColor(sf::Color(255, 221, 92));
                    window.draw(placeHint);
                }

                if (addingEdgeMode_) {
                    std::string edgeHint = "Add Edge mode: click two nodes";
                    if (pendingEdgeFrom_ >= 0) {
                        edgeHint += " (from " + std::to_string(pendingEdgeFrom_) + ")";
                    }
                    sf::Text edgeHintText(edgeHint, font_, 12);
                    edgeHintText.setPosition(14.f, 274.f);
                    edgeHintText.setFillColor(sf::Color(255, 221, 92));
                    window.draw(edgeHintText);
                }

                const int n = static_cast<int>(adjacencyMatrix_.size());
                const float matrixCell = matrixCellForSize(n);
                const sf::Vector2f matrixOrigin = matrixOriginForSize(n);

                if (canvasMode_ == MstCanvasMode::Matrix) {
                    sf::Text matrixHelp("Adjacency Matrix: click a cell to toggle edge", font_, 12);
                    matrixHelp.setPosition(matrixOrigin.x, matrixOrigin.y - 24.f);
                    matrixHelp.setFillColor(sf::Color(210, 210, 210));
                    window.draw(matrixHelp);

                    for (int r = -1; r < n; ++r) {
                        for (int c = -1; c < n; ++c) {
                            sf::RectangleShape cell(sf::Vector2f(matrixCell - 1.0f, matrixCell - 1.0f));
                            cell.setPosition(matrixOrigin.x + (static_cast<float>(c + 1) * matrixCell),
                                             matrixOrigin.y + (static_cast<float>(r + 1) * matrixCell));

                            if (r == -1 || c == -1) {
                                cell.setFillColor(sf::Color(55, 60, 72));
                            } else if (r == c) {
                                cell.setFillColor(sf::Color(38, 42, 50));
                            } else {
                                cell.setFillColor(adjacencyMatrix_[static_cast<size_t>(r)][static_cast<size_t>(c)] > 0
                                                      ? sf::Color(50, 95, 55)
                                                      : sf::Color(36, 40, 48));
                            }
                            window.draw(cell);

                            sf::Text t("", font_, 11);
                            t.setFillColor(sf::Color(232, 232, 232));
                            if (r == -1 && c >= 0) {
                                t.setString(std::to_string(c));
                            } else if (c == -1 && r >= 0) {
                                t.setString(std::to_string(r));
                            } else if (r >= 0 && c >= 0) {
                                t.setString(std::to_string(adjacencyMatrix_[static_cast<size_t>(r)][static_cast<size_t>(c)]));
                            }
                            t.setPosition(cell.getPosition().x + 8.f, cell.getPosition().y + 5.f);
                            window.draw(t);
                        }
                    }
                }

                if (step != nullptr) {
                    sf::Text desc(step->description, font_, 13);
                    desc.setPosition(14.f, 190.f);
                    desc.setFillColor(sf::Color(220, 220, 220));
                    window.draw(desc);

                    sf::Text pseudoTitle("Pseudocode", font_, 14);
                    pseudoTitle.setPosition(canvasMode_ == MstCanvasMode::Matrix ? 900.f : 960.f, 168.f);
                    pseudoTitle.setFillColor(sf::Color(235, 235, 235));
                    window.draw(pseudoTitle);

                    for (size_t i = 0; i < pseudocode_.size(); ++i) {
                        sf::Text line(pseudocode_[i], font_, 13);
                        line.setPosition(canvasMode_ == MstCanvasMode::Matrix ? 900.f : 960.f,
                                         196.f + static_cast<float>(i) * 22.f);
                        const int lineNumber = static_cast<int>(i + 1);
                        bool highlighted =
                            std::find(step->pseudocodeLines.begin(), step->pseudocodeLines.end(), lineNumber) !=
                            step->pseudocodeLines.end();
                        line.setFillColor(highlighted ? sf::Color(255, 215, 0) : sf::Color(185, 185, 190));
                        window.draw(line);
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
