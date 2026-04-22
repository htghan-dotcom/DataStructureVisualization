#include "EdgeRender.h"

#include "../../config/Config.h"

namespace {
constexpr float kGraphMinX = 274.0f;
constexpr float kGraphMaxX = 934.0f;
constexpr float kGraphMinY = 110.0f;
constexpr float kGraphMaxY = 560.0f;

sf::Vector2f graphToScreen(float nx, float ny) {
    return sf::Vector2f(kGraphMinX + nx * (kGraphMaxX - kGraphMinX),
                        kGraphMinY + ny * (kGraphMaxY - kGraphMinY));
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
    seg.setOrigin(0.f, thickness * 0.5f);
    seg.setPosition(a);
    seg.setRotation(std::atan2(d.y, d.x) * 180.0f / 3.14159265f);
    seg.setFillColor(color);
    window.draw(seg);
}

}  // namespace

void EdgeRender::draw(sf::RenderWindow& window,
                      const Graph& graph,
                      const std::vector<Edge>& accepted,
                      const std::vector<Edge>& candidates,
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
        sf::FloatRect expanded(rect.left - 3.f, rect.top - 2.f, rect.width + 6.f, rect.height + 4.f);
        for (const auto& used : occupiedWeightBounds) {
            if (expanded.intersects(used)) {
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

        const sf::Vector2f p0 = graphToScreen(n1.x, n1.y);
        const sf::Vector2f p2 = graphToScreen(n2.x, n2.y);
        const sf::Vector2f mid((p0.x + p2.x) * 0.5f, (p0.y + p2.y) * 0.5f);
        const sf::Vector2f d(p2.x - p0.x, p2.y - p0.y);
        const float len = std::sqrt(d.x * d.x + d.y * d.y);
        const sf::Vector2f normal = (len > 0.01f) ? sf::Vector2f(-d.y / len, d.x / len) : sf::Vector2f(0.f, -1.f);

        const auto key = edgeKey(e);
        const bool isAccepted = acceptedMap.count(key) > 0;
        const bool isCandidate = candidateMap.count(key) > 0;

        sf::Color edgeColor = config::kEdgeColor;
        if (isAccepted) {
            edgeColor = config::kHighlightEdgeColor;
        } else if (isCandidate) {
            edgeColor = config::kCandidateEdgeColor;
        }

        sf::Vector2f centerPoint = mid;
        sf::Vector2f labelNormal = normal;
        drawThickSegment(window, p0, p2, 2.8f, edgeColor);

        sf::Text weightText(std::to_string(e.weight), font, 12);
        weightText.setFillColor(sf::Color(220, 220, 226));

        bool placed = false;
        for (int k = 0; k < 6; ++k) {
            const float offset = 7.0f + static_cast<float>(k) * 5.0f;
            const float dir = (k % 2 == 0) ? 1.0f : -1.0f;
            const sf::Vector2f pos(centerPoint.x + labelNormal.x * offset * dir,
                                   centerPoint.y + labelNormal.y * offset * dir);
            weightText.setPosition(pos.x, pos.y);
            const sf::FloatRect bounds = weightText.getGlobalBounds();
            if (!intersectsExisting(bounds)) {
                occupiedWeightBounds.push_back(
                    sf::FloatRect(bounds.left - 2.f, bounds.top - 2.f, bounds.width + 4.f, bounds.height + 4.f));
                window.draw(weightText);
                placed = true;
                break;
            }
        }

        if (!placed) {
            weightText.setPosition(centerPoint.x, centerPoint.y);
            window.draw(weightText);
        }
    }
}
