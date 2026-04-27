#pragma once

#include "../../Algo/Graph/Graph.h"
#include "../../core/Step.h"
#include "../../config/Common.h"

class Krustal {
public:
    static std::vector<Step> buildSteps(const Graph& graph);
};
