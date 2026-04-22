#pragma once

#include <SFML/Graphics/Color.hpp>

namespace config {
inline constexpr unsigned int kWindowWidth = 1280;
inline constexpr unsigned int kWindowHeight = 720;
inline constexpr float kNodeRadius = 18.0f;
inline const sf::Color kBackgroundColor(242, 242, 239);
inline const sf::Color kNodeFillColor(170, 170, 170);
inline const sf::Color kNodeOutlineColor(28, 28, 28);
inline const sf::Color kEdgeColor(112, 150, 88);
inline const sf::Color kHighlightEdgeColor(196, 76, 92);
inline const sf::Color kCandidateEdgeColor(217, 170, 74);
}  // namespace config
