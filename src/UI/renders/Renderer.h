#pragma once

#include <SFML/Graphics.hpp>
#include "../../Algo/Graph/Graph.h"
#include "../../core/Step.h"

enum class RenderViewKind {
    MST,
    DoublyLinkedList,
    HashTable,
    RedBlackTree
};

struct RenderViewModel {
    RenderViewKind kind = RenderViewKind::MST;
    const Graph* graph = nullptr;
    sf::FloatRect graphViewport = sf::FloatRect(274.0f, 110.0f, 660.0f, 450.0f);
    std::vector<Edge> highlightedEdges;
    std::vector<Edge> candidateEdges;
    std::vector<int> highlightedNodes;
    StepEvent currentStepEvent = StepEvent::None;
    int selectedNodeId = -1;
};

class Renderer {
public:
    static void draw(sf::RenderWindow& window, const RenderViewModel& vm, const sf::Font& font);
};
