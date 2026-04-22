#pragma once

#include <SFML/Graphics.hpp>

#include "../Algo/AlgorithmFactory.h"
#include "../Algo/Graph/Graph.h"
#include "../core/Animation.h"
#include "../core/AppState.h"
#include "components/Button.h"
#include "components/Slider.h"
#include "renders/Renderer.h"

class Visualizer {
public:
    void run();

private:
    enum class Screen {
        Menu,
        Visualization
    };

    enum class MstCanvasMode {
        Graph,
        Matrix
    };

    void setupDefaultGraph();
    void syncAdjacencyMatrixFromGraph();
    void rebuildGraphFromAdjacencyMatrix();
    void pushUndoState();
    void rebuildTimeline();
    void buildPseudocode();
    bool isInsideGraphViewport(const sf::Vector2f& mousePos) const;
    sf::Vector2f screenToGraphNormalized(const sf::Vector2f& mousePos) const;
    int hitTestNode(const sf::Vector2f& mousePos) const;
    bool handleAdjacencyMatrixClick(const sf::Vector2f& mousePos);
    void onSelectStructure(RenderViewKind kind);
    bool isMstSelected() const;

    void onActionSample();
    void onActionNewGraph();
    void onActionRandom();
    void onActionUndo();
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
    RenderViewKind selectedStructure_ = RenderViewKind::MST;
    algo::AlgorithmType algorithmType_ = algo::AlgorithmType::Kruskal;
    Animation animation_;
    Screen currentScreen_ = Screen::Menu;

    AppState state_ = AppState::Idle;
    PlaybackMode playbackMode_ = PlaybackMode::StepByStep;
    bool playing_ = false;
    float speed_ = 2.0f;

    std::vector<Button> menuButtons_;
    std::vector<Button> controlButtons_;
    Button backButton_;
    Slider speedSlider_;
    sf::Font font_;
    sf::Font monoFont_;
    sf::Clock playClock_;

    std::vector<std::string> pseudocode_;
    std::vector<std::vector<int>> adjacencyMatrix_;
    std::vector<bool> nodeAlive_;
    struct GraphSnapshot {
        Graph graph;
        std::vector<std::vector<int>> adjacency;
        std::vector<bool> alive;
        int selectedNodeId = -1;
    };
    std::vector<GraphSnapshot> undoStack_;
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
    bool showLeftActions_ = true;
    bool showRightStepPanel_ = true;
    bool showRightPseudocodePanel_ = true;
};

