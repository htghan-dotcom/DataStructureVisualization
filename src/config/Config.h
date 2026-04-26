#pragma once

#include <SFML/Graphics/Color.hpp>

namespace config {
inline constexpr unsigned int kWindowWidth = 1280;
inline constexpr unsigned int kWindowHeight = 720;
inline constexpr float kNodeRadius = 18.0f;
inline const sf::Color kBackgroundColor(18, 20, 28);
inline const sf::Color kNodeFillColor(48, 140, 210);
inline const sf::Color kNodeOutlineColor(240, 240, 240);
inline const sf::Color kEdgeColor(120, 120, 130);
inline const sf::Color kHighlightEdgeColor(231, 76, 60);
inline const sf::Color kCandidateEdgeColor(241, 196, 15);
}  // namespace config
