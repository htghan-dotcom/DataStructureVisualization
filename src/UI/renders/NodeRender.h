#pragma once

#include <SFML/Graphics.hpp>
#include "../../Algo/Graph/Node.h"

class NodeRender {
public:
    static void draw(sf::RenderWindow& window, const Node& node, float radius, const sf::Font& font);
};
