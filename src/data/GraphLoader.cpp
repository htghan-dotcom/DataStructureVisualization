#include "GraphLoader.h"

Graph GraphLoader::createEmptyGraph() {
    return Graph{};
}

Graph GraphLoader::createRandomGraph(int nodeCount, int maxWeight) {
    Graph graph;
    if (nodeCount <= 0) {
        return graph;
    }

    std::mt19937 rng(static_cast<unsigned int>(std::random_device{}()));
    std::uniform_int_distribution<int> weightDist(1, std::max(1, maxWeight));

    int cols = std::max(2, static_cast<int>(std::ceil(std::sqrt(static_cast<float>(nodeCount)))));
    for (int i = 0; i < nodeCount; ++i) {
        float x = 0.30f + 0.55f * static_cast<float>(i % cols) / static_cast<float>(cols - 1);
        float y = 0.20f + 0.60f * static_cast<float>(i / cols) / static_cast<float>(std::max(1, (nodeCount - 1) / cols));
        graph.addNode(i, x, y);
    }

    for (int i = 1; i < nodeCount; ++i) {
        graph.addEdge(i - 1, i, weightDist(rng));
    }

    for (int i = 0; i < nodeCount; ++i) {
        for (int j = i + 2; j < nodeCount; ++j) {
            if ((rng() % 4) == 0) {
                graph.addEdge(i, j, weightDist(rng));
            }
        }
    }

    return graph;
}

std::optional<Graph> GraphLoader::loadFromFile(const std::string& filePath) {
    std::ifstream in(filePath);
    if (!in.is_open()) {
        return std::nullopt;
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(in, line)) {
        if (!line.empty()) {
            lines.push_back(line);
        }
    }
    return loadFromManualLines(lines);
}

std::optional<Graph> GraphLoader::loadFromManualLines(const std::vector<std::string>& lines) {
    Graph graph;

    // Format:
    // N <count>
    // E <u> <v> <w>
    int nodeCount = 0;
    bool nodeHeaderSeen = false;

    for (const auto& line : lines) {
        std::stringstream ss(line);
        std::string tag;
        ss >> tag;
        if (tag == "N") {
            ss >> nodeCount;
            if (nodeCount < 0) {
                return std::nullopt;
            }
            nodeHeaderSeen = true;
            int cols = std::max(2, static_cast<int>(std::ceil(std::sqrt(static_cast<float>(std::max(1, nodeCount))))));
            for (int i = 0; i < nodeCount; ++i) {
                float x = 0.30f + 0.55f * static_cast<float>(i % cols) / static_cast<float>(cols - 1);
                float y = 0.20f + 0.60f * static_cast<float>(i / cols) / static_cast<float>(std::max(1, (nodeCount - 1) / cols));
                graph.addNode(i, x, y);
            }
        } else if (tag == "E") {
            int u = 0;
            int v = 0;
            int w = 0;
            ss >> u >> v >> w;
            if (!nodeHeaderSeen || u < 0 || v < 0 || u >= nodeCount || v >= nodeCount) {
                return std::nullopt;
            }
            graph.addEdge(u, v, w);
        }
    }

    if (!nodeHeaderSeen) {
        return std::nullopt;
    }

    return graph;
}
