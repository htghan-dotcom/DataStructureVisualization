#pragma once
#include "../../config/Common.h"
#include"Node.h"
#include"Edge.h"
class Graph{
    private:
    std::vector<Node> nodes;
    std::vector<Edge> edges;
    public:
    Graph()=default;
    void addNode(int id,float x=0,float y=0);
    void addEdge(int from,int to,int weight);
    const std::vector<Node>& getNodes() const;
    const std::vector<Edge>& getEdges() const;

};

