
#include "Graph.h"

void Graph::addNode(int id,float x,float y){
    nodes.push_back(Node(id,x,y));
}

void Graph::addEdge(int from,int to,int weight){
    edges.push_back(Edge(from,to,weight));
}

const std::vector<Node>& Graph::getNodes() const {
    return nodes;
}

const std::vector<Edge>& Graph::getEdges() const {
    return edges;
}