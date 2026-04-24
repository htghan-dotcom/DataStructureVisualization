#include "ThemeManager.h"

bool ThemeManager::isDark = false;

ThemeType ThemeManager::currentType = ThemeType::DEFAULT;

ThemePalette ThemeManager::current = {
    sf::Color(90, 150, 44),
    sf::Color(132, 216, 68),
    sf::Color(217, 217, 217),
    sf::Color(245, 245, 240),
    sf::Color(255, 255, 255),
    sf::Color(0, 0, 0)
};
