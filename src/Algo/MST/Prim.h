#pragma once
#include "../../config/Common.h"
#include "../Graph/Graph.h"

class Prim {
public:
    static std::vector<Edge> run(const Graph& g, int startNode = 0);
};