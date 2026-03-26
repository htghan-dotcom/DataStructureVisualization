#include "Graph.h"

void Graph::clear() {
    nodes_.clear();
    edges_.clear();
}

void Graph::addNode(int id, float x, float y) {
    nodes_.push_back(Node{id, x, y});
}

void Graph::addEdge(int from, int to, int weight) {
    edges_.push_back(Edge{from, to, weight});
}

bool Graph::setNodePosition(int id, float x, float y) {
    for (auto& node : nodes_) {
        if (node.id == id) {
            node.x = x;
            node.y = y;
            return true;
        }
    }
    return false;
}

const std::vector<Node>& Graph::getNodes() const {
    return nodes_;
}

const std::vector<Edge>& Graph::getEdges() const {
    return edges_;
}
