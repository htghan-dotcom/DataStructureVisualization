#include "Renderer.h"

#include "EdgeRender.h"
#include "NodeRender.h"
#include "../../config/Config.h"

void Renderer::draw(sf::RenderWindow& window, const RenderViewModel& vm, const sf::Font& font) {
    if (vm.kind != RenderViewKind::MST) {
        sf::Text unsupported("Structure renderer scaffolded (implementation pending)", font, 16);
        unsupported.setPosition(270.f, 60.f);
        unsupported.setFillColor(sf::Color(220, 220, 220));
        window.draw(unsupported);
        return;
    }

    if (vm.graph == nullptr) {
        return;
    }

    EdgeRender::draw(window, *vm.graph, vm.highlightedEdges, vm.candidateEdges, font);
    for (const auto& node : vm.graph->getNodes()) {
        NodeRender::draw(window, node, config::kNodeRadius, font);
    }
}
