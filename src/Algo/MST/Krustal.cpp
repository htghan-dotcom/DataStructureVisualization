#include "Krustal.h"

std::vector<Edge> Kruskal::run(const Graph& g) {
    std::vector<Edge> resultMST;
    std::vector<Edge> edges = g.getEdges(); 
    
    std::sort(edges.begin(), edges.end());

    int numNodes = g.getNodes().size();
    if (numNodes == 0) return resultMST;
    DSU dsu(numNodes);

    for (const auto& edge : edges) {
        if (dsu.unite(edge.from, edge.to)) {
            resultMST.push_back(edge);
        }
        if (resultMST.size() == numNodes - 1) {
            break;
        }
    }

    return resultMST;
}