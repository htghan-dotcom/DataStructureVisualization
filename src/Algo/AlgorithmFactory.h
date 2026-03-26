#pragma once

#include "../core/Step.h"
#include "Graph/Graph.h"
#include "../config/Common.h"

namespace algo {
enum class AlgorithmType {
    Kruskal,
    Prim
};

class AlgorithmFactory {
public:
    static std::vector<Step> buildSteps(AlgorithmType type, const Graph& graph);

    // Backward-compatible output for legacy rendering paths.
    static std::vector<Edge> run(AlgorithmType type, const Graph& graph);
};
}  // namespace algo
