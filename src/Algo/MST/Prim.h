#pragma once

#include "../../Algo/Graph/Graph.h"
#include "../../core/Step.h"
#include "../../config/Common.h"

class Prim {
public:
    static std::vector<Step> buildSteps(const Graph& graph, int startNode = 0);
};
