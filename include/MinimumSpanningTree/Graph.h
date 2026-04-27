#pragma once

#include "../Common.h"

struct Node {
    int id = 0;
    float x = 0.f;
    float y = 0.f;
};

struct Edge {
    int from = 0;
    int to = 0;
    int weight = 0;

    bool operator<(const Edge& other) const {
        if (weight != other.weight) {
            return weight < other.weight;
        }
        if (from != other.from) {
            return from < other.from;
        }
        return to < other.to;
    }
};

class Graph {
public:
    void clear();
    void addNode(int id, float x, float y);
    void addEdge(int from, int to, int weight);
    bool setNodePosition(int id, float x, float y);

    const std::vector<Node>& getNodes() const;
    const std::vector<Edge>& getEdges() const;

private:
    std::vector<Node> nodes_;
    std::vector<Edge> edges_;
};

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
