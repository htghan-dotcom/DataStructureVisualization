#pragma once

#include <SFML/Graphics/Color.hpp>

namespace config {
inline constexpr unsigned int kWindowWidth = 1280;
inline constexpr unsigned int kWindowHeight = 720;
inline constexpr float kNodeRadius = 18.0f;
inline const sf::Color kBackgroundColor(240, 240, 240);
inline const sf::Color kNodeFillColor(170, 170, 170);
inline const sf::Color kNodeOutlineColor(28, 28, 28);
inline const sf::Color kEdgeColor(126, 136, 150);
inline const sf::Color kHighlightEdgeColor(78, 171, 98);
inline const sf::Color kCandidateEdgeColor(240, 204, 92);
inline const sf::Color kRejectedEdgeColor(209, 83, 83);
}  // namespace config
