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
    std::vector<Edge> highlightedEdges;
    std::vector<Edge> candidateEdges;
    std::vector<int> highlightedNodes;
};

class Renderer {
public:
    static void draw(sf::RenderWindow& window, const RenderViewModel& vm, const sf::Font& font);
};
