#include "Prim.h"

namespace {
Step makeBaseStep(int index, StepEvent event, const std::string& description) {
    Step step;
    step.index = index;
    step.structure = StructureKind::MST;
    step.event = event;
    step.description = description;
    return step;
}
}  // namespace

std::vector<Step> Prim::buildSteps(const Graph& graph, int startNode) {
    std::vector<Step> steps;
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
    using Item = std::pair<int, Edge>;  // weight, edge
    auto cmp = [](const Item& a, const Item& b) { return a.first > b.first; };
    std::priority_queue<Item, std::vector<Item>, decltype(cmp)> pq(cmp);

    Step start = makeBaseStep(0, StepEvent::Visit,
                              "Start Prim at node " + std::to_string(startNode) +
                                  ": add node to tree and push its edges");
    start.highlightedNodes.push_back(startNode);
    start.pseudocodeLines = {1};
    steps.push_back(start);

    inTree[startNode] = true;
    for (const auto& e : adj[startNode]) {
        pq.push({e.weight, e});
    }

    std::vector<Edge> accepted;
    while (!pq.empty() && static_cast<int>(accepted.size()) < activeCount - 1) {
        Edge e = pq.top().second;
        pq.pop();

        Step candidate = makeBaseStep(static_cast<int>(steps.size()), StepEvent::Candidate,
                                      "Pick minimum frontier edge " + std::to_string(e.from) + "-" +
                                          std::to_string(e.to) + " (w=" + std::to_string(e.weight) + ")");
        candidate.highlightedEdges = accepted;
        candidate.candidateEdges.push_back(Edge{e.from, e.to, e.weight});
        candidate.pseudocodeLines = {2, 3};
        steps.push_back(candidate);

        if (inTree[e.to]) {
            Step reject = makeBaseStep(static_cast<int>(steps.size()), StepEvent::Reject,
                                       "Reject " + std::to_string(e.from) + "-" + std::to_string(e.to) +
                                           ": destination already in tree");
            reject.highlightedEdges = accepted;
            reject.candidateEdges.push_back(Edge{e.from, e.to, e.weight});
            reject.pseudocodeLines = {4};
            steps.push_back(reject);
            continue;
        }

        inTree[e.to] = true;
        accepted.push_back(Edge{e.from, e.to, e.weight});

        Step accept = makeBaseStep(static_cast<int>(steps.size()), StepEvent::Accept,
                                   "Accept " + std::to_string(e.from) + "-" + std::to_string(e.to) +
                                       ": add node " + std::to_string(e.to) + " and expand frontier");
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

    Step end = makeBaseStep(static_cast<int>(steps.size()), StepEvent::Complete,
                            "Prim complete: selected MST edges");
    end.highlightedEdges = accepted;
    end.pseudocodeLines = {7};
    steps.push_back(end);

    return steps;
}
