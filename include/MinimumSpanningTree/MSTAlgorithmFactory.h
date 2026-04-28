#pragma once

#include "Graph.h"
#include "Common.h"

enum class StructureKind {
    MST,
    DoublyLinkedList,
    HashTable,
    RedBlackTree
};

enum class StepEvent {
    None,
    Candidate,
    Accept,
    Reject,
    Visit,
    Compare,
    Rotate,
    Recolor,
    Insert,
    Delete,
    Update,
    Search,
    Complete
};

struct MSTStep {
    int index = 0;
    StructureKind structure = StructureKind::MST;
    StepEvent event = StepEvent::None;
    std::string description;
    std::vector<Edge> highlightedEdges;
    std::vector<Edge> candidateEdges;
    std::vector<int> highlightedNodes;
    std::vector<int> pseudocodeLines;
    std::unordered_map<std::string, std::string> metadata;
};

class MSTAnimation {
public:
    void clear();
    void setSteps(const std::vector<MSTStep>& steps);
    void addStep(const MSTStep& step);

    bool empty() const;
    int totalSteps() const;
    int currentIndex() const;

    const MSTStep* currentStep() const;
    const MSTStep* stepAt(int index) const;

    bool moveNext();
    bool movePrev();
    void moveToStart();
    void moveToEnd();
    void setCursor(int index);

private:
    std::vector<MSTStep> steps_;
    int cursor_ = 0;
};

namespace algo {
enum class AlgorithmType {
    Kruskal,
    Prim
};

class AlgorithmFactory {
public:
    static std::vector<MSTStep> buildSteps(AlgorithmType type, const Graph& graph, int startNode = 0);

    // Backward-compatible output for legacy rendering paths.
    static std::vector<Edge> run(AlgorithmType type, const Graph& graph);
};
}  // namespace algo
