#include "Prim.h"


std::vector<Edge> Prim::run(const Graph& g, int startNode) {
    std::vector<Edge> resultMST;
    int numNodes = g.getNodes().size();
    if (numNodes == 0) return resultMST;

    std::vector<std::vector<std::pair<int, int>>> adj(numNodes);
    for (const auto& e : g.getEdges()) {
        adj[e.from].push_back({e.to, e.weight});
        adj[e.to].push_back({e.from, e.weight});
    }

    using Path = std::pair<int, std::pair<int, int>>;
    std::priority_queue<Path, std::vector<Path>, std::greater<Path>> pq;

    std::vector<bool> visited(numNodes, false);

    visited[startNode] = true;
    for (const auto& neighbor : adj[startNode]) {
        pq.push({neighbor.second, {startNode, neighbor.first}});
    }

    while (!pq.empty() && resultMST.size() < numNodes - 1) {
        auto top = pq.top();
        pq.pop();

        int weight = top.first;
        int u = top.second.first;
        int v = top.second.second;

        if (visited[v]) continue;

        visited[v] = true;
        resultMST.push_back(Edge(u, v, weight));

        for (const auto& neighbor : adj[v]) {
            if (!visited[neighbor.first]) {
                pq.push({neighbor.second, {v, neighbor.first}});
            }
        }
    }

    return resultMST;
}