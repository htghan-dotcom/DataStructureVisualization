#pragma once
#include <SFML/Graphics.hpp>
#include "MinimumSpanningTree/MSTAlgorithmFactory.h"
#include "MinimumSpanningTree/Graph.h"
#include "GUI.h"
#include "AppLayout.h"
#include "Common.h"
#include "ThemeManager.h"


enum class MSTAppState {
    Idle,
    Animating,
    Paused,
    Finished,
    Error
};

enum class PlaybackMode {
    StepByStep,
    RunAtOnce
};

enum class RenderViewKind {
    MST
};

struct RenderViewModel {
    RenderViewKind kind = RenderViewKind::MST;
    const Graph* graph = nullptr;
    std::vector<Edge> highlightedEdges;
    std::vector<Edge> candidateEdges;
    std::vector<int> highlightedNodes;
    int selectedNodeId = -1;
};

class Renderer {
public:
    static void draw(sf::RenderWindow& window, const RenderViewModel& vm, const sf::Font& font);
};

class MSTVisualizer {
public:
    void init();
    void update(sf::Vector2i mousePos = sf::Vector2i(0,0));
    void draw(sf::RenderWindow& window, bool showUI = true);
    void processEvent(const sf::Event& event, sf::RenderWindow& window);

    bool checkReturnHome() const { return layout_.mGoHome; }
    void resetReturnHome()       { layout_.mGoHome = false; }

private:
    enum class MstCanvasMode {
        Graph,
        Matrix
    };

    void setupDefaultGraph();
    void syncAdjacencyMatrixFromGraph();
    void rebuildGraphFromAdjacencyMatrix();
    void rebuildTimeline();
    void buildPseudocode();
    bool isInsideGraphViewport(const sf::Vector2f& mousePos) const;
    sf::Vector2f screenToGraphNormalized(const sf::Vector2f& mousePos) const;
    int hitTestNode(const sf::Vector2f& mousePos) const;
    bool handleAdjacencyMatrixClick(const sf::Vector2f& mousePos);
    void onActionNewGraph();
    void onActionRandom();
    void onActionLoadFromFile();
    void onActionAddNode();
    void onActionAddEdgeMode();
    void onActionRemoveNode();
    void onActionKruskal();
    void onActionPrim();
    void onActionToggleAlgorithm();
    void onActionBuild();
    void onActionPrev();
    void onActionNext();
    void onActionPlayPause();
    void onActionEnd();

    Graph graph_;
    algo::AlgorithmType algorithmType_ = algo::AlgorithmType::Kruskal;
    MSTAnimation animation_;

    MSTAppState state_ = MSTAppState::Idle;
    PlaybackMode playbackMode_ = PlaybackMode::StepByStep;
    bool playing_ = false;
    float speed_ = 2.0f;

    AppLayout layout_;

    std::optional<RoundedButton> btnNew_;
    std::optional<RoundedButton> btnUndo_;
    std::optional<RoundedButton> btnNewRandom_;
    std::optional<RoundedButton> btnNewLoadFile_;
    std::optional<RoundedButton> btnNodeMode_;
    std::optional<RoundedButton> btnResetView_;
    std::optional<RoundedButton> btnAddNode_;
    std::optional<RoundedButton> btnAddEdge_;
    std::optional<RoundedButton> btnDelete_;
    std::optional<RoundedButton> btnKruskal_;
    std::optional<RoundedButton> btnPrim_;
    std::optional<RoundedButton> btnAlgoToggle_;
    std::optional<RoundedButton> btnMatrix_;
    std::optional<RoundedButton> btnBuild_;

    sf::Font font_;
    sf::Font monoFont_;
    sf::Clock playClock_;

    std::vector<std::string> pseudocode_;
    std::vector<std::vector<int>> adjacencyMatrix_;
    std::vector<bool> nodeAlive_;
    int selectedNodeId_ = -1;
    bool placingNode_ = false;
    bool deletingNodeMode_ = false;
    bool addingEdgeMode_ = false;
    int pendingEdgeFrom_ = -1;
    int pendingEdgeTo_ = -1;
    bool enteringEdgeWeight_ = false;
    std::string edgeWeightInput_;
    bool draggingNode_ = false;
    int draggingNodeId_ = -1;
    float graphZoom_ = 1.0f;
    sf::Vector2f graphPan_ = sf::Vector2f(0.f, 0.f);
    bool panningGraph_ = false;
    sf::Vector2f lastPanMouse_ = sf::Vector2f(0.f, 0.f);
    bool autoNodeMode_ = true;
    bool showNewMenu_ = false;
    MstCanvasMode canvasMode_ = MstCanvasMode::Graph;
    bool timelineDirty_ = false;
};