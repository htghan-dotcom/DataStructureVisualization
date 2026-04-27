#pragma once

#include "../Algo/Graph/Graph.h"
#include "../config/Common.h"

enum class InitSource {
    Empty,
    Random,
    File,
    Manual
};

class GraphLoader {
public:
    static Graph createEmptyGraph();
    static Graph createRandomGraph(int nodeCount, int maxWeight);
    static std::optional<Graph> loadFromFile(const std::string& filePath);
    static std::optional<Graph> loadFromManualLines(const std::vector<std::string>& lines);
};
