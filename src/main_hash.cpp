#include <SFML/Graphics.hpp>
#include <iostream>
#include "HashChaining/HashVisualizer.h"
#include "config/Config.h"

int main() {
    // Tạo cửa sổ
    sf::RenderWindow window(
        sf::VideoMode(sf::Vector2u(config::kWindowWidth, config::kWindowHeight)),
        "Hash Chaining Visualizer",
        sf::State::Windowed
    );
    window.setFramerateLimit(60);

    // Tạo visualizer
    HashVisualizer visualizer(window);

    // Vòng lặp chính
    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            if (event->getIf<sf::Event::Closed>()) {
                window.close();
            }
            visualizer.update(event);
        }

        window.clear(sf::Color(210, 230, 250)); // nền pastel nhẹ
        visualizer.render(true);
        window.display();
    }

    return 0;
}
