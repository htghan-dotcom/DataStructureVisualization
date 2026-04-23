#pragma once

#include <SFML/Graphics.hpp>
#include <sstream>
#include <iomanip>

#include "../Algo/AlgorithmFactory.h"
#include "../Algo/Graph/Graph.h"
#include "../core/Animation.h"
#include "../core/AppState.h"
#include "components/Button.h"
#include "components/Slider.h"
#include "renders/Renderer.h"

class SpeedSlider {
private:
    sf::RectangleShape mTrack;
    sf::RectangleShape mFill;
    sf::CircleShape mThumb;
    sf::Text mLabelText;
    sf::Text mValText;
    float mMinVal, mMaxVal, mCurrentVal;
    bool mIsDragging = false;
public:
    SpeedSlider() = default;
    SpeedSlider(float x, float y, float w, float min, float max, float current, const sf::Font& font)
        : mMinVal(min), mMaxVal(max), mCurrentVal(current) {
        mLabelText.setFont(font);
        mLabelText.setString("Speed:");
        mLabelText.setCharacterSize(14);
        mLabelText.setFillColor(sf::Color::White);
        sf::FloatRect labelBounds = mLabelText.getLocalBounds();
        mLabelText.setOrigin(sf::Vector2f(0.f, labelBounds.position.y + labelBounds.size.y / 2.0f));
        
        mValText.setFont(font);
        mValText.setCharacterSize(14);
        mValText.setFillColor(sf::Color::White);
        
        mTrack.setSize(sf::Vector2f(w, 6.f));
        mTrack.setOrigin(sf::Vector2f(0.f, 3.f));
        mTrack.setFillColor(sf::Color(100, 110, 125));
        
        mFill.setOrigin(sf::Vector2f(0.f, 3.f));
        mFill.setFillColor(sf::Color(132, 216, 68));
        
        mThumb.setRadius(8.f);
        mThumb.setOrigin(sf::Vector2f(8.f, 8.f));
        mThumb.setFillColor(sf::Color(90, 150, 44));
        
        setPosition(x, y);
    }
    void setPosition(float x, float y) {
        mLabelText.setPosition(sf::Vector2f(x, y));
        mValText.setPosition(sf::Vector2f(x + 55.f, y));
        mTrack.setPosition(sf::Vector2f(x + 95.f, y));
        mFill.setPosition(sf::Vector2f(x + 95.f, y));
        updateThumbPos();
    }
    void updateThumbPos() {
        float percent = (mCurrentVal - mMinVal) / (mMaxVal - mMinVal);
        float thumbX = mTrack.getPosition().x + percent * mTrack.getSize().x;
        mThumb.setPosition(sf::Vector2f(thumbX, mTrack.getPosition().y));
        mFill.setSize(sf::Vector2f(percent * mTrack.getSize().x, 6.f));
        
        std::stringstream stream;
        stream << std::fixed << std::setprecision(1) << mCurrentVal << "x";
        mValText.setString(stream.str());
        sf::FloatRect valBounds = mValText.getLocalBounds();
        mValText.setOrigin(sf::Vector2f(0.f, valBounds.position.y + valBounds.size.y / 2.0f));
    }
    bool contains(const sf::Vector2f& mousePos) const {
        sf::FloatRect trackBounds(mTrack.getPosition().x - 10.f, mTrack.getPosition().y - 15.f, mTrack.getSize().x + 20.f, 30.f);
        return trackBounds.contains(mousePos);
    }
    void setActive(bool active) { mIsDragging = active; }
    void onMouseMoved(const sf::Vector2f& mousePos) {
        if (mIsDragging) {
            float newX = std::max(mTrack.getPosition().x, std::min(mousePos.x, mTrack.getPosition().x + mTrack.getSize().x));
            float percent = (newX - mTrack.getPosition().x) / mTrack.getSize().x;
            mCurrentVal = mMinVal + percent * (mMaxVal - mMinVal);
            updateThumbPos();
        }
    }
    float getValue() const { return mCurrentVal; }
    void draw(sf::RenderTarget& target) const {
        target.draw(mLabelText);
        target.draw(mValText);
        target.draw(mTrack);
        target.draw(mFill);
        target.draw(mThumb);
    }
};

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
    SpeedSlider speedSlider_;
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

    float taskBarAnimProgress_ = 0.0f;

    float zoomLevel_ = 1.0f;
    sf::Vector2f panOffset_{0.0f, 0.0f};
    bool panning_ = false;
    sf::Vector2f lastMousePos_{0.0f, 0.0f};
};
