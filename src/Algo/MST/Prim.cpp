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

    int n = static_cast<int>(nodes.size());
    if (startNode < 0 || startNode >= n) {
        startNode = 0;
    }

    std::vector<std::vector<Edge>> adj(n);
    for (const auto& e : edges) {
        adj[e.from].push_back(e);
        adj[e.to].push_back(Edge{e.to, e.from, e.weight});
    }

    std::vector<bool> inTree(n, false);
    using Item = std::pair<int, Edge>;  // weight, edge
    auto cmp = [](const Item& a, const Item& b) { return a.first > b.first; };
    std::priority_queue<Item, std::vector<Item>, decltype(cmp)> pq(cmp);

    Step start = makeBaseStep(0, StepEvent::Visit, "Prim start from node " + std::to_string(startNode));
    start.highlightedNodes.push_back(startNode);
    start.pseudocodeLines = {1};
    steps.push_back(start);

    inTree[startNode] = true;
    for (const auto& e : adj[startNode]) {
        pq.push({e.weight, e});
    }

    std::vector<Edge> accepted;
    while (!pq.empty() && static_cast<int>(accepted.size()) < n - 1) {
        Edge e = pq.top().second;
        pq.pop();

        Step candidate = makeBaseStep(static_cast<int>(steps.size()), StepEvent::Candidate,
                                      "Pick lightest frontier edge " + std::to_string(e.from) + "-" +
                                          std::to_string(e.to) + " (w=" + std::to_string(e.weight) + ")");
        candidate.highlightedEdges = accepted;
        candidate.candidateEdges.push_back(Edge{e.from, e.to, e.weight});
        candidate.pseudocodeLines = {2, 3};
        steps.push_back(candidate);

        if (inTree[e.to]) {
            Step reject = makeBaseStep(static_cast<int>(steps.size()), StepEvent::Reject,
                                       "Reject edge: destination already in tree");
            reject.highlightedEdges = accepted;
            reject.candidateEdges.push_back(Edge{e.from, e.to, e.weight});
            reject.pseudocodeLines = {4};
            steps.push_back(reject);
            continue;
        }

        inTree[e.to] = true;
        accepted.push_back(Edge{e.from, e.to, e.weight});

        Step accept = makeBaseStep(static_cast<int>(steps.size()), StepEvent::Accept,
                                   "Accept edge and expand frontier");
        accept.highlightedEdges = accepted;
        accept.highlightedNodes.push_back(e.to);
        accept.pseudocodeLines = {5, 6};
        steps.push_back(accept);

        for (const auto& next : adj[e.to]) {
            if (!inTree[next.to]) {
                pq.push({next.weight, next});
            }
        }
    }

    Step end = makeBaseStep(static_cast<int>(steps.size()), StepEvent::Complete,
                            "Prim complete: MST edges selected");
    end.highlightedEdges = accepted;
    end.pseudocodeLines = {7};
    steps.push_back(end);

    return steps;
}
