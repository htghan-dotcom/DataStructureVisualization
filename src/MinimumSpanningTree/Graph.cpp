#include "MinimumSpanningTree/Graph.h"

#include <tuple>
#include <unordered_set>
#include <sstream>

namespace {
class DSU {
public:
    explicit DSU(int n) : parent_(n), rank_(n, 0) {
        for (int i = 0; i < n; ++i) {
            parent_[i] = i;
        }
    }

    int find(int i) {
        if (parent_[i] == i) {
            return i;
        }
        parent_[i] = find(parent_[i]);
        return parent_[i];
    }

    bool unite(int u, int v) {
        const int rootU = find(u);
        const int rootV = find(v);
        if (rootU == rootV) {
            return false;
        }

        if (rank_[rootU] < rank_[rootV]) {
            parent_[rootU] = rootV;
        } else if (rank_[rootU] > rank_[rootV]) {
            parent_[rootV] = rootU;
        } else {
            parent_[rootV] = rootU;
            ++rank_[rootU];
        }
        return true;
    }

private:
    std::vector<int> parent_;
    std::vector<int> rank_;
};

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
}  // namespace

void Graph::clear() {
    nodes_.clear();
    edges_.clear();
}

void Graph::addNode(int id, float x, float y) {
    nodes_.push_back(Node{id, x, y});
}

void Graph::addEdge(int from, int to, int weight) {
    edges_.push_back(Edge{from, to, weight});
}

bool Graph::setNodePosition(int id, float x, float y) {
    for (auto& node : nodes_) {
        if (node.id == id) {
            node.x = x;
            node.y = y;
            return true;
        }
    }
    return false;
}

const std::vector<Node>& Graph::getNodes() const {
    return nodes_;
}

const std::vector<Edge>& Graph::getEdges() const {
    return edges_;
}

void Graph::randomizeEdges(int maxWeight) {
    edges_.clear();

    const int nodeCount = static_cast<int>(nodes_.size());
    if (nodeCount <= 1) {
        return;
    }

    std::mt19937 rng(static_cast<unsigned int>(std::random_device{}()));
    std::uniform_int_distribution<int> weightDist(1, std::max(1, maxWeight));

    int maxId = 0;
    for (const auto& node : nodes_) {
        if (node.id > maxId) {
            maxId = node.id;
        }
    }
    DSU dsu(maxId + 1);

    // Create candidate edges and sort by distance
    std::vector<std::tuple<float, int, int>> candidates;
    for (int i = 0; i < nodeCount; ++i) {
        for (int j = i + 1; j < nodeCount; ++j) {
            const auto& n1 = nodes_[i];
            const auto& n2 = nodes_[j];
            candidates.emplace_back(distance2(n1.x, n1.y, n2.x, n2.y), n1.id, n2.id);
        }
    }
    std::sort(candidates.begin(), candidates.end());

    // Add edges to ensure connectivity
    int edgesAdded = 0;
    for (const auto& cand : candidates) {
        if (edgesAdded >= nodeCount - 1) break;
        if (dsu.unite(std::get<1>(cand), std::get<2>(cand))) {
            addEdge(std::get<1>(cand), std::get<2>(cand), weightDist(rng));
            edgesAdded++;
        }
    }

    // Add more random edges up to a target
    const int targetEdges = std::max(nodeCount - 1, static_cast<int>(std::round(1.3f * static_cast<float>(nodeCount))));
    std::shuffle(candidates.begin(), candidates.end(), rng);

    for (const auto& cand : candidates) {
        if (edges_.size() >= static_cast<size_t>(targetEdges)) break;

        bool exists = false;
        for (const auto& existing_edge : edges_) {
            if ((existing_edge.from == std::get<1>(cand) && existing_edge.to == std::get<2>(cand)) ||
                (existing_edge.from == std::get<2>(cand) && existing_edge.to == std::get<1>(cand))) {
                exists = true;
                break;
            }
        }
        if (!exists) {
            addEdge(std::get<1>(cand), std::get<2>(cand), weightDist(rng));
        }
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

    const int cols = std::max(2, static_cast<int>(std::ceil(std::sqrt(static_cast<float>(nodeCount)))));
    std::vector<std::pair<float, float>> pos(static_cast<size_t>(nodeCount));
    for (int i = 0; i < nodeCount; ++i) {
        const float x = 0.30f + 0.55f * static_cast<float>(i % cols) / static_cast<float>(cols - 1);
        const float y = 0.20f + 0.60f * static_cast<float>(i / cols) /
                                   static_cast<float>(std::max(1, (nodeCount - 1) / cols));
        pos[static_cast<size_t>(i)] = {x, y};
        graph.addNode(i, x, y);
    }

    std::vector<std::vector<bool>> connected(static_cast<size_t>(nodeCount),
                                             std::vector<bool>(static_cast<size_t>(nodeCount), false));

    for (int i = 1; i < nodeCount; ++i) {
        std::uniform_int_distribution<int> parentDist(0, i - 1);
        const int parent = parentDist(rng);
        graph.addEdge(parent, i, weightDist(rng));
        connected[static_cast<size_t>(parent)][static_cast<size_t>(i)] = true;
        connected[static_cast<size_t>(i)][static_cast<size_t>(parent)] = true;
    }

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

Graph GraphLoader::createNodesOnlyGraph(int nodeCount) {
    Graph graph;
    if (nodeCount <= 0) {
        return graph;
    }

    const int cols = std::max(2, static_cast<int>(std::ceil(std::sqrt(static_cast<float>(nodeCount)))));
    for (int i = 0; i < nodeCount; ++i) {
        const float x = 0.30f + 0.55f * static_cast<float>(i % cols) / static_cast<float>(cols - 1);
        const float y = 0.20f + 0.60f * static_cast<float>(i / cols) /
                                   static_cast<float>(std::max(1, (nodeCount - 1) / cols));
        graph.addNode(i, x, y);
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
            const int cols = std::max(2, static_cast<int>(std::ceil(std::sqrt(static_cast<float>(std::max(1, nodeCount))))));
            for (int i = 0; i < nodeCount; ++i) {
                const float x = 0.30f + 0.55f * static_cast<float>(i % cols) / static_cast<float>(cols - 1);
                const float y = 0.20f + 0.60f * static_cast<float>(i / cols) /
                                           static_cast<float>(std::max(1, (nodeCount - 1) / cols));
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
