#include "MinimumSpanningTree/MSTVisualizer.h"
#include "tinyfiledialogs.h"


namespace config {
inline constexpr unsigned int kWindowWidth = 1280;
inline constexpr unsigned int kWindowHeight = 720;
inline constexpr float kNodeRadius = 22.0f;
inline const sf::Color kBackgroundColor(18, 20, 28);
inline const sf::Color kNodeFillColor(48, 140, 210);
inline const sf::Color kNodeOutlineColor(240, 240, 240);
inline const sf::Color kEdgeColor(120, 120, 130);
inline const sf::Color kHighlightEdgeColor(231, 76, 60);
inline const sf::Color kCandidateEdgeColor(241, 196, 15);
}  // namespace config

namespace {
int randomWeight() {
    static std::mt19937 rng(static_cast<unsigned int>(std::random_device{}()));
    static std::uniform_int_distribution<int> dist(1, 30);
    return dist(rng);
}

constexpr float kMatrixOriginX = 620.0f;
constexpr float kMatrixOriginY = 140.0f;
constexpr float kMatrixBaseCell = 34.0f;

constexpr float kLeftPanelX = 12.0f;
constexpr float kLeftPanelY = 12.0f;
constexpr float kLeftPanelW = 248.0f;
constexpr float kLeftPanelH = 560.0f;

constexpr float kRightPanelX = 948.0f;
constexpr float kRightPanelY = 12.0f;
constexpr float kRightPanelW = 320.0f;
constexpr float kRightPanelH = 560.0f;

constexpr float kBottomPanelX = 12.0f;
constexpr float kBottomPanelY = 628.0f;
constexpr float kBottomPanelW = 1256.0f;
constexpr float kBottomPanelH = 80.0f;

constexpr float kGraphMinX = 360.0f;
constexpr float kGraphMaxX = 1080.0f;
constexpr float kGraphMinY = 180.0f;
constexpr float kGraphMaxY = 740.0f;

sf::Vector2f gGraphPan(0.f, 0.f);
float gGraphZoom = 1.0f;

sf::Vector2f graphViewCenter() {
    return sf::Vector2f((kGraphMinX + kGraphMaxX) * 0.5f, (kGraphMinY + kGraphMaxY) * 0.5f);
}

sf::Vector2f graphToScreen(float nx, float ny) {
    const sf::Vector2f base(kGraphMinX + nx * (kGraphMaxX - kGraphMinX),
                            kGraphMinY + ny * (kGraphMaxY - kGraphMinY));
    const sf::Vector2f center = graphViewCenter();
    return sf::Vector2f(center.x + (base.x - center.x) * gGraphZoom + gGraphPan.x,
                        center.y + (base.y - center.y) * gGraphZoom + gGraphPan.y);
}

float matrixCellForSize(int n) {
    const float centerAreaW = kGraphMaxX - kGraphMinX - 20.f;
    const float centerAreaH = kGraphMaxY - kGraphMinY - 60.f;
    const float maxCellW = centerAreaW / static_cast<float>(n + 1);
    const float maxCellH = centerAreaH / static_cast<float>(n + 1);
    return std::min({kMatrixBaseCell, maxCellW, maxCellH});
}

sf::Vector2f matrixOriginForSize(int n) {
    const float cell = matrixCellForSize(n);
    const float tableW = cell * static_cast<float>(n + 1);
    const float tableH = cell * static_cast<float>(n + 1);
    const float centerAreaW = kGraphMaxX - kGraphMinX;
    const float centerAreaH = kGraphMaxY - kGraphMinY;
    const float x = kGraphMinX + (centerAreaW - tableW) * 0.5f;
    const float y = kGraphMinY + (centerAreaH - tableH) * 0.5f + 15.f;
    return sf::Vector2f(x, y);
}

void drawRoundedFill(sf::RenderWindow& window, const sf::FloatRect& rect, float radius, const sf::Color& color) {
    if (rect.size.x <= 0.f || rect.size.y <= 0.f) {
        return;
    }

    const float r = std::clamp(radius, 0.0f, std::min(rect.size.x, rect.size.y) * 0.5f);
    if (r <= 0.01f) {
        sf::RectangleShape box(rect.size);
        box.setPosition(rect.position);
        box.setFillColor(color);
        window.draw(box);
        return;
    }

    sf::RectangleShape center(sf::Vector2f(rect.size.x - 2.f * r, rect.size.y));
    center.setPosition({rect.position.x + r, rect.position.y});
    center.setFillColor(color);
    window.draw(center);

    sf::RectangleShape middle(sf::Vector2f(rect.size.x, rect.size.y - 2.f * r));
    middle.setPosition({rect.position.x, rect.position.y + r});
    middle.setFillColor(color);
    window.draw(middle);

    sf::CircleShape corner(r);
    corner.setFillColor(color);

    corner.setPosition(rect.position);
    window.draw(corner);

    corner.setPosition({rect.position.x + rect.size.x - 2.f * r, rect.position.y});
    window.draw(corner);

    corner.setPosition({rect.position.x, rect.position.y + rect.size.y - 2.f * r});
    window.draw(corner);

    corner.setPosition({rect.position.x + rect.size.x - 2.f * r, rect.position.y + rect.size.y - 2.f * r});
    window.draw(corner);
}

void drawRoundedBox(sf::RenderWindow& window,
                    const sf::FloatRect& rect,
                    float radius,
                    float outlineThickness,
                    const sf::Color& fill,
                    const sf::Color& outline) {
    drawRoundedFill(window, rect, radius, outline);

    if (outlineThickness <= 0.01f) {
        return;
    }

    const float inset = outlineThickness;
    const sf::FloatRect inner({rect.position.x + inset,
                               rect.position.y + inset},
                              {std::max(0.f, rect.size.x - inset * 2.f),
                               std::max(0.f, rect.size.y - inset * 2.f)});
    drawRoundedFill(window, inner, std::max(0.f, radius - inset), fill);
}

bool loadFontFromCandidates(sf::Font& font, const std::vector<std::string>& candidates) {
    for (const auto& path : candidates) {
        std::error_code ec;
        if (!std::filesystem::exists(path, ec) || ec) {
            continue;
        }
        if (font.openFromFile(path)) {
            return true;
        }
    }
    return false;
}

void clampTextToWidth(sf::Text& text, float maxWidth) {
    const std::string original = text.getString().toAnsiString();
    if (original.empty()) {
        return;
    }

    if (text.getLocalBounds().size.x <= maxWidth) {
        return;
    }

    const std::string ellipsis = "...";
    std::string trimmed = original;
    text.setString(trimmed + ellipsis);
    while (!trimmed.empty() && text.getLocalBounds().size.x > maxWidth) {
        trimmed.pop_back();
        text.setString(trimmed + ellipsis);
    }

    if (trimmed.empty() && text.getLocalBounds().size.x > maxWidth) {
        text.setString(ellipsis);
    }
}

std::vector<std::string> wrapTextToWidth(const sf::Font& font,
                                         const std::string& content,
                                         unsigned int charSize,
                                         float maxWidth) {
    sf::Text measure(font, "", charSize);
    auto fits = [&](const std::string& s) {
        measure.setString(s);
        return measure.getLocalBounds().size.x <= maxWidth;
    };

    std::vector<std::string> lines;
    std::istringstream iss(content);
    std::string word;
    std::string current;

    auto flushCurrent = [&]() {
        if (!current.empty()) {
            lines.push_back(current);
            current.clear();
        }
    };

    auto appendLongWord = [&](const std::string& longWord) {
        std::string chunk;
        for (char ch : longWord) {
            const std::string candidate = chunk + ch;
            if (!chunk.empty() && !fits(candidate)) {
                lines.push_back(chunk);
                chunk = std::string(1, ch);
            } else {
                chunk = candidate;
            }
        }
        return chunk;
    };

    while (iss >> word) {
        if (current.empty()) {
            if (fits(word)) {
                current = word;
            } else {
                current = appendLongWord(word);
            }
            continue;
        }

        const std::string candidate = current + " " + word;
        if (fits(candidate)) {
            current = candidate;
        } else {
            flushCurrent();
            if (fits(word)) {
                current = word;
            } else {
                current = appendLongWord(word);
            }
        }
    }

    flushCurrent();
    if (lines.empty()) {
        lines.push_back("");
    }
    return lines;
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
    seg.setOrigin({0.f, thickness * 0.5f});
    seg.setPosition(a);
    seg.setRotation(sf::degrees(std::atan2(d.y, d.x) * 180.0f / 3.14159265f));
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

void drawNode(sf::RenderWindow& window,
              const Node& node,
              float radius,
              const sf::Font& font,
              bool isSelected,
              bool isActive) {
    sf::CircleShape circle(radius);
    circle.setOrigin({radius, radius});
    circle.setPosition(graphToScreen(node.x, node.y));

    if (isSelected) {
        circle.setFillColor(ThemeManager::current.primaryLight);
    } else if (isActive) {
        circle.setFillColor(ThemeManager::current.primary);
    } else {
        circle.setFillColor(ThemeManager::current.secondary);
    }

    circle.setOutlineColor(ThemeManager::current.textColor);
    circle.setOutlineThickness(2.4f);
    window.draw(circle);

    sf::Text idText(font, std::to_string(node.id), 16);
    idText.setFillColor(ThemeManager::current.textColor);
    idText.setPosition({circle.getPosition().x - 5.f, circle.getPosition().y - 9.f});
    window.draw(idText);
}

void drawEdges(sf::RenderWindow& window,
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
        sf::FloatRect expanded({rect.position.x - 3.f, rect.position.y - 2.f}, {rect.size.x + 6.f, rect.size.y + 4.f});
        for (const auto& used : occupiedWeightBounds) {
            if (expanded.findIntersection(used)) {
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

    sf::Color edgeColor = ThemeManager::current.textColor;
    edgeColor.a = 150;
        if (isAccepted) {
        edgeColor = ThemeManager::current.primary;
        } else if (isCandidate) {
        edgeColor = sf::Color(241, 196, 15);
        }

        sf::Vector2f centerPoint = mid;
        sf::Vector2f labelNormal = normal;

        if (shouldCurve) {
            sf::VertexArray curve(sf::PrimitiveType::LineStrip, 12);
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

        sf::Text weightText(font, std::to_string(e.weight), 20);
    weightText.setFillColor(ThemeManager::current.textColor);

        bool placed = false;
        for (int k = 0; k < 6; ++k) {
            const float offset = 7.0f + static_cast<float>(k) * 5.0f;
            const float dir = (k % 2 == 0) ? 1.0f : -1.0f;
            const sf::Vector2f pos(centerPoint.x + labelNormal.x * offset * dir,
                                   centerPoint.y + labelNormal.y * offset * dir);
            weightText.setPosition(pos);
            const sf::FloatRect bounds = weightText.getGlobalBounds();
            if (!intersectsExisting(bounds)) {
                occupiedWeightBounds.push_back(
                    sf::FloatRect({bounds.position.x - 2.f, bounds.position.y - 2.f}, {bounds.size.x + 4.f, bounds.size.y + 4.f}));

                sf::RectangleShape bg(sf::Vector2f(bounds.size.x + 6.f, bounds.size.y + 4.f));
                bg.setPosition({bounds.position.x - 3.f, bounds.position.y - 2.f});
        bg.setFillColor(ThemeManager::current.screenBg);
                window.draw(bg);
                window.draw(weightText);
                placed = true;
                break;
            }
        }

        if (!placed) {
            weightText.setPosition(centerPoint);
            const sf::FloatRect bounds = weightText.getGlobalBounds();
            sf::RectangleShape bg(sf::Vector2f(bounds.size.x + 6.f, bounds.size.y + 4.f));
            bg.setPosition({bounds.position.x - 3.f, bounds.position.y - 2.f});
            bg.setFillColor(ThemeManager::current.screenBg);
            window.draw(bg);
            window.draw(weightText);
        }
    }
}
}  // namespace

void Renderer::draw(sf::RenderWindow& window, const RenderViewModel& vm, const sf::Font& font) {
    if (vm.kind != RenderViewKind::MST) {
        sf::Text unsupported(font, "Structure renderer scaffolded (implementation pending)", 16);
        unsupported.setPosition({270.f, 60.f});
        unsupported.setFillColor(sf::Color(220, 220, 220));
        window.draw(unsupported);
        return;
    }

    if (vm.graph == nullptr) {
        return;
    }

    drawEdges(window, *vm.graph, vm.highlightedEdges, vm.candidateEdges, font);
    for (const auto& node : vm.graph->getNodes()) {
        const bool isActive = std::find(vm.highlightedNodes.begin(), vm.highlightedNodes.end(), node.id) !=
                              vm.highlightedNodes.end();
        const bool isSelected = node.id == vm.selectedNodeId;
        drawNode(window, node, config::kNodeRadius, font, isSelected, isActive);
    }
}

void MSTVisualizer::setupDefaultGraph() {
    graph_.clear();
    graph_.addNode(0, 0.30f, 0.35f);
    graph_.addNode(1, 0.50f, 0.22f);
    graph_.addNode(2, 0.72f, 0.34f);
    graph_.addNode(3, 0.66f, 0.58f);
    graph_.addNode(4, 0.42f, 0.66f);
    graph_.addNode(5, 0.23f, 0.54f);

    graph_.addEdge(0, 1, 4);
    graph_.addEdge(1, 2, 7);
    graph_.addEdge(2, 3, 3);
    graph_.addEdge(3, 4, 6);
    graph_.addEdge(4, 5, 5);
    graph_.addEdge(5, 0, 8);
    graph_.addEdge(0, 2, 9);
    graph_.addEdge(1, 4, 10);

    selectedNodeId_ = -1;
    syncAdjacencyMatrixFromGraph();
}

void MSTVisualizer::syncAdjacencyMatrixFromGraph() {
    int maxId = -1;
    for (const auto& node : graph_.getNodes()) {
        maxId = std::max(maxId, node.id);
    }

    const int n = maxId + 1;
    adjacencyMatrix_.assign(static_cast<size_t>(n), std::vector<int>(static_cast<size_t>(n), 0));
    nodeAlive_.assign(static_cast<size_t>(n), false);

    for (const auto& node : graph_.getNodes()) {
        if (node.id >= 0 && node.id < n) {
            nodeAlive_[static_cast<size_t>(node.id)] = true;
        }
    }

    for (const auto& edge : graph_.getEdges()) {
        if (edge.from >= 0 && edge.to >= 0 && edge.from < n && edge.to < n) {
            adjacencyMatrix_[static_cast<size_t>(edge.from)][static_cast<size_t>(edge.to)] = edge.weight;
            adjacencyMatrix_[static_cast<size_t>(edge.to)][static_cast<size_t>(edge.from)] = edge.weight;
        }
    }
}

void MSTVisualizer::rebuildGraphFromAdjacencyMatrix() {
    const int n = static_cast<int>(adjacencyMatrix_.size());
    if (static_cast<int>(nodeAlive_.size()) < n) {
        nodeAlive_.resize(static_cast<size_t>(n), true);
    }

    const auto oldNodes = graph_.getNodes();
    std::unordered_map<int, sf::Vector2f> oldPos;
    oldPos.reserve(oldNodes.size());
    for (const auto& node : oldNodes) {
        oldPos[node.id] = sf::Vector2f(node.x, node.y);
    }

    Graph rebuilt;
    for (int i = 0; i < n; ++i) {
        if (!nodeAlive_[static_cast<size_t>(i)]) {
            continue;
        }

        auto it = oldPos.find(i);
        if (it != oldPos.end()) {
            rebuilt.addNode(i, it->second.x, it->second.y);
        } else {
            const float angle = 0.7f * static_cast<float>(i);
            float x = 0.55f + 0.28f * std::cos(angle);
            float y = 0.58f + 0.28f * std::sin(angle);
            rebuilt.addNode(i, std::clamp(x, 0.18f, 0.92f), std::clamp(y, 0.22f, 0.90f));
        }
    }

    for (int i = 0; i < n; ++i) {
        if (!nodeAlive_[static_cast<size_t>(i)]) {
            continue;
        }
        for (int j = i + 1; j < n; ++j) {
            if (!nodeAlive_[static_cast<size_t>(j)]) {
                continue;
            }
            const int w = adjacencyMatrix_[static_cast<size_t>(i)][static_cast<size_t>(j)];
            if (w > 0) {
                rebuilt.addEdge(i, j, w);
            }
        }
    }

    graph_ = rebuilt;
}

int MSTVisualizer::hitTestNode(const sf::Vector2f& mousePos) const {
    const float radius = config::kNodeRadius;
    for (const auto& node : graph_.getNodes()) {
        const sf::Vector2f screenPos = graphToScreen(node.x, node.y);
        const float nx = screenPos.x;
        const float ny = screenPos.y;
        const float dx = mousePos.x - nx;
        const float dy = mousePos.y - ny;
        if ((dx * dx + dy * dy) <= (radius * radius)) {
            return node.id;
        }
    }
    return -1;
}

bool MSTVisualizer::handleAdjacencyMatrixClick(const sf::Vector2f& mousePos) {
    const int n = static_cast<int>(adjacencyMatrix_.size());
    if (n <= 0) {
        return false;
    }

    const float matrixCell = matrixCellForSize(n);
    const sf::Vector2f origin = matrixOriginForSize(n);
    const float tableW = matrixCell * static_cast<float>(n + 1);
    const float tableH = matrixCell * static_cast<float>(n + 1);
    if (mousePos.x < origin.x || mousePos.y < origin.y || mousePos.x > (origin.x + tableW) ||
        mousePos.y > (origin.y + tableH)) {
        return false;
    }

    const int col = static_cast<int>((mousePos.x - origin.x) / matrixCell) - 1;
    const int row = static_cast<int>((mousePos.y - origin.y) / matrixCell) - 1;
    if (row < 0 || col < 0 || row >= n || col >= n || row == col) {
        return false;
    }

    if (static_cast<int>(nodeAlive_.size()) < n) {
        nodeAlive_.resize(static_cast<size_t>(n), true);
    }
    if (!nodeAlive_[static_cast<size_t>(row)] || !nodeAlive_[static_cast<size_t>(col)]) {
        return false;
    }

    pendingEdgeFrom_ = row;
    pendingEdgeTo_ = col;
    enteringEdgeWeight_ = true;
    const int currentW = adjacencyMatrix_[static_cast<size_t>(row)][static_cast<size_t>(col)];
    edgeWeightInput_ = (currentW > 0) ? std::to_string(currentW) : "";
    addingEdgeMode_ = false;
    return true;
}

bool MSTVisualizer::isInsideGraphViewport(const sf::Vector2f& mousePos) const {
    return mousePos.x >= kGraphMinX && mousePos.x <= kGraphMaxX && mousePos.y >= kGraphMinY && mousePos.y <= kGraphMaxY;
}

sf::Vector2f MSTVisualizer::screenToGraphNormalized(const sf::Vector2f& mousePos) const {
    const sf::Vector2f center = graphViewCenter();
    const float invZoom = 1.0f / std::max(0.001f, gGraphZoom);
    const sf::Vector2f unviewed(center.x + (mousePos.x - center.x - gGraphPan.x) * invZoom,
                                center.y + (mousePos.y - center.y - gGraphPan.y) * invZoom);
    const float nx = (unviewed.x - kGraphMinX) / (kGraphMaxX - kGraphMinX);
    const float ny = (unviewed.y - kGraphMinY) / (kGraphMaxY - kGraphMinY);
    return sf::Vector2f(std::clamp(nx, 0.05f, 0.95f), std::clamp(ny, 0.05f, 0.95f));
}

void MSTVisualizer::buildPseudocode() {
    if (algorithmType_ == algo::AlgorithmType::Kruskal) {
        pseudocode_ = {
            "1. Sort edges by weight",
            "2. For each edge E in sorted edges",
            "3. Mark E as candidate",
            "4. If endpoints of E are in different sets",
            "5. Accept E and union sets",
            "6. Else reject E",
            "7. Done"};
    } else {
        pseudocode_ = {
            "1. Pick start node and push frontier edges",
            "2. While priority queue is not empty",
            "3. Pop minimum edge E",
            "4. If destination is already in tree, reject E",
            "5. Else accept E",
            "6. Add destination frontier edges",
            "7. Done"};
    }
}

void MSTVisualizer::onActionNewGraph() {
    showNewMenu_ = !showNewMenu_;
}

void MSTVisualizer::pushUndoState() {
    UndoSnapshot snap;
    snap.graph = graph_;
    snap.adjacencyMatrix = adjacencyMatrix_;
    snap.nodeAlive = nodeAlive_;
    snap.selectedNodeId = selectedNodeId_;
    snap.autoNodeMode = autoNodeMode_;
    snap.manualNodeCount = manualNodeCount_;

    undoStack_.push_back(std::move(snap));
    if (undoStack_.size() > 64) undoStack_.erase(undoStack_.begin());
}

void MSTVisualizer::onActionUndo() {
    if (undoStack_.empty()) return;
    const UndoSnapshot snap = undoStack_.back();
    undoStack_.pop_back();

    graph_ = snap.graph;
    adjacencyMatrix_ = snap.adjacencyMatrix;
    nodeAlive_ = snap.nodeAlive;
    selectedNodeId_ = snap.selectedNodeId;
    autoNodeMode_ = snap.autoNodeMode;
    manualNodeCount_ = snap.manualNodeCount;

    rebuildGraphFromAdjacencyMatrix();
    animation_.clear();
    playing_ = false;
    state_ = MSTAppState::Paused;
    timelineDirty_ = true;
}

void MSTVisualizer::onActionRandom() {
    pushUndoState();
    graph_.randomizeEdges(30);
    selectedNodeId_ = -1;
    placingNode_ = false;
    deletingNodeMode_ = false;
    addingEdgeMode_ = false;
    pendingEdgeFrom_ = -1;
    pendingEdgeTo_ = -1;
    enteringEdgeWeight_ = false;
    edgeWeightInput_.clear();
    syncAdjacencyMatrixFromGraph();
    rebuildTimeline();
    showNewMenu_ = false;
}

void MSTVisualizer::onActionLoadFromFile() {
    const char* filters[] = {"*.txt", "*.csv", "*.graph"};
    char* filePath = tinyfd_openFileDialog("Load Graph", "", 3, filters, "Graph files", 0);
    if (filePath == nullptr || *filePath == '\0') {
        return;
    }

    const std::optional<Graph> loaded = GraphLoader::loadFromFile(filePath);
    if (!loaded) {
        return;
    }

    pushUndoState();
    graph_ = *loaded;
    selectedNodeId_ = -1;
    placingNode_ = false;
    deletingNodeMode_ = false;
    addingEdgeMode_ = false;
    pendingEdgeFrom_ = -1;
    pendingEdgeTo_ = -1;
    enteringEdgeWeight_ = false;
    edgeWeightInput_.clear();
    syncAdjacencyMatrixFromGraph();
    rebuildTimeline();
}

void MSTVisualizer::onActionAddNode() {
    placingNode_ = !placingNode_;
    if (placingNode_) {
        deletingNodeMode_ = false;
        addingEdgeMode_ = false;
    }
}

void MSTVisualizer::onActionAddEdgeMode() {
    addingEdgeMode_ = !addingEdgeMode_;
    if (addingEdgeMode_) {
        placingNode_ = false;
        deletingNodeMode_ = false;
        pendingEdgeFrom_ = -1;
        pendingEdgeTo_ = -1;
        enteringEdgeWeight_ = false;
        edgeWeightInput_.clear();
    }
}

void MSTVisualizer::onActionRemoveNode() {
    deletingNodeMode_ = !deletingNodeMode_;
    if (deletingNodeMode_) {
        placingNode_ = false;
        addingEdgeMode_ = false;
    }
}

void MSTVisualizer::onActionKruskal() {
    algorithmType_ = algo::AlgorithmType::Kruskal;
    rebuildTimeline();
}

void MSTVisualizer::onActionPrim() {
    algorithmType_ = algo::AlgorithmType::Prim;
    rebuildTimeline();
}

void MSTVisualizer::onActionToggleAlgorithm() {
    controlPanelExpanded_ = !controlPanelExpanded_;
}

void MSTVisualizer::onActionBuild() {
    rebuildTimeline();
    playing_ = false;
    layout_.setPaused(true);
}

void MSTVisualizer::onActionPrev() {
    playing_ = false;
    layout_.setPaused(true);
    animation_.movePrev();
}

void MSTVisualizer::onActionNext() {
    playing_ = false;
    layout_.setPaused(true);
    animation_.moveNext();
}

void MSTVisualizer::onActionPlayPause() {
    if (animation_.empty()) {
        return;
    }

    playing_ = !playing_;
    layout_.setPaused(!playing_);
    if (playing_) {
        playClock_.restart();
    }
}

void MSTVisualizer::onActionEnd() {
    playing_ = false;
    layout_.setPaused(true);
    animation_.moveToEnd();
}

void MSTVisualizer::rebuildTimeline() {
    buildPseudocode();

    const int startNode = graph_.getNodes().empty() ? 0 : graph_.getNodes().front().id;
    animation_.setSteps(algo::AlgorithmFactory::buildSteps(algorithmType_, graph_, startNode));
    animation_.moveToStart();
    timelineDirty_ = false;
    state_ = animation_.empty() ? MSTAppState::Idle : MSTAppState::Paused;
    layout_.setPaused(true);
}

void MSTVisualizer::updateLayout() {
    layoutFooterY_ = kBottomPanelY - 18.f;

    if (!controlPanelExpanded_) {
        return;
    }

    const float titleHeight = 18.f;
    const float titleBottomGap = 10.f;
    const float groupGap = 20.f;
    const float buildBottomGap = 8.f;

    float currentY = layoutSystemY_;
    const float halfBtnWidth = (layoutPanelWidth_ - layoutBtnGap_) / 2.f;

    layoutSystemTitleY_ = currentY;
    currentY += titleHeight + titleBottomGap;

    if (btnNew_) btnNew_->setPosition(layoutPanelX_, currentY, halfBtnWidth, layoutBtnHeight_, layoutBtnRadius_);
    if (btnUndo_) btnUndo_->setPosition(layoutPanelX_ + halfBtnWidth + layoutBtnGap_, currentY, halfBtnWidth, layoutBtnHeight_, layoutBtnRadius_);
    currentY += layoutBtnHeight_ + layoutRowGap_;

    if (showNewMenu_) {
        if (btnNewRandom_) btnNewRandom_->setPosition(layoutPanelX_ + 4.f, currentY, layoutPanelWidth_ - 8.f, layoutBtnHeight_, layoutBtnRadius_);
        currentY += layoutBtnHeight_ + layoutRowGap_;
        if (btnNewLoadFile_) btnNewLoadFile_->setPosition(layoutPanelX_ + 4.f, currentY, layoutPanelWidth_ - 8.f, layoutBtnHeight_, layoutBtnRadius_);
        currentY += layoutBtnHeight_ + layoutRowGap_;
    }

    if (btnNodeMode_) btnNodeMode_->setPosition(layoutPanelX_, currentY, halfBtnWidth, layoutBtnHeight_, layoutBtnRadius_);
    if (btnMatrix_) btnMatrix_->setPosition(layoutPanelX_ + halfBtnWidth + layoutBtnGap_, currentY, halfBtnWidth, layoutBtnHeight_, layoutBtnRadius_);
    currentY += layoutBtnHeight_ + layoutRowGap_;

    if (btnResetView_) btnResetView_->setPosition(layoutPanelX_, currentY, layoutPanelWidth_, layoutBtnHeight_, layoutBtnRadius_);
    currentY += layoutBtnHeight_ + groupGap;

    layoutActionsTitleY_ = currentY;
    currentY += titleHeight + titleBottomGap;

    if (btnAddNode_) btnAddNode_->setPosition(layoutPanelX_, currentY, halfBtnWidth, layoutBtnHeight_, layoutBtnRadius_);
    if (btnDelete_) btnDelete_->setPosition(layoutPanelX_ + halfBtnWidth + layoutBtnGap_, currentY, halfBtnWidth, layoutBtnHeight_, layoutBtnRadius_);
    currentY += layoutBtnHeight_ + layoutRowGap_;

    if (btnAddEdge_) btnAddEdge_->setPosition(layoutPanelX_, currentY, layoutPanelWidth_, layoutBtnHeight_, layoutBtnRadius_);
    currentY += layoutBtnHeight_ + layoutRowGap_;

    if (btnBuild_) btnBuild_->setPosition(layoutPanelX_, currentY, layoutPanelWidth_, 38.f, layoutBtnRadius_);
    currentY += 38.f + buildBottomGap + groupGap;

    layoutAlgorithmsTitleY_ = currentY;
    currentY += titleHeight + titleBottomGap;

    if (algorithmPanelExpanded_) {
        if (btnKruskal_) btnKruskal_->setPosition(layoutPanelX_, currentY, halfBtnWidth, layoutBtnHeight_, layoutBtnRadius_);
        if (btnPrim_) btnPrim_->setPosition(layoutPanelX_ + halfBtnWidth + layoutBtnGap_, currentY, halfBtnWidth, layoutBtnHeight_, layoutBtnRadius_);
    }
}
 
void MSTVisualizer::init() {
    layout_.setPlayPauseCallback([this](){ onActionPlayPause(); });
    layout_.setSkipBackCallback([this](){ onActionPrev(); });
    layout_.setSkipForwardCallback([this](){ onActionNext(); });
    layout_.setStepBackCallback([this](){ onActionPrev(); });
    layout_.setStepForwardCallback([this](){ onActionNext(); });

    loadFontFromCandidates(font_, {
        "assets/fonts/Inter-Bold.ttf",
        "../assets/fonts/Inter-Bold.ttf"
    });
    loadFontFromCandidates(monoFont_, {
        "assets/fonts/JetBrainsMono-2.304/fonts/ttf/JetBrainsMono-Regular.ttf",
        "../assets/fonts/JetBrainsMono-2.304/fonts/ttf/JetBrainsMono-Regular.ttf",
        "assets/fonts/Inter-Regular.ttf",
        "../assets/fonts/Inter-Regular.ttf"
    });

    const float halfBtnWidth = (layoutPanelWidth_ - layoutBtnGap_) / 2.f;

    btnNew_.emplace(font_, "New Graph", layoutPanelX_, layoutSystemY_, halfBtnWidth, layoutBtnHeight_, layoutBtnRadius_, ThemeManager::current.secondary);
    btnUndo_.emplace(font_, "Undo", layoutPanelX_ + halfBtnWidth + layoutBtnGap_, layoutSystemY_, halfBtnWidth, layoutBtnHeight_, layoutBtnRadius_, ThemeManager::current.secondary);
    btnNewRandom_.emplace(font_, "Random", layoutPanelX_, layoutSystemY_, layoutPanelWidth_, layoutBtnHeight_, layoutBtnRadius_, ThemeManager::current.secondary);
    btnNewLoadFile_.emplace(font_, "Load From File", layoutPanelX_, layoutSystemY_, layoutPanelWidth_, layoutBtnHeight_, layoutBtnRadius_, ThemeManager::current.secondary);
    btnNodeMode_.emplace(font_, "Nodes: 6", layoutPanelX_, layoutSystemY_, halfBtnWidth, layoutBtnHeight_, layoutBtnRadius_, ThemeManager::current.secondary);
    btnMatrix_.emplace(font_, "View: Matrix", layoutPanelX_ + halfBtnWidth + layoutBtnGap_, layoutSystemY_, halfBtnWidth, layoutBtnHeight_, layoutBtnRadius_, ThemeManager::current.secondary);
    btnResetView_.emplace(font_, "Reset View", layoutPanelX_, layoutSystemY_, layoutPanelWidth_, layoutBtnHeight_, layoutBtnRadius_, ThemeManager::current.secondary);
    btnAddNode_.emplace(font_, "Add Node", layoutPanelX_, layoutSystemY_, halfBtnWidth, layoutBtnHeight_, layoutBtnRadius_, ThemeManager::current.secondary);
    btnDelete_.emplace(font_, "Delete Node", layoutPanelX_ + halfBtnWidth + layoutBtnGap_, layoutSystemY_, halfBtnWidth, layoutBtnHeight_, layoutBtnRadius_, ThemeManager::current.secondary);
    btnAddEdge_.emplace(font_, "Edit Edges", layoutPanelX_, layoutSystemY_, layoutPanelWidth_, layoutBtnHeight_, layoutBtnRadius_, ThemeManager::current.secondary);
    btnBuild_.emplace(font_, "BUILD", layoutPanelX_, layoutSystemY_, layoutPanelWidth_, 38.f, layoutBtnRadius_, ThemeManager::current.primary);
    btnKruskal_.emplace(font_, "Kruskal", layoutPanelX_, layoutSystemY_, halfBtnWidth, layoutBtnHeight_, layoutBtnRadius_, ThemeManager::current.secondary);
    btnPrim_.emplace(font_, "Prim", layoutPanelX_ + halfBtnWidth + layoutBtnGap_, layoutSystemY_, halfBtnWidth, layoutBtnHeight_, layoutBtnRadius_, ThemeManager::current.secondary);
    float toggleX_init = controlPanelExpanded_ ? (layoutPanelX_ + layoutPanelWidth_ + 16.f) : 12.f;
    float toggleY_init = kLeftPanelY + (kLeftPanelH - 44.f) / 2.f;
    btnAlgoToggle_.emplace(font_, controlPanelExpanded_ ? "<" : ">", toggleX_init, toggleY_init, 20.f, 44.f, 10.f, ThemeManager::current.secondary);

    btnNew_->setCallback([this](){ onActionNewGraph(); });
    btnUndo_->setCallback([this](){ onActionUndo(); });
    if (btnNewRandom_) btnNewRandom_->setCallback([this](){ pushUndoState(); onActionRandom(); showNewMenu_ = false; });
    if (btnNewLoadFile_) btnNewLoadFile_->setCallback([this](){ pushUndoState(); onActionLoadFromFile(); showNewMenu_ = false; });
    btnAddNode_->setCallback([this](){ onActionAddNode(); });
    btnAddEdge_->setCallback([this](){ onActionAddEdgeMode(); });
    btnDelete_->setCallback([this](){ onActionRemoveNode(); });
    btnKruskal_->setCallback([this](){ onActionKruskal(); });
    btnPrim_->setCallback([this](){ onActionPrim(); });
    btnAlgoToggle_->setCallback([this](){ onActionToggleAlgorithm(); });
    btnMatrix_->setCallback([this](){ canvasMode_ = (canvasMode_ == MstCanvasMode::Graph) ? MstCanvasMode::Matrix : MstCanvasMode::Graph; });
    btnResetView_->setCallback([this](){ graphZoom_ = 1.0f; graphPan_ = sf::Vector2f(0.f, 0.f); });
    btnBuild_->setCallback([this](){ onActionBuild(); });

    if (btnNew_) btnNew_->setCharacterSize(16);
    if (btnUndo_) btnUndo_->setCharacterSize(16);
    if (btnNewRandom_) btnNewRandom_->setCharacterSize(16);
    if (btnNewLoadFile_) btnNewLoadFile_->setCharacterSize(16);
    if (btnNodeMode_) btnNodeMode_->setCharacterSize(16);
    if (btnMatrix_) btnMatrix_->setCharacterSize(16);
    if (btnResetView_) btnResetView_->setCharacterSize(16);
    if (btnAddNode_) btnAddNode_->setCharacterSize(16);
    if (btnDelete_) btnDelete_->setCharacterSize(16);
    if (btnAddEdge_) btnAddEdge_->setCharacterSize(16);
    if (btnKruskal_) btnKruskal_->setCharacterSize(16);
    if (btnPrim_) btnPrim_->setCharacterSize(16);
    if (btnBuild_) btnBuild_->setCharacterSize(22);
    if (btnAlgoToggle_) btnAlgoToggle_->setCharacterSize(20);

    if (btnNodeMode_) {
        btnNodeMode_->setCallback([this](){
            enteringNodeCount_ = true;
            nodeCountInput_.clear();
        });
    }

    setupDefaultGraph();
    rebuildTimeline();
    updateLayout();  // Initialize button positions
}

void MSTVisualizer::processEvent(const sf::Event& event, sf::RenderWindow& window) {
    gGraphZoom = graphZoom_;
    gGraphPan = graphPan_;

    if (const auto* mbp = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mbp->button == sf::Mouse::Button::Left) {
            const sf::Vector2f mousePos = window.mapPixelToCoords(mbp->position);

            bool consumed = false;

            if (!consumed && !enteringEdgeWeight_ && canvasMode_ == MstCanvasMode::Matrix &&
                handleAdjacencyMatrixClick(mousePos)) {
                consumed = true;
            }

            if (!consumed && !enteringEdgeWeight_ && canvasMode_ == MstCanvasMode::Graph) {
                if (placingNode_ && isInsideGraphViewport(mousePos)) {
                    // save undo snapshot before mutating graph
                    pushUndoState();
                    const sf::Vector2f npos = screenToGraphNormalized(mousePos);
                    const int n = static_cast<int>(adjacencyMatrix_.size());
                    for (auto& row : adjacencyMatrix_) {
                        row.push_back(0);
                    }
                    adjacencyMatrix_.push_back(std::vector<int>(static_cast<size_t>(n + 1), 0));
                    nodeAlive_.push_back(true);

                    rebuildGraphFromAdjacencyMatrix();
                    graph_.setNodePosition(n, npos.x, npos.y);
                    selectedNodeId_ = n;
                    // keep placingNode_ true so user can add multiple nodes until they press the button again
                    animation_.clear();
                    playing_ = false;
                    state_ = MSTAppState::Paused;
                    timelineDirty_ = true;
                    consumed = true;
                }
            }

            if (!consumed && !enteringEdgeWeight_ && canvasMode_ == MstCanvasMode::Graph) {
                const int hitNode = hitTestNode(mousePos);
                if (deletingNodeMode_) {
                    const int aliveCount = static_cast<int>(graph_.getNodes().size());
                    if (hitNode > 0 && aliveCount > 1 && hitNode < static_cast<int>(adjacencyMatrix_.size()) &&
                        hitNode < static_cast<int>(nodeAlive_.size()) &&
                        nodeAlive_[static_cast<size_t>(hitNode)]) {
                        // save undo before removal
                        pushUndoState();
                        nodeAlive_[static_cast<size_t>(hitNode)] = false;
                        for (int i = 0; i < static_cast<int>(adjacencyMatrix_.size()); ++i) {
                            adjacencyMatrix_[static_cast<size_t>(hitNode)][static_cast<size_t>(i)] = 0;
                            adjacencyMatrix_[static_cast<size_t>(i)][static_cast<size_t>(hitNode)] = 0;
                        }
                        selectedNodeId_ = -1;
                        pendingEdgeFrom_ = -1;
                        pendingEdgeTo_ = -1;
                        enteringEdgeWeight_ = false;
                        edgeWeightInput_.clear();
                        rebuildGraphFromAdjacencyMatrix();
                        animation_.clear();
                        playing_ = false;
                        state_ = MSTAppState::Paused;
                        timelineDirty_ = true;
                    }
                    consumed = true;
                } else if (addingEdgeMode_) {
                    if (hitNode >= 0) {
                        if (pendingEdgeFrom_ < 0) {
                            pendingEdgeFrom_ = hitNode;
                            selectedNodeId_ = hitNode;
                        } else if (pendingEdgeFrom_ != hitNode) {
                            pendingEdgeTo_ = hitNode;
                            enteringEdgeWeight_ = true;
                            edgeWeightInput_.clear();
                        }
                    }
                    consumed = true;
                } else {
                    selectedNodeId_ = hitNode;
                    if (hitNode >= 0 && !playing_) {
                        draggingNode_ = true;
                        draggingNodeId_ = hitNode;
                        placingNode_ = false;
                    }
                }
            }
        }

        if (mbp->button == sf::Mouse::Button::Right) {
            const sf::Vector2f mousePos = window.mapPixelToCoords(mbp->position);
            if (canvasMode_ == MstCanvasMode::Graph && isInsideGraphViewport(mousePos)) {
                panningGraph_ = true;
                lastPanMouse_ = mousePos;
            }
        }
    }

    if (const auto* mbr = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (mbr->button == sf::Mouse::Button::Left) {
            draggingNode_ = false;
            draggingNodeId_ = -1;
        }
        if (mbr->button == sf::Mouse::Button::Right) {
            panningGraph_ = false;
        }
    }

    if (const auto* mm = event.getIf<sf::Event::MouseMoved>()) {
        const sf::Vector2f mousePos = window.mapPixelToCoords(mm->position);

        if (canvasMode_ == MstCanvasMode::Graph && draggingNode_ && draggingNodeId_ >= 0 && !playing_) {
            if (isInsideGraphViewport(mousePos)) {
                const sf::Vector2f npos = screenToGraphNormalized(mousePos);
                graph_.setNodePosition(draggingNodeId_, npos.x, npos.y);
            }
        }

        if (canvasMode_ == MstCanvasMode::Graph && panningGraph_) {
            const sf::Vector2f delta(mousePos.x - lastPanMouse_.x, mousePos.y - lastPanMouse_.y);
            graphPan_.x += delta.x;
            graphPan_.y += delta.y;
            lastPanMouse_ = mousePos;
            gGraphPan = graphPan_;
        }
    }

    if (const auto* mw = event.getIf<sf::Event::MouseWheelScrolled>()) {
        const sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(static_cast<int>(mw->position.x), static_cast<int>(mw->position.y)));
        if (canvasMode_ == MstCanvasMode::Graph && isInsideGraphViewport(mousePos)) {
            const float oldZoom = graphZoom_;
            const float factor = (mw->delta > 0.f) ? 1.12f : (1.0f / 1.12f);
            graphZoom_ = std::clamp(graphZoom_ * factor, 0.55f, 2.8f);

            const sf::Vector2f center = graphViewCenter();
            graphPan_ = sf::Vector2f(
                mousePos.x - center.x - (mousePos.x - center.x - graphPan_.x) * (graphZoom_ / oldZoom),
                mousePos.y - center.y - (mousePos.y - center.y - graphPan_.y) * (graphZoom_ / oldZoom)
            );

            gGraphZoom = graphZoom_;
            gGraphPan = graphPan_;
        }
    }

    if (const auto* te = event.getIf<sf::Event::TextEntered>()) {
        // Node count input handling
        if (enteringNodeCount_) {
            const auto uni = te->unicode;
            if (uni >= '0' && uni <= '9') {
                if (nodeCountInput_.size() < 2) nodeCountInput_.push_back(static_cast<char>(uni));
            } else if (uni == 8) {
                if (!nodeCountInput_.empty()) nodeCountInput_.pop_back();
            } else if (uni == 13) {
                if (!nodeCountInput_.empty()) {
                    int v = std::stoi(nodeCountInput_);
                    manualNodeCount_ = std::clamp(v, 2, 20);
                    autoNodeMode_ = false;
                    
                    pushUndoState();
                    graph_ = GraphLoader::createNodesOnlyGraph(manualNodeCount_);
                    selectedNodeId_ = -1;
                    placingNode_ = false;
                    deletingNodeMode_ = false;
                    addingEdgeMode_ = false;
                    pendingEdgeFrom_ = -1;
                    pendingEdgeTo_ = -1;
                    enteringEdgeWeight_ = false;
                    edgeWeightInput_.clear();
                    syncAdjacencyMatrixFromGraph();
                    rebuildTimeline();
                }
                enteringNodeCount_ = false;
            } else if (uni == 27) {
                enteringNodeCount_ = false;
                nodeCountInput_.clear();
            }
        } else if (enteringEdgeWeight_) {
            const auto uni = te->unicode;
            if (uni >= '0' && uni <= '9') {
                if (edgeWeightInput_.size() < 3) {
                    edgeWeightInput_.push_back(static_cast<char>(uni));
                }
                } else if (uni == 8) {
                    if (!edgeWeightInput_.empty()) {
                        edgeWeightInput_.pop_back();
                    }
                } else if (uni == 13) {
                    int w = 0;
                    if (!edgeWeightInput_.empty()) {
                        w = std::stoi(edgeWeightInput_);
                    } else if (addingEdgeMode_) {
                        w = 1;
                    }
                    w = std::clamp(w, 0, 999);

                    const int n = static_cast<int>(adjacencyMatrix_.size());
                    if (pendingEdgeFrom_ >= 0 && pendingEdgeTo_ >= 0 && pendingEdgeFrom_ < n && pendingEdgeTo_ < n &&
                        pendingEdgeFrom_ != pendingEdgeTo_) {
                        // save undo snapshot before changing weights
                        pushUndoState();
                        adjacencyMatrix_[static_cast<size_t>(pendingEdgeFrom_)][static_cast<size_t>(pendingEdgeTo_)] = w;
                        adjacencyMatrix_[static_cast<size_t>(pendingEdgeTo_)][static_cast<size_t>(pendingEdgeFrom_)] = w;
                        rebuildGraphFromAdjacencyMatrix();
                        animation_.clear();
                        playing_ = false;
                        state_ = MSTAppState::Paused;
                        timelineDirty_ = true;
                    }

                    pendingEdgeFrom_ = -1;
                    pendingEdgeTo_ = -1;
                    addingEdgeMode_ = false;
                    enteringEdgeWeight_ = false;
                    edgeWeightInput_.clear();
                } else if (uni == 27) {
                    pendingEdgeFrom_ = -1;
                    pendingEdgeTo_ = -1;
                    enteringEdgeWeight_ = false;
                    edgeWeightInput_.clear();
                }
        }
    }

    if (const auto* kp = event.getIf<sf::Event::KeyPressed>()) {
        if (enteringEdgeWeight_ || enteringNodeCount_) {
            if (kp->code == sf::Keyboard::Key::Escape) {
                if (enteringEdgeWeight_) {
                    pendingEdgeFrom_ = -1;
                    pendingEdgeTo_ = -1;
                    enteringEdgeWeight_ = false;
                    edgeWeightInput_.clear();
                }
                if (enteringNodeCount_) {
                    enteringNodeCount_ = false;
                    nodeCountInput_.clear();
                }
            }
        } else {
            if (kp->code == sf::Keyboard::Key::Space)  onActionPlayPause();
            if (kp->code == sf::Keyboard::Key::Left)   onActionPrev();
            if (kp->code == sf::Keyboard::Key::Right)  onActionNext();
            if (kp->code == sf::Keyboard::Key::B)      onActionBuild();
            if (kp->code == sf::Keyboard::Key::R)      onActionRandom();
            if (kp->code == sf::Keyboard::Key::K)      onActionKruskal();
            if (kp->code == sf::Keyboard::Key::P)      onActionPrim();
            if (kp->code == sf::Keyboard::Key::N)      onActionNewGraph();
            if (kp->code == sf::Keyboard::Key::A)      onActionAddNode();
            if (kp->code == sf::Keyboard::Key::E)      onActionAddEdgeMode();
            if (kp->code == sf::Keyboard::Key::D)      onActionRemoveNode();
            if (kp->code == sf::Keyboard::Key::M)      canvasMode_ = (canvasMode_ == MstCanvasMode::Graph) ? MstCanvasMode::Matrix : MstCanvasMode::Graph;
        }
    }
}

void MSTVisualizer::update(sf::Vector2i mousePos) {
    gGraphZoom = graphZoom_;
    gGraphPan = graphPan_;

    // Update button layout whenever dropdown visibility changes
    if (controlPanelExpanded_) {
        updateLayout();
    }

    layout_.update(mousePos);

    // Reposition toggle button depending on collapsed state (keeps a small visible handle when collapsed)
    float toggleX = controlPanelExpanded_ ? (layoutPanelX_ + layoutPanelWidth_ + 16.f) : 12.f;
    float toggleY = kLeftPanelY + (kLeftPanelH - 44.f) / 2.f;
    btnAlgoToggle_.emplace(font_, controlPanelExpanded_ ? "<" : ">", toggleX, toggleY, 20.f, 44.f, 10.f, ThemeManager::current.secondary);
    btnAlgoToggle_->setCallback([this](){ onActionToggleAlgorithm(); });
    if (btnAlgoToggle_) btnAlgoToggle_->update(mousePos);

    // Only update other buttons if control panel is expanded
    if (controlPanelExpanded_) {
        if (btnNew_) btnNew_->update(mousePos);
        if (btnUndo_) btnUndo_->update(mousePos);
        if (btnNodeMode_) btnNodeMode_->update(mousePos);
        if (btnResetView_) btnResetView_->update(mousePos);
        if (btnAddNode_) btnAddNode_->update(mousePos);
        if (btnAddEdge_) btnAddEdge_->update(mousePos);
        if (btnDelete_) btnDelete_->update(mousePos);
        if (algorithmPanelExpanded_) {
            if (btnKruskal_) btnKruskal_->update(mousePos);
            if (btnPrim_) btnPrim_->update(mousePos);
        }
        if (btnMatrix_) btnMatrix_->update(mousePos);
        if (btnBuild_) btnBuild_->update(mousePos);
        if (showNewMenu_) {
            if (btnNewRandom_) btnNewRandom_->update(mousePos);
            if (btnNewLoadFile_) btnNewLoadFile_->update(mousePos);
        }
    }

    speed_ = layout_.getSpeed() * 2.0f;
    if (speed_ < 0.1f) speed_ = 0.1f;

    if (playing_ && !animation_.empty() && playClock_.getElapsedTime().asSeconds() >= (1.0f / speed_)) {
        playClock_.restart();
        if (!animation_.moveNext()) {
            playing_ = false;
            state_ = MSTAppState::Finished;
            layout_.setPaused(true);
        }
    }
}

void MSTVisualizer::draw(sf::RenderWindow& window, bool showUI) {
    gGraphZoom = graphZoom_;
    gGraphPan = graphPan_;

    const MSTStep* step = animation_.currentStep();

    RenderViewModel vm;
    vm.kind = RenderViewKind::MST;
    vm.graph = &graph_;
    vm.selectedNodeId = selectedNodeId_;
    if (step != nullptr) {
        vm.highlightedEdges = step->highlightedEdges;
        vm.candidateEdges = step->candidateEdges;
        vm.highlightedNodes = step->highlightedNodes;
    }
    if (canvasMode_ == MstCanvasMode::Graph) {
        Renderer::draw(window, vm, font_);
    }

    if (!showUI) return;

    int total = std::max(0, animation_.totalSteps() - 1);
    layout_.setStepText("Step " + std::to_string(animation_.currentIndex()) + " / " + std::to_string(total));

    if (step) {
        layout_.setDescription(step->description);
        layout_.setPseudoCode(pseudocode_);
        std::vector<int> zeroBasedLines;
        for (int ln : step->pseudocodeLines) zeroBasedLines.push_back(ln - 1);
        layout_.setActiveCodeLines(zeroBasedLines);
    } else {
        layout_.setDescription("Ready.");
        layout_.setPseudoCode(pseudocode_);
        layout_.setActiveCodeLines({});
    }

    layout_.draw(window);

    auto syncBtn = [](std::optional<RoundedButton>& b) {
        if (b) b->setThemeColor(ThemeManager::current.secondary);
    };
    syncBtn(btnNew_); syncBtn(btnUndo_); syncBtn(btnNodeMode_); syncBtn(btnResetView_); syncBtn(btnAddNode_);
    syncBtn(btnAddEdge_); syncBtn(btnDelete_); syncBtn(btnKruskal_);
    syncBtn(btnPrim_); syncBtn(btnMatrix_); syncBtn(btnBuild_);
    syncBtn(btnAlgoToggle_);
    syncBtn(btnNewRandom_); syncBtn(btnNewLoadFile_);

    if (btnNodeMode_) {
        btnNodeMode_->setText("Nodes: " + std::to_string(manualNodeCount_));
    }
    if (btnMatrix_) btnMatrix_->setText(canvasMode_ == MstCanvasMode::Matrix ? "View: Graph" : "View: Matrix");
    if (btnAlgoToggle_) btnAlgoToggle_->setText(controlPanelExpanded_ ? "<" : ">");

    if (btnBuild_) {
        if (timelineDirty_) {
            btnBuild_->setThemeColor(sf::Color(208, 58, 64));
        } else {
            btnBuild_->setThemeColor(ThemeManager::current.primary);
        }
    }
    if (btnNodeMode_) btnNodeMode_->setThemeColor(ThemeManager::current.secondary);

    if (placingNode_ && btnAddNode_) btnAddNode_->setThemeColor(ThemeManager::current.primary);
    if (addingEdgeMode_ && btnAddEdge_) btnAddEdge_->setThemeColor(ThemeManager::current.primary);
    if (deletingNodeMode_ && btnDelete_) btnDelete_->setThemeColor(ThemeManager::current.primary);
    if (canvasMode_ == MstCanvasMode::Matrix && btnMatrix_) btnMatrix_->setThemeColor(ThemeManager::current.primary);
    if (algorithmPanelExpanded_) {
        if (algorithmType_ == algo::AlgorithmType::Kruskal && btnKruskal_) btnKruskal_->setThemeColor(ThemeManager::current.primary);
        if (algorithmType_ == algo::AlgorithmType::Prim && btnPrim_) btnPrim_->setThemeColor(ThemeManager::current.primary);
    }

    // Draw toggle button (always visible)
    if (btnAlgoToggle_) btnAlgoToggle_->draw(window);

    // Draw other buttons only if panel is expanded
    if (controlPanelExpanded_) {
        if (btnNew_) btnNew_->draw(window);
        if (btnUndo_) btnUndo_->draw(window);
        if (showNewMenu_) {
            if (btnNewRandom_) btnNewRandom_->draw(window);
            if (btnNewLoadFile_) btnNewLoadFile_->draw(window);
        }
        if (btnNodeMode_) btnNodeMode_->draw(window);
        if (btnResetView_) btnResetView_->draw(window);
        if (btnAddNode_) btnAddNode_->draw(window);
        if (btnAddEdge_) btnAddEdge_->draw(window);
        if (btnDelete_) btnDelete_->draw(window);
        if (algorithmPanelExpanded_) {
            if (btnKruskal_) btnKruskal_->draw(window);
            if (btnPrim_) btnPrim_->draw(window);
        }
        if (btnMatrix_) btnMatrix_->draw(window);
        if (btnBuild_) btnBuild_->draw(window);
    }

    // Section titles (only show when panel is expanded) - positioned with computed group coordinates
    if (controlPanelExpanded_) {
        // System & Tools title (above first group)
        sf::Text systemTitle(font_, "System & Tools", 18);
        systemTitle.setFillColor(ThemeManager::current.textColor);
        systemTitle.setPosition(sf::Vector2f(layoutPanelX_, layoutSystemTitleY_));
        window.draw(systemTitle);

        // MST Actions title
        sf::Text actionsTitle(font_, "MST Actions", 18);
        actionsTitle.setFillColor(ThemeManager::current.textColor);
        actionsTitle.setPosition(sf::Vector2f(layoutPanelX_, layoutActionsTitleY_));
        window.draw(actionsTitle);

        // Algorithms title
        sf::Text algoTitle(font_, "Algorithms", 18);
        algoTitle.setFillColor(ThemeManager::current.textColor);
        algoTitle.setPosition(sf::Vector2f(layoutPanelX_, layoutAlgorithmsTitleY_));
        window.draw(algoTitle);
    }

    // Move hero number/title down near the bottom left corner
    sf::Text heroNum(font_, "04", 40);
    heroNum.setFillColor(ThemeManager::current.textColor);
    heroNum.setPosition(sf::Vector2f(layoutPanelX_, 680.f));
    window.draw(heroNum);

    sf::Text heroLine1(font_, "Minimum Spanning", 46);
    heroLine1.setFillColor(ThemeManager::current.textColor);
    heroLine1.setPosition(sf::Vector2f(layoutPanelX_, 730.f));
    window.draw(heroLine1);

    sf::Text heroLine2(font_, "Tree", 46);
    heroLine2.setFillColor(ThemeManager::current.primary);
    heroLine2.setPosition(sf::Vector2f(layoutPanelX_, 785.f));
    window.draw(heroLine2);

    sf::Text modeText(font_, "", 16);
    modeText.setFillColor(ThemeManager::current.textColor);
    if (enteringNodeCount_) {
        modeText.setString("Enter node count (2-20): " + nodeCountInput_ + "_");
    } else if (enteringEdgeWeight_) {
        modeText.setString("Enter edge weight (0 = remove): " + edgeWeightInput_ + "_");
    } else if (pendingEdgeFrom_ >= 0 && canvasMode_ == MstCanvasMode::Graph) {
        modeText.setString("Select destination node...");
    }

    if (enteringNodeCount_ || enteringEdgeWeight_ || (pendingEdgeFrom_ >= 0 && canvasMode_ == MstCanvasMode::Graph)) {
        sf::FloatRect textBounds = modeText.getLocalBounds();
        float centerX = (kGraphMinX + kGraphMaxX) / 2.f;
        float topY = kGraphMinY + 20.f;
        modeText.setPosition({centerX - textBounds.size.x / 2.f, topY});
        window.draw(modeText);
    }

    const int n = static_cast<int>(adjacencyMatrix_.size());
    const float matrixCell = matrixCellForSize(n);
    const sf::Vector2f matrixOrigin = matrixOriginForSize(n);

    if (canvasMode_ == MstCanvasMode::Matrix) {
        sf::Text matrixHelp(font_, "Adjacency Matrix: click a cell to edit weight", 14);
        matrixHelp.setPosition({matrixOrigin.x, matrixOrigin.y - 34.f});
        matrixHelp.setFillColor(ThemeManager::current.textColor);
        clampTextToWidth(matrixHelp, kGraphMaxX - matrixOrigin.x - 8.0f);
        window.draw(matrixHelp);

        int deletedCount = 0;
        for (bool alive : nodeAlive_) {
            if (!alive) {
                ++deletedCount;
            }
        }

        if (deletedCount > 0) {
            sf::Text matrixLegend(monoFont_, "x = deleted node, disabled row/column (" +
                                      std::to_string(deletedCount) + " removed)", 12);
            matrixLegend.setPosition({matrixOrigin.x, matrixOrigin.y - 16.f});
            matrixLegend.setFillColor(sf::Color(220, 120, 120));
            clampTextToWidth(matrixLegend, kGraphMaxX - matrixOrigin.x - 8.0f);
            window.draw(matrixLegend);
        }

        for (int r = -1; r < n; ++r) {
            for (int c = -1; c < n; ++c) {
                sf::RectangleShape cell(sf::Vector2f(matrixCell - 1.0f, matrixCell - 1.0f));
                cell.setPosition({matrixOrigin.x + (static_cast<float>(c + 1) * matrixCell),
                                 matrixOrigin.y + (static_cast<float>(r + 1) * matrixCell)});

                if (r == -1 || c == -1) {
                    const bool inactiveHeader =
                        (r == -1 && c >= 0 && c < n && c < static_cast<int>(nodeAlive_.size()) && !nodeAlive_[static_cast<size_t>(c)]) ||
                        (c == -1 && r >= 0 && r < n && r < static_cast<int>(nodeAlive_.size()) && !nodeAlive_[static_cast<size_t>(r)]);
                    cell.setFillColor(inactiveHeader ? sf::Color(50, 36, 36) : sf::Color(55, 60, 72));
                } else if (r == c) {
                    const bool inactive = r < static_cast<int>(nodeAlive_.size()) && !nodeAlive_[static_cast<size_t>(r)];
                    cell.setFillColor(inactive ? sf::Color(50, 36, 36) : sf::Color(38, 42, 50));
                } else {
                    const bool inactive =
                        r < static_cast<int>(nodeAlive_.size()) && c < static_cast<int>(nodeAlive_.size()) &&
                        (!nodeAlive_[static_cast<size_t>(r)] || !nodeAlive_[static_cast<size_t>(c)]);
                    if (inactive) {
                        cell.setFillColor(sf::Color(42, 32, 32));
                    } else {
                        cell.setFillColor(adjacencyMatrix_[static_cast<size_t>(r)][static_cast<size_t>(c)] > 0
                                              ? sf::Color(50, 95, 55)
                                              : sf::Color(36, 40, 48));
                    }
                }
                window.draw(cell);

                unsigned int fontSize = static_cast<unsigned int>(std::max(9.0f, matrixCell * 0.45f));
                sf::Text t(font_, "", fontSize);
                t.setFillColor(sf::Color(232, 232, 232));
                if (r == -1 && c >= 0) {
                    if (c < static_cast<int>(nodeAlive_.size()) && !nodeAlive_[static_cast<size_t>(c)]) {
                        t.setString("x");
                        t.setFillColor(sf::Color(220, 120, 120));
                    } else {
                        t.setString(std::to_string(c));
                    }
                } else if (c == -1 && r >= 0) {
                    if (r < static_cast<int>(nodeAlive_.size()) && !nodeAlive_[static_cast<size_t>(r)]) {
                        t.setString("x");
                        t.setFillColor(sf::Color(220, 120, 120));
                    } else {
                        t.setString(std::to_string(r));
                    }
                } else if (r >= 0 && c >= 0) {
                    if ((r < static_cast<int>(nodeAlive_.size()) && !nodeAlive_[static_cast<size_t>(r)]) ||
                        (c < static_cast<int>(nodeAlive_.size()) && !nodeAlive_[static_cast<size_t>(c)])) {
                        t.setString("-");
                        t.setFillColor(sf::Color(180, 120, 120));
                    } else {
                        t.setString(std::to_string(adjacencyMatrix_[static_cast<size_t>(r)][static_cast<size_t>(c)]));
                    }
                }
                sf::FloatRect textBounds = t.getLocalBounds();
                t.setPosition({cell.getPosition().x + (matrixCell - 1.0f - textBounds.size.x) / 2.f - textBounds.position.x,
                               cell.getPosition().y + (matrixCell - 1.0f - textBounds.size.y) / 2.f - textBounds.position.y});
                window.draw(t);
            }
        }
    }

}
