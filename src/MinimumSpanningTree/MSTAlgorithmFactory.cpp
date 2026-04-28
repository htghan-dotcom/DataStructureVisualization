#include "MinimumSpanningTree/MSTAlgorithmFactory.h"

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

MSTStep makeBaseStep(int index, StepEvent event, const std::string& description) {
    MSTStep step;
    step.index = index;
    step.structure = StructureKind::MST;
    step.event = event;
    step.description = description;
    return step;
}

std::vector<MSTStep> buildKruskalSteps(const Graph& graph) {
    std::vector<MSTStep> steps;

    const auto& edges = graph.getEdges();
    std::vector<Edge> sorted = edges;
    std::sort(sorted.begin(), sorted.end());

    MSTStep start = makeBaseStep(0, StepEvent::Visit, "Kruskal start: sort edges by weight");
    start.pseudocodeLines = {1};
    steps.push_back(start);

    int maxId = -1;
    for (const auto& node : graph.getNodes()) {
        maxId = std::max(maxId, node.id);
    }

    std::vector<Edge> accepted;
    DSU dsu(maxId + 1);

    for (const auto& edge : sorted) {
        MSTStep candidate = makeBaseStep(static_cast<int>(steps.size()), StepEvent::Candidate,
                                      "Consider edge " + std::to_string(edge.from) + "-" + std::to_string(edge.to) +
                                          " (w=" + std::to_string(edge.weight) + ")");
        candidate.candidateEdges.push_back(edge);
        candidate.highlightedEdges = accepted;
        candidate.pseudocodeLines = {2, 3};
        steps.push_back(candidate);

        const int rootU = dsu.find(edge.from);
        const int rootV = dsu.find(edge.to);
        const bool merged = dsu.unite(edge.from, edge.to);

        MSTStep decision = makeBaseStep(static_cast<int>(steps.size()), merged ? StepEvent::Accept : StepEvent::Reject,
                                     merged ? "Accept edge (no cycle)" : "Reject edge (cycle detected)");
        decision.candidateEdges.push_back(edge);
        if (merged) {
            accepted.push_back(edge);
        }
        decision.highlightedEdges = accepted;
        decision.pseudocodeLines = merged ? std::vector<int>{4, 5} : std::vector<int>{4, 6};
        decision.metadata["u"] = std::to_string(edge.from);
        decision.metadata["v"] = std::to_string(edge.to);
        decision.metadata["rootU"] = std::to_string(rootU);
        decision.metadata["rootV"] = std::to_string(rootV);
        steps.push_back(decision);
    }

    MSTStep end = makeBaseStep(static_cast<int>(steps.size()), StepEvent::Complete,
                            "Kruskal complete: MST edges selected");
    end.highlightedEdges = accepted;
    end.pseudocodeLines = {7};
    steps.push_back(end);

    return steps;
}

