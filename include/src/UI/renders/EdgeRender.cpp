#include "EdgeRender.h"

#include "../../config/Config.h"

#include <functional>

namespace {
sf::Vector2f graphToScreen(float nx, float ny, const sf::FloatRect& graphViewport) {
    return sf::Vector2f(graphViewport.position.x + nx * graphViewport.size.x,
                        graphViewport.position.y + ny * graphViewport.size.y);
}

std::string edgeKey(const Edge& e) {
    const int a = std::min(e.from, e.to);
    const int b = std::max(e.from, e.to);
    return std::to_string(a) + "-" + std::to_string(b);
}

void drawThickSegment(sf::RenderWindow& window,
                      const sf::Vector2f& a,
                      const sf::Vector2f& b,
                      float thickness,
                      const sf::Color& color) {
    const sf::Vector2f d(b.x - a.x, b.y - a.y);
    const float len = std::sqrt(d.x * d.x + d.y * d.y);
    if (len <= 0.01f) {
        return;
    }

    sf::RectangleShape seg(sf::Vector2f(len, thickness));
    seg.setOrigin({0.f, thickness * 0.5f});
    // Snap to half-pixel to reduce shimmering and keep lines visually crisp.
    seg.setPosition({std::round(a.x) + 0.5f, std::round(a.y) + 0.5f});
    seg.setRotation(sf::degrees(std::atan2(d.y, d.x) * 180.0f / 3.14159265f));
    seg.setFillColor(color);
    window.draw(seg);
}

void drawWeight(sf::RenderWindow& window, const sf::Font& font, const sf::Vector2f& center, int weight) {
    sf::Text weightText(font, std::to_string(weight), 12);
    weightText.setFillColor(sf::Color(22, 28, 38));
    weightText.setOutlineColor(sf::Color(252, 252, 252)); // Bo viền trắng (Halo) quanh chữ
    weightText.setOutlineThickness(2.5f);
    const sf::FloatRect b = weightText.getLocalBounds();
    weightText.setPosition({std::round(center.x - (b.size.x * 0.5f) - b.position.x),
                           std::round(center.y - (b.size.y * 0.5f) - b.position.y)});
    window.draw(weightText);
}

}  // namespace

void EdgeRender::draw(sf::RenderWindow& window,
                      const Graph& graph,
                      const sf::FloatRect& graphViewport,
                      const std::vector<Edge>& accepted,
                      const std::vector<Edge>& candidates,
                      StepEvent currentStepEvent,
                      const sf::Font& font) {
    std::unordered_map<std::string, int> acceptedMap;
    std::unordered_map<std::string, int> candidateMap;
    for (const auto& e : accepted) {
        acceptedMap[edgeKey(e)] = e.weight;
    }
    for (const auto& e : candidates) {
        candidateMap[edgeKey(e)] = e.weight;
    }

    std::unordered_map<int, Node> nodeById;
    nodeById.reserve(graph.getNodes().size());
    for (const auto& n : graph.getNodes()) {
        nodeById[n.id] = n;
    }

    std::vector<sf::FloatRect> occupiedWeightBounds;
    occupiedWeightBounds.reserve(graph.getEdges().size() + 8);

    auto intersectsExisting = [&occupiedWeightBounds](const sf::FloatRect& rect) {
        sf::FloatRect expanded({rect.position.x - 8.f, rect.position.y - 6.f}, {rect.size.x + 16.f, rect.size.y + 12.f});
        for (const auto& used : occupiedWeightBounds) {
            if (expanded.findIntersection(used).has_value()) {
                return true;
            }
        }
        return false;
    };

    for (const auto& e : graph.getEdges()) {
        auto it1 = nodeById.find(e.from);
        auto it2 = nodeById.find(e.to);
        if (it1 == nodeById.end() || it2 == nodeById.end()) {
            continue;
        }

        const Node& n1 = it1->second;
        const Node& n2 = it2->second;

        const sf::Vector2f p0 = graphToScreen(n1.x, n1.y, graphViewport);
        const sf::Vector2f p2 = graphToScreen(n2.x, n2.y, graphViewport);
        const sf::Vector2f mid((p0.x + p2.x) * 0.5f, (p0.y + p2.y) * 0.5f);
        const sf::Vector2f d(p2.x - p0.x, p2.y - p0.y);
        const float len = std::sqrt(d.x * d.x + d.y * d.y);
        const sf::Vector2f normal = (len > 0.01f) ? sf::Vector2f(-d.y / len, d.x / len) : sf::Vector2f(0.f, -1.f);

        const auto key = edgeKey(e);
        const bool isAccepted = acceptedMap.count(key) > 0;
        const bool isCandidate = candidateMap.count(key) > 0;

        sf::Color edgeColor = config::kEdgeColor;
        float edgeThickness = 4.0f;
        if (isAccepted) {
            edgeColor = config::kHighlightEdgeColor;
            edgeThickness = 6.5f;
        } else if (isCandidate) {
            if (currentStepEvent == StepEvent::Reject) {
                edgeColor = config::kRejectedEdgeColor;
                edgeThickness = 3.5f;
            } else {
                edgeColor = config::kCandidateEdgeColor;
                edgeThickness = 5.5f;
            }
        }

        sf::Vector2f centerPoint = mid;
        sf::Vector2f labelNormal = normal;
        const sf::Vector2f tangent = (len > 0.01f) ? sf::Vector2f(d.x / len, d.y / len) : sf::Vector2f(1.0f, 0.0f);
        const float normalDir = ((std::hash<std::string>{}(key) & 1u) == 0u) ? 1.0f : -1.0f;
        drawThickSegment(window, p0, p2, edgeThickness, edgeColor);

        bool placed = false;
        for (int k = 0; k < 8; ++k) { // Tăng số vòng quét tìm vị trí trống
            const float offset = 14.0f + static_cast<float>(k) * 5.0f; // Bắt đầu xa tâm hơn
            const float tangentShift = (k == 0) ? 0.0f : ((k % 2 == 0) ? 14.0f : -14.0f) * (1.0f + (k / 2) * 0.4f);
            const sf::Vector2f pos(centerPoint.x + labelNormal.x * offset * normalDir + tangent.x * tangentShift,
                                   centerPoint.y + labelNormal.y * offset * normalDir + tangent.y * tangentShift);
            const sf::FloatRect bounds({pos.x - 12.0f, pos.y - 10.0f}, {24.0f, 20.0f});
            if (!intersectsExisting(bounds)) {
                occupiedWeightBounds.push_back(
                    sf::FloatRect({bounds.position.x - 2.f, bounds.position.y - 2.f}, {bounds.size.x + 4.f, bounds.size.y + 4.f}));
                drawWeight(window, font, pos, e.weight);
                placed = true;
                break;
            }
        }

        if (!placed) {
            drawWeight(window, font, centerPoint, e.weight);
        }
    }
}
