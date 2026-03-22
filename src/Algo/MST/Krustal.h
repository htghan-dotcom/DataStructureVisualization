#pragma once
#include "../../config//Common.h"
#include "../Graph/Graph.h"
#include "../Graph/Edge.h"
#include"DSU.h"

class Kruskal {
public:
    static std::vector<Edge> run(const Graph& g);
};