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

sf::Vector2f bezierPoint(const sf::Vector2f& p0, const sf::Vector2f& p1, const sf::Vector2f& p2, float t) {
    const float u = 1.0f - t;
    return sf::Vector2f(u * u * p0.x + 2.0f * u * t * p1.x + t * t * p2.x,
                        u * u * p0.y + 2.0f * u * t * p1.y + t * t * p2.y);
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

float distancePointToSegment(const sf::Vector2f& p, const sf::Vector2f& a, const sf::Vector2f& b, float* outT) {
    const sf::Vector2f ab(b.x - a.x, b.y - a.y);
    const float ab2 = ab.x * ab.x + ab.y * ab.y;
    if (ab2 <= 0.0001f) {
        if (outT != nullptr) {
            *outT = 0.0f;
        }
        const float dx = p.x - a.x;
        const float dy = p.y - a.y;
        return std::sqrt(dx * dx + dy * dy);
    }

    float t = ((p.x - a.x) * ab.x + (p.y - a.y) * ab.y) / ab2;
    t = std::clamp(t, 0.0f, 1.0f);
    if (outT != nullptr) {
        *outT = t;
    }

    const sf::Vector2f c(a.x + t * ab.x, a.y + t * ab.y);
    const float dx = p.x - c.x;
    const float dy = p.y - c.y;
    return std::sqrt(dx * dx + dy * dy);
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

        bool shouldCurve = false;
        const float clearance = config::kNodeRadius * 1.15f;
        for (const auto& [id, node] : nodeById) {
            if (id == e.from || id == e.to) {
                continue;
            }
            float tOnEdge = 0.0f;
            const float dist = distancePointToSegment(graphToScreen(node.x, node.y), p0, p2, &tOnEdge);
            if (dist < clearance && tOnEdge > 0.12f && tOnEdge < 0.88f) {
                shouldCurve = true;
                break;
            }
        }

        const int a = std::min(e.from, e.to);
        const int b = std::max(e.from, e.to);
        const float sign = ((a + b) % 2 == 0) ? 1.0f : -1.0f;
        const float curveStrength = std::clamp(len * 0.14f, 12.0f, 38.0f);
        const sf::Vector2f p1(mid.x + normal.x * curveStrength * sign, mid.y + normal.y * curveStrength * sign);

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

        if (shouldCurve) {
            sf::VertexArray curve(sf::LineStrip, 12);
            sf::Vector2f prev = p0;
            for (size_t i = 0; i < curve.getVertexCount(); ++i) {
                const float t = static_cast<float>(i) / static_cast<float>(curve.getVertexCount() - 1);
                curve[i].position = bezierPoint(p0, p1, p2, t);
                curve[i].color = edgeColor;

                if (i > 0) {
                    drawThickSegment(window, prev, curve[i].position, 2.8f, edgeColor);
                }
                prev = curve[i].position;
            }

            centerPoint = bezierPoint(p0, p1, p2, 0.5f);
            const sf::Vector2f tangent(2.0f * (p1.x - p0.x) + 2.0f * 0.5f * (p2.x - 2.0f * p1.x + p0.x),
                                       2.0f * (p1.y - p0.y) + 2.0f * 0.5f * (p2.y - 2.0f * p1.y + p0.y));
            const float tLen = std::sqrt(tangent.x * tangent.x + tangent.y * tangent.y);
            labelNormal = (tLen > 0.01f) ? sf::Vector2f(-tangent.y / tLen, tangent.x / tLen)
                                         : sf::Vector2f(0.f, -1.f);
        } else {
            drawThickSegment(window, p0, p2, 2.8f, edgeColor);
        }

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

                sf::RectangleShape bg(sf::Vector2f(bounds.width + 6.f, bounds.height + 4.f));
                bg.setPosition(bounds.left - 3.f, bounds.top - 2.f);
                bg.setFillColor(sf::Color(12, 16, 24, 190));
                window.draw(bg);
                window.draw(weightText);
                placed = true;
                break;
            }
        }

        if (!placed) {
            weightText.setPosition(centerPoint.x, centerPoint.y);
            const sf::FloatRect bounds = weightText.getGlobalBounds();
            sf::RectangleShape bg(sf::Vector2f(bounds.width + 6.f, bounds.height + 4.f));
            bg.setPosition(bounds.left - 3.f, bounds.top - 2.f);
            bg.setFillColor(sf::Color(12, 16, 24, 190));
            window.draw(bg);
            window.draw(weightText);
        }
    }
}
