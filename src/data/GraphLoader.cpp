#include "GraphLoader.h"

namespace {
float orient(float ax, float ay, float bx, float by, float cx, float cy) {
    return (bx - ax) * (cy - ay) - (by - ay) * (cx - ax);
}

bool segmentsCross(float ax,
                   float ay,
                   float bx,
                   float by,
                   float cx,
                   float cy,
                   float dx,
                   float dy) {
    const float o1 = orient(ax, ay, bx, by, cx, cy);
    const float o2 = orient(ax, ay, bx, by, dx, dy);
    const float o3 = orient(cx, cy, dx, dy, ax, ay);
    const float o4 = orient(cx, cy, dx, dy, bx, by);
    return (o1 * o2 < 0.0f) && (o3 * o4 < 0.0f);
}

float distance2(float ax, float ay, float bx, float by) {
    const float dx = ax - bx;
    const float dy = ay - by;
    return dx * dx + dy * dy;
}
}

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
    std::vector<std::pair<float, float>> pos(static_cast<size_t>(nodeCount));
    for (int i = 0; i < nodeCount; ++i) {
        float x = 0.30f + 0.55f * static_cast<float>(i % cols) / static_cast<float>(cols - 1);
        float y = 0.20f + 0.60f * static_cast<float>(i / cols) / static_cast<float>(std::max(1, (nodeCount - 1) / cols));
        pos[static_cast<size_t>(i)] = {x, y};
        graph.addNode(i, x, y);
    }

    std::vector<std::vector<bool>> connected(static_cast<size_t>(nodeCount),
                                             std::vector<bool>(static_cast<size_t>(nodeCount), false));

    // Build a random spanning tree first so the graph is always connected.
    for (int i = 1; i < nodeCount; ++i) {
        std::uniform_int_distribution<int> parentDist(0, i - 1);
        const int parent = parentDist(rng);
        graph.addEdge(parent, i, weightDist(rng));
        connected[static_cast<size_t>(parent)][static_cast<size_t>(i)] = true;
        connected[static_cast<size_t>(i)][static_cast<size_t>(parent)] = true;
    }

    // Keep random graphs readable: target around 1.3*n total edges.
    const int minEdges = nodeCount - 1;
    const int targetEdges = std::max(minEdges, static_cast<int>(std::round(1.3f * static_cast<float>(nodeCount))));

    std::vector<std::tuple<float, int, int>> candidates;
    candidates.reserve(static_cast<size_t>(nodeCount * std::max(0, nodeCount - 1) / 2));
    for (int i = 0; i < nodeCount; ++i) {
        for (int j = i + 1; j < nodeCount; ++j) {
            if (!connected[static_cast<size_t>(i)][static_cast<size_t>(j)]) {
                const auto [x1, y1] = pos[static_cast<size_t>(i)];
                const auto [x2, y2] = pos[static_cast<size_t>(j)];
                candidates.push_back({distance2(x1, y1, x2, y2), i, j});
            }
        }
    }

    std::sort(candidates.begin(), candidates.end(),
              [](const auto& a, const auto& b) { return std::get<0>(a) < std::get<0>(b); });

    int edgeCount = minEdges;
    std::vector<std::pair<int, int>> acceptedEdges;
    acceptedEdges.reserve(static_cast<size_t>(targetEdges));
    for (int i = 1; i < nodeCount; ++i) {
        // spanning edges from parentDist build
        // infer accepted edges from connectivity matrix to check crossing for extras
        for (int j = 0; j < i; ++j) {
            if (connected[static_cast<size_t>(i)][static_cast<size_t>(j)]) {
                acceptedEdges.push_back({j, i});
                break;
            }
        }
    }

    for (const auto& [dist, u, v] : candidates) {
        (void)dist;
        if (edgeCount >= targetEdges) {
            break;
        }

        bool crosses = false;
        const auto [ux, uy] = pos[static_cast<size_t>(u)];
        const auto [vx, vy] = pos[static_cast<size_t>(v)];
        for (const auto& [a, b] : acceptedEdges) {
            if (a == u || a == v || b == u || b == v) {
                continue;
            }
            const auto [ax, ay] = pos[static_cast<size_t>(a)];
            const auto [bx, by] = pos[static_cast<size_t>(b)];
            if (segmentsCross(ux, uy, vx, vy, ax, ay, bx, by)) {
                crosses = true;
                break;
            }
        }

        if (crosses) {
            continue;
        }

        graph.addEdge(u, v, weightDist(rng));
        acceptedEdges.push_back({u, v});
        ++edgeCount;
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
