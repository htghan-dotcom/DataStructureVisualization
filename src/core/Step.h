#pragma once

#include "../Algo/Graph/Edge.h"
#include "../config/Common.h"

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

struct Step {
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
