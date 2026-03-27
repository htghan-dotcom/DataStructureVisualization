#include "Krustal.h"

#include "DSU.h"

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

std::vector<Step> Krustal::buildSteps(const Graph& graph) {
    std::vector<Step> steps;

    const auto& edges = graph.getEdges();
    std::vector<Edge> sorted = edges;
    std::sort(sorted.begin(), sorted.end());

    Step start = makeBaseStep(0, StepEvent::Visit, "Kruskal start: sort edges by weight");
    start.pseudocodeLines = {1};
    steps.push_back(start);

    int maxId = -1;
    for (const auto& node : graph.getNodes()) {
        maxId = std::max(maxId, node.id);
    }

    std::vector<Edge> accepted;
    DSU dsu(maxId + 1);

    for (const auto& edge : sorted) {
        Step candidate = makeBaseStep(static_cast<int>(steps.size()), StepEvent::Candidate,
                                      "Consider edge " + std::to_string(edge.from) + "-" + std::to_string(edge.to) +
                                          " (w=" + std::to_string(edge.weight) + ")");
        candidate.candidateEdges.push_back(edge);
        candidate.highlightedEdges = accepted;
        candidate.pseudocodeLines = {2, 3};
        steps.push_back(candidate);

        DSUStepMeta meta = dsu.inspectAndUnite(edge.from, edge.to);

        Step decision = makeBaseStep(static_cast<int>(steps.size()), meta.merged ? StepEvent::Accept : StepEvent::Reject,
                                     meta.merged ? "Accept edge (no cycle)" : "Reject edge (cycle detected)");
        decision.candidateEdges.push_back(edge);
        if (meta.merged) {
            accepted.push_back(edge);
        }
        decision.highlightedEdges = accepted;
        decision.pseudocodeLines = meta.merged ? std::vector<int>{4, 5} : std::vector<int>{4, 6};
        decision.metadata["u"] = std::to_string(meta.u);
        decision.metadata["v"] = std::to_string(meta.v);
        decision.metadata["rootU"] = std::to_string(meta.rootU);
        decision.metadata["rootV"] = std::to_string(meta.rootV);
        steps.push_back(decision);
    }

    Step end = makeBaseStep(static_cast<int>(steps.size()), StepEvent::Complete,
                            "Kruskal complete: MST edges selected");
    end.highlightedEdges = accepted;
    end.pseudocodeLines = {7};
    steps.push_back(end);

    return steps;
}
