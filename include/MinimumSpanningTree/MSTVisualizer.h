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

// Dynamic layout grouping: manages positioning of button rows
struct ButtonGroup {
    float startY = 0.f;          // Y position where group starts
    float endY = 0.f;            // Y position after all items in group
    float itemHeight = 32.f;     // Height of each item in group
    float itemGap = 6.f;         // Vertical gap between items
    float rowItemGap = 8.f;      // Horizontal gap between items in same row
    int itemCount = 0;           // Number of items to layout
    
    // Calculate total height: itemCount * itemHeight + (itemCount - 1) * itemGap
    float calculateHeight() const {
        if (itemCount <= 0) return 0.f;
        return itemCount * itemHeight + (itemCount - 1) * itemGap;
    }
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

    struct UndoSnapshot {
        Graph graph;
        std::vector<std::vector<int>> adjacencyMatrix;
        std::vector<bool> nodeAlive;
        int selectedNodeId = -1;
        bool autoNodeMode = true;
        int manualNodeCount = 8;
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
    void onActionUndo();
    MSTAnimation animation_;
    void pushUndoState();

    // Dynamic offset layout system - single unified function
    void updateLayout();  // Master layout function using dynamic offset principle
    MSTAppState state_ = MSTAppState::Idle;
    Graph graph_;
    PlaybackMode playbackMode_ = PlaybackMode::StepByStep;
    bool playing_ = false;
    float speed_ = 2.0f;

    AppLayout layout_;
    
    // Algorithm state tracking
    algo::AlgorithmType algorithmType_ = algo::AlgorithmType::Kruskal;

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
    bool autoNodeMode_ = false;
    int manualNodeCount_ = 6;  // Node count for non-auto mode
    bool enteringNodeCount_ = false;
    std::string nodeCountInput_;
    bool showNewMenu_ = false;
    MstCanvasMode canvasMode_ = MstCanvasMode::Graph;
    bool algorithmPanelExpanded_ = true;
    bool controlPanelExpanded_ = true;  // Controls visibility of all control buttons
    std::vector<UndoSnapshot> undoStack_;
    bool timelineDirty_ = false;

    // Layout parameters for dynamic repositioning
    float layoutPanelX_ = 32.f;
    float layoutBtnHeight_ = 32.f;
    float layoutBtnRadius_ = 16.f;
    float layoutPanelWidth_ = 262.f;
    float layoutRowGap_ = 6.f;
    float layoutBtnGap_ = 8.f;
    float layoutGroupMarginTop_ = 20.f;
    float layoutGroupMarginBottom_ = 12.f;
    float layoutSystemY_ = 128.f;
    
    // Group titles header sizes and spacing
    float layoutGroupTitleSize_ = 14.f;
    float layoutGroupTitleMarginBottom_ = 8.f;
    float layoutSidebarGap_ = 15.f;  // Gap between major groups in sidebar
    
    // Footer section positioning
    float layoutFooterY_ = 0.f;  // Dynamic: kBottomPanelY - offset
    float layoutSystemTitleY_ = 0.f;
    float layoutActionsTitleY_ = 0.f;
    float layoutAlgorithmsTitleY_ = 0.f;
};