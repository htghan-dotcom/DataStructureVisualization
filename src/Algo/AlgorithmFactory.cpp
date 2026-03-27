#include "AlgorithmFactory.h"

#include "MST/Krustal.h"
#include "MST/Prim.h"

namespace algo {
std::vector<Step> AlgorithmFactory::buildSteps(AlgorithmType type, const Graph& graph, int startNode) {
    switch (type) {
        case AlgorithmType::Kruskal:
            return Krustal::buildSteps(graph);
        case AlgorithmType::Prim:
            return Prim::buildSteps(graph, startNode);
        default:
            return {};
    }
}

std::vector<Edge> AlgorithmFactory::run(AlgorithmType type, const Graph& graph) {
    const auto steps = buildSteps(type, graph, 0);
    if (steps.empty()) {
        return {};
    }
    return steps.back().highlightedEdges;
}
}  // namespace algo
