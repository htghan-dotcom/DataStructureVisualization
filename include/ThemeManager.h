#pragma once
#include <SFML/Graphics.hpp>

enum class ThemeType { DEFAULT, PINKY, NAVY, GOLDEN };

struct ThemePalette {
    sf::Color primary;
    sf::Color primaryLight;
    sf::Color secondary;
    sf::Color bg;
    sf::Color screenBg;
    sf::Color textColor;
};

class ThemeManager {
public:
    static ThemePalette current;
    static ThemeType currentType;
    static bool isDark;
    
    static void setTheme(ThemeType type){
        currentType = type;
        
        if (!isDark){
            // ================= LIGHT MODE =================
            if (type == ThemeType::DEFAULT){
                current = {
                    sf::Color(90, 150, 44),
                    sf::Color(132, 216, 68),
                    sf::Color(217, 217, 217),
                    sf::Color(245, 245, 240),
                    sf::Color(255, 255, 255),
                    sf::Color(41, 42, 49)
                };
            }
            
            else if (type == ThemeType::PINKY){
                current = {
                    sf::Color(232, 62, 140),
                    sf::Color(247, 131, 172),
                    sf::Color(217, 217, 217),
                    sf::Color(255, 245, 248),
                    sf::Color(255, 255, 255),
                    sf::Color(41, 42, 49)
                };
            }
            
            else if (type == ThemeType::NAVY){
                current = {
                    sf::Color(30, 58, 138),
                    sf::Color(59, 130, 246),
                    sf::Color(217, 217, 217),
                    sf::Color(248, 250, 252),
                    sf::Color(255, 255, 255),
                    sf::Color(41, 42, 49)
                };
            }
            
            else if (type == ThemeType::GOLDEN){
                current = {
                    sf::Color(245, 158, 11),
                    sf::Color(252, 211, 77),
                    sf::Color(217, 217, 217),
                    sf::Color(255, 253, 240),
                    sf::Color(255, 255, 255),
                    sf::Color(41, 42, 49)
                };
            }
        } else {
            // ================= DARK MODE =================
            if (type == ThemeType::DEFAULT){
                current = {
                    sf::Color(110, 176, 58),
                    sf::Color(132, 216, 68),
                    sf::Color(67, 66, 67),
                    sf::Color(32, 39, 34),
                    sf::Color(41, 42, 49),
                    sf::Color(255, 255, 255)
                };
            }
            
            else if (type == ThemeType::PINKY){
                current = {
                    sf::Color(213, 63, 140),
                    sf::Color(247, 131, 172),
                    sf::Color(67, 66, 67),
                    sf::Color(45, 27, 38),
                    sf::Color(41, 42, 49),
                    sf::Color(255, 255, 255)
                };
            }
            
            else if (type == ThemeType::NAVY){
                current = {
                    sf::Color(59, 130, 246),
                    sf::Color(59, 130, 246),
                    sf::Color(67, 66, 67),
                    sf::Color(30, 41, 59),
                    sf::Color(41, 42, 49),
                    sf::Color(255, 255, 255)
                };
            } else if (type == ThemeType::GOLDEN){
                current = {
                    sf::Color(249, 115, 22),
                    sf::Color(252, 211, 77),
                    sf::Color(67, 66, 67),
                    sf::Color(44, 36, 27),
                    sf::Color(41, 42, 49),
                    sf::Color(255, 255, 255)
                };
            }
        }
    }
};
