#pragma once

#include <SFML/Graphics.hpp>
#include "../../Algo/Graph/Edge.h"
#include "../../Algo/Graph/Graph.h"
#include "../../core/Step.h"

class EdgeRender {
public:
    static void draw(sf::RenderWindow& window,
                     const Graph& graph,
                     const sf::FloatRect& graphViewport,
                     const std::vector<Edge>& accepted,
                     const std::vector<Edge>& candidates,
                     StepEvent currentStepEvent,
                     const sf::Font& font);
};

