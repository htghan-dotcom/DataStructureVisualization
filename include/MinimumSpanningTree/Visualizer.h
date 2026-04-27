#pragma once

#include <SFML/Graphics.hpp>

#include "AlgorithmFactory.h"
#include "Graph.h"
#include "UI/components/Button.h"
#include "UI/components/Slider.h"

enum class AppState {
    Idle,
    Loading,
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
    MST,
    DoublyLinkedList,
    HashTable,
    RedBlackTree
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

class Visualizer {
public:
    void run();

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
    Animation animation_;

    AppState state_ = AppState::Idle;
    PlaybackMode playbackMode_ = PlaybackMode::StepByStep;
    bool playing_ = false;
    float speed_ = 2.0f;

    std::vector<Button> controlButtons_;
    Button backButton_;
    Slider speedSlider_;
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
    MstCanvasMode canvasMode_ = MstCanvasMode::Graph;
    bool timelineDirty_ = false;
};