std::vector<MSTStep> buildPrimSteps(const Graph& graph, int startNode) {
    std::vector<MSTStep> steps;
    const auto& nodes = graph.getNodes();
    const auto& edges = graph.getEdges();

    if (nodes.empty()) {
        return steps;
    }

    int maxId = -1;
    for (const auto& node : nodes) {
        maxId = std::max(maxId, node.id);
    }
    if (maxId < 0) {
        return steps;
    }

    const int activeCount = static_cast<int>(nodes.size());
    std::vector<bool> active(static_cast<size_t>(maxId + 1), false);
    for (const auto& node : nodes) {
        active[static_cast<size_t>(node.id)] = true;
    }

    if (startNode < 0 || startNode > maxId || !active[static_cast<size_t>(startNode)]) {
        startNode = nodes.front().id;
    }

    std::vector<std::vector<Edge>> adj(static_cast<size_t>(maxId + 1));
    for (const auto& e : edges) {
        if (e.from < 0 || e.to < 0 || e.from > maxId || e.to > maxId) {
            continue;
        }
        if (!active[static_cast<size_t>(e.from)] || !active[static_cast<size_t>(e.to)]) {
            continue;
        }
        adj[static_cast<size_t>(e.from)].push_back(e);
        adj[static_cast<size_t>(e.to)].push_back(Edge{e.to, e.from, e.weight});
    }

    std::vector<bool> inTree(static_cast<size_t>(maxId + 1), false);
    using Item = std::pair<int, Edge>;
    auto cmp = [](const Item& a, const Item& b) { return a.first > b.first; };
    std::priority_queue<Item, std::vector<Item>, decltype(cmp)> pq(cmp);

    MSTStep start = makeBaseStep(0, StepEvent::Visit, "Prim start from node " + std::to_string(startNode));
    start.highlightedNodes.push_back(startNode);
    start.pseudocodeLines = {1};
    steps.push_back(start);

    inTree[static_cast<size_t>(startNode)] = true;
    for (const auto& e : adj[static_cast<size_t>(startNode)]) {
        pq.push({e.weight, e});
    }

    std::vector<Edge> accepted;
    while (!pq.empty() && static_cast<int>(accepted.size()) < activeCount - 1) {
        Edge e = pq.top().second;
        pq.pop();

        MSTStep candidate = makeBaseStep(static_cast<int>(steps.size()), StepEvent::Candidate,
                                      "Pick lightest frontier edge " + std::to_string(e.from) + "-" +
                                          std::to_string(e.to) + " (w=" + std::to_string(e.weight) + ")");
        candidate.highlightedEdges = accepted;
        candidate.candidateEdges.push_back(Edge{e.from, e.to, e.weight});
        candidate.pseudocodeLines = {2, 3};
        steps.push_back(candidate);

        if (inTree[static_cast<size_t>(e.to)]) {
            MSTStep reject = makeBaseStep(static_cast<int>(steps.size()), StepEvent::Reject,
                                       "Reject edge: destination already in tree");
            reject.highlightedEdges = accepted;
            reject.candidateEdges.push_back(Edge{e.from, e.to, e.weight});
            reject.pseudocodeLines = {4};
            steps.push_back(reject);
            continue;
        }

        inTree[static_cast<size_t>(e.to)] = true;
        accepted.push_back(Edge{e.from, e.to, e.weight});

        MSTStep accept = makeBaseStep(static_cast<int>(steps.size()), StepEvent::Accept,
                                   "Accept edge and expand frontier");
        accept.highlightedEdges = accepted;
        accept.highlightedNodes.push_back(e.to);
        accept.pseudocodeLines = {5, 6};
        steps.push_back(accept);

        for (const auto& next : adj[static_cast<size_t>(e.to)]) {
            if (next.to >= 0 && next.to <= maxId && active[static_cast<size_t>(next.to)] &&
                !inTree[static_cast<size_t>(next.to)]) {
                pq.push({next.weight, next});
            }
        }
    }

    MSTStep end = makeBaseStep(static_cast<int>(steps.size()), StepEvent::Complete,
                            "Prim complete: MST edges selected");
    end.highlightedEdges = accepted;
    end.pseudocodeLines = {7};
    steps.push_back(end);

    return steps;
}
}  // namespace

void MSTAnimation::clear() {
    steps_.clear();
    cursor_ = 0;
}

void MSTAnimation::setSteps(const std::vector<MSTStep>& steps) {
    steps_ = steps;
    cursor_ = 0;
}

void MSTAnimation::addStep(const MSTStep& step) {
    steps_.push_back(step);
}

bool MSTAnimation::empty() const {
    return steps_.empty();
}

int MSTAnimation::totalSteps() const {
    return static_cast<int>(steps_.size());
}

int MSTAnimation::currentIndex() const {
    return cursor_;
}

const MSTStep* MSTAnimation::currentStep() const {
    if (steps_.empty()) {
        return nullptr;
    }
    if (cursor_ < 0 || cursor_ >= totalSteps()) {
        return nullptr;
    }
    return &steps_[cursor_];
}

const MSTStep* MSTAnimation::stepAt(int index) const {
    if (index < 0 || index >= totalSteps()) {
        return nullptr;
    }
    return &steps_[index];
}

bool MSTAnimation::moveNext() {
    if (steps_.empty() || cursor_ + 1 >= totalSteps()) {
        return false;
    }
    ++cursor_;
    return true;
}

bool MSTAnimation::movePrev() {
    if (steps_.empty() || cursor_ - 1 < 0) {
        return false;
    }
    --cursor_;
    return true;
}

void MSTAnimation::moveToStart() {
    cursor_ = 0;
}

void MSTAnimation::moveToEnd() {
    if (steps_.empty()) {
        cursor_ = 0;
        return;
    }
    cursor_ = totalSteps() - 1;
}

void MSTAnimation::setCursor(int index) {
    if (steps_.empty()) {
        cursor_ = 0;
        return;
    }
    cursor_ = std::clamp(index, 0, totalSteps() - 1);
}

namespace algo {
std::vector<MSTStep> AlgorithmFactory::buildSteps(AlgorithmType type, const Graph& graph, int startNode) {
    switch (type) {
        case AlgorithmType::Kruskal:
            return buildKruskalSteps(graph);
        case AlgorithmType::Prim:
            return buildPrimSteps(graph, startNode);
        default:
            return {};
    }
}

std::vector<Edge> AlgorithmFactory::run(AlgorithmType type, const Graph& graph) {
    const auto steps = buildSteps(type, graph, 0);
    if (steps.empty()) {
        return {};
    }
    return steps.back().highlightedEdges;
}
}  // namespace algo