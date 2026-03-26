#pragma once

#include "Edge.h"
#include "Node.h"
#include "../../config/Common.h"

class Graph {
public:
    void clear();
    void addNode(int id, float x, float y);
    void addEdge(int from, int to, int weight);
    bool setNodePosition(int id, float x, float y);

    const std::vector<Node>& getNodes() const;
    const std::vector<Edge>& getEdges() const;

private:
    std::vector<Node> nodes_;
    std::vector<Edge> edges_;
};
