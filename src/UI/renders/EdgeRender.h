#pragma once

#include <SFML/Graphics.hpp>
#include "../../Algo/Graph/Edge.h"
#include "../../Algo/Graph/Graph.h"
#include "../../config/Common.h"

class EdgeRender {
public:
    static void draw(sf::RenderWindow& window,
                     const Graph& graph,
                     const std::vector<Edge>& accepted,
                     const std::vector<Edge>& candidates,
                     const sf::Font& font);
};

