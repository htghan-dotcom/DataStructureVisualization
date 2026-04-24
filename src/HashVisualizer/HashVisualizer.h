#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <unordered_map>

#include "HashChaining/HashChaining.h"
#include "../core/Animation.h"
#include "../core/AppState.h"
#include "../UI/components/Button.h"
#include "../UI/components/Slider.h"
#include "../config/Config.h"

class HashVisualizer {
public:
    HashVisualizer(sf::Font& mainFont, sf::Font& monoFont);
    void handleEvent(sf::Event& event, sf::RenderWindow& window);
    void update(sf::Time dt);
    void draw(sf::RenderWindow& window);

private:
    void initLayout();
    void buildPseudocode();
    void syncStepsWithAnimation();

    void renderMenuPanel(sf::RenderWindow& window);
    void renderBuckets(sf::RenderWindow& window);
    void renderNodes(sf::RenderWindow& window, const Step* currentStep);
    void renderPseudocodePanel(sf::RenderWindow& window, const Step* currentStep);
    void renderStepPanel(sf::RenderWindow& window, const Step* currentStep);
    void renderControlBar(sf::RenderWindow& window);

    void drawRoundedRect(sf::RenderWindow& window, const sf::FloatRect& rect,
                         float radius, const sf::Color& fill,
                         const sf::Color& border, float borderThick = 2.f);
    void drawArrowUp(sf::RenderWindow& window, sf::Vector2f from,
                     sf::Vector2f to, const sf::Color& color);

    void onActionRandom();
    void onActionClear();

    // Backend & Animation
    HashChaining hashBackend_;
    Animation animation_;
    sf::Font& font_;
    sf::Font& monoFont_;

    // UI components
    std::vector<Button> menuButtons_;
    std::vector<Button> controlButtons_;
    Button btnChangeCanvas_;
    Button btnMoveControls_;
    Slider speedSlider_;

    // Layout
    sf::FloatRect menuPanelRect_;
    sf::FloatRect stepPanelRect_;
    sf::FloatRect pseudoPanelRect_;
    sf::FloatRect controlBarRect_;
    sf::FloatRect canvasRect_;

    // Bucket layout cache
    float bucketStartX_ = 0.f;
    float bucketRowY_   = 0.f;
    float bucketW_      = 0.f;
    float bucketH_      = 0.f;
    float bucketGapX_   = 0.f;

    std::vector<std::string> pseudocodeLines_;

    bool playing_ = false;
    float speed_  = 1.0f;
    sf::Clock playClock_;

    bool enteringValue_ = false;
    std::string inputBuffer_;
    int pendingAction_ = -1;
    int oldKey_ = -1;

    // W/H display in control bar
    std::string wBuffer_ = "1280";
    std::string hBuffer_ = "720";
    bool editingW_ = false;
    bool editingH_ = false;

    // Pastel node color palettes
    std::vector<std::pair<sf::Color, sf::Color>> colorPalettes_ = {
        {{188, 214, 245, 220}, {210, 230, 255, 220}},
        {{245, 188, 210, 220}, {255, 210, 230, 220}},
        {{188, 240, 210, 220}, {210, 255, 230, 220}},
        {{245, 235, 155, 220}, {255, 248, 190, 220}},
        {{220, 195, 245, 220}, {238, 220, 255, 220}},
        {{245, 210, 180, 220}, {255, 230, 205, 220}},
        {{175, 230, 240, 220}, {205, 245, 255, 220}},
        {{240, 200, 200, 220}, {255, 225, 225, 220}},
    };
    std::unordered_map<int, int> valueToColorIdx_;

    float animationProgress_ = 0.0f;
};