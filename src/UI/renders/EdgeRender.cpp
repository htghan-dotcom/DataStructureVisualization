#include "EdgeRender.h"

#include "../../config/Config.h"

namespace {
constexpr float kGraphMinX = 230.0f;
constexpr float kGraphMaxX = 840.0f;
constexpr float kGraphMinY = 220.0f;
constexpr float kGraphMaxY = 680.0f;

sf::Vector2f graphToScreen(float nx, float ny) {
    return sf::Vector2f(kGraphMinX + nx * (kGraphMaxX - kGraphMinX),
                        kGraphMinY + ny * (kGraphMaxY - kGraphMinY));
}

std::string edgeKey(const Edge& e) {
    const int a = std::min(e.from, e.to);
    const int b = std::max(e.from, e.to);
    return std::to_string(a) + "-" + std::to_string(b);
}

std::optional<Node> findNode(const Graph& graph, int id) {
    for (const auto& n : graph.getNodes()) {
        if (n.id == id) {
            return n;
        }
    }
    return std::nullopt;
}
}  // namespace

void EdgeRender::draw(sf::RenderWindow& window,
                      const Graph& graph,
                      const std::vector<Edge>& accepted,
                      const std::vector<Edge>& candidates,
                      const sf::Font& font) {
    (void)findNode; // Retained helper unused after fast-path optimization.

    std::unordered_map<std::string, int> acceptedMap;
    std::unordered_map<std::string, int> candidateMap;
    for (const auto& e : accepted) {
        acceptedMap[edgeKey(e)] = e.weight;
    }
    for (const auto& e : candidates) {
        candidateMap[edgeKey(e)] = e.weight;
    }

    // Build O(1) node lookup once per frame; avoids repeated linear scans per edge.
    std::unordered_map<int, Node> nodeById;
    nodeById.reserve(graph.getNodes().size());
    for (const auto& n : graph.getNodes()) {
        nodeById[n.id] = n;
    }

    const bool drawWeightText = graph.getEdges().size() <= 140;

    for (const auto& e : graph.getEdges()) {
        auto it1 = nodeById.find(e.from);
        auto it2 = nodeById.find(e.to);
        if (it1 == nodeById.end() || it2 == nodeById.end()) {
            continue;
        }

        const Node& n1 = it1->second;
        const Node& n2 = it2->second;

        sf::Vertex line[] = {
            sf::Vertex(graphToScreen(n1.x, n1.y)),
            sf::Vertex(graphToScreen(n2.x, n2.y))};

        const auto key = edgeKey(e);
        if (acceptedMap.count(key)) {
            line[0].color = config::kHighlightEdgeColor;
            line[1].color = config::kHighlightEdgeColor;
        } else if (candidateMap.count(key)) {
            line[0].color = config::kCandidateEdgeColor;
            line[1].color = config::kCandidateEdgeColor;
        } else {
            line[0].color = config::kEdgeColor;
            line[1].color = config::kEdgeColor;
        }

        window.draw(line, 2, sf::Lines);

        if (drawWeightText) {
            sf::Text weightText(std::to_string(e.weight), font, 12);
            weightText.setFillColor(sf::Color(215, 215, 220));
            weightText.setPosition((line[0].position.x + line[1].position.x) * 0.5f,
                                   (line[0].position.y + line[1].position.y) * 0.5f);
            window.draw(weightText);
        }
    }
}
