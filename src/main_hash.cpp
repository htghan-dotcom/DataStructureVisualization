#include <SFML/Graphics.hpp>
#include <iostream>
#include "HashVisualizer/HashVisualizer.h"
#include "config/Config.h"

int main() {
    // Tạo cửa sổ
    sf::RenderWindow window(
        sf::VideoMode(config::kWindowWidth, config::kWindowHeight),
        "Hash Chaining Visualizer",
        sf::Style::Close
    );
    window.setFramerateLimit(60);

    // Load fonts
    sf::Font mainFont, monoFont;

    // Đường dẫn font chính xác hơn
    const std::string interPath = "assets/fonts/Inter Font Family/Inter Font Family/Inter/Inter-Regular.otf";
    const std::string jetbrainsPath = "assets/fonts/JetBrainsMono-2.304/fonts/ttf/JetBrainsMono-Regular.ttf";
    const std::string arialPath = "../assets/fonts/arial.ttf";

    // Load Inter font
    if (!mainFont.loadFromFile(interPath)) {
        std::cerr << "Failed to load Inter font, trying JetBrainsMono...\n";
        if (!mainFont.loadFromFile(jetbrainsPath)) {
            std::cerr << "Failed to load JetBrainsMono, trying Arial...\n";
            if (!mainFont.loadFromFile(arialPath)) {
                std::cerr << "Failed to load all fonts, using SFML default.\n";
            }
        }
    }

    // Load mono font
    if (!monoFont.loadFromFile(jetbrainsPath)) {
        std::cerr << "Failed to load mono font, fallback to main font.\n";
        monoFont = mainFont;
    }

    // Tạo visualizer
    HashVisualizer visualizer(mainFont, monoFont);

    // Clock cho delta time
    sf::Clock frameClock;

    // Vòng lặp chính
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            visualizer.handleEvent(event, window);
        }

        sf::Time deltaTime = frameClock.restart();
        visualizer.update(deltaTime);

        window.clear(sf::Color(210, 230, 250)); // nền pastel nhẹ
        visualizer.draw(window);
        window.display();
    }

    return 0;
}
