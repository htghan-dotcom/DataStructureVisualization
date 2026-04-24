#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "WelcomeScreen.h"
#include "SettingsScreen.h"
#include "Menu.h"
#include "RBTVisualizer.h"
#include "Common.h"

enum class AppState { WELCOME, MENU, SETTINGS, EXPANDING, VISUALIZER, SHRINKING };

sf::View getLetterboxView(sf::View view, unsigned int windowWidth, unsigned int windowHeight){
    float windowRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
    float viewRatio = view.getSize().x / view.getSize().y;
    float sizeX = 1.0f, sizeY = 1.0f;
    float posX = 0.0f, posY = 0.0f;

    if (windowRatio > viewRatio){
        sizeX = viewRatio / windowRatio;
        posX = (1.0f - sizeX) / 2.0f;
    } else {
        sizeY = windowRatio / viewRatio;
        posY = (1.0f - sizeY) / 2.0f;
    }

    view.setViewport(sf::FloatRect(sf::Vector2f(posX, posY), sf::Vector2f(sizeX, sizeY)));
    return view;
}

float smoothLerp(float a, float b, float t){
    float easeT = t * t * (3.0f - 2.0f * t);
    return a + (b - a) * easeT;
}

int main(){
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    unsigned int winW = desktop.size.x;
    unsigned int winH = desktop.size.y - 80;
    
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(winW, winH)), "Data Structure Simulator", sf::State::Windowed, settings);
    window.setFramerateLimit(60);

    const float LOGIC_W = 1440.f;
    const float LOGIC_H = 960.f;
    
    sf::View mainView(sf::FloatRect(sf::Vector2f(0.f, 0.f), sf::Vector2f(LOGIC_W, LOGIC_H)));
    mainView = getLetterboxView(mainView, window.getSize().x, window.getSize().y);
    window.setView(mainView);
    
    WelcomeScreen welcomeScreen(LOGIC_W, LOGIC_H);
    MainMenu mainMenu(LOGIC_W, LOGIC_H);
    SettingsScreen settingsScreen(LOGIC_W, LOGIC_H);
    RBTVisualizer rbtVisualizer(window);
    
    sf::Music bgMusic;
    if (!bgMusic.openFromFile("assets/audio/bg_music.ogg")){
        cerr << "Cannot load bg_music.ogg" << endl;
    } else {
        bgMusic.setLooping(true);
        bgMusic.setVolume(40.f);
        bgMusic.play();
    }
    
    AppState currentState = AppState::WELCOME;

    int activeDS = -1;
    float animProgress = 0.f;
    sf::FloatRect startRect;
    sf::FloatRect fullScreenRect(sf::Vector2f(0.f, 0.f), sf::Vector2f(LOGIC_W, LOGIC_H));
    sf::Color activeBgColor;
    
    while (window.isOpen()){
        sf::Vector2f worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        sf::Vector2i mousePos(static_cast<int>(worldPos.x), static_cast<int>(worldPos.y));
        
        while (const optional event = window.pollEvent()){
            if (event->is<sf::Event::Closed>()){
                window.close();
            }

            if (const auto* resizeEvent = event->getIf<sf::Event::Resized>()){
                mainView = getLetterboxView(mainView, resizeEvent->size.x, resizeEvent->size.y);
                window.setView(mainView);
            }
            
            if (currentState == AppState::MENU or currentState == AppState::SETTINGS){
                if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()){
                    if (keyEvent->code == sf::Keyboard::Key::Escape){
                        currentState = AppState::WELCOME;
                        welcomeScreen.resetStart();
                        if (currentState == AppState::SETTINGS) settingsScreen.mGoBack = false;
                    }
                }
            }
            
            if (currentState == AppState::WELCOME){
                welcomeScreen.update(mousePos, event);
            }
            
            else if (currentState == AppState::SETTINGS){
                settingsScreen.update(mousePos, event);
                            
                if (event and event->is<sf::Event::MouseButtonPressed>()){
                    auto mouseEvent = event->getIf<sf::Event::MouseButtonPressed>();
                    if (mouseEvent->button == sf::Mouse::Button::Left){
                        sf::Vector2f mPos(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
                                    
                        // Nếu bấm vào nút mBtnColor (Theme) thì bật Popup
                        sf::FloatRect colorBtnBounds(sf::Vector2f(494.f, 715.f), sf::Vector2f(145.f, 145.f));
                        if (colorBtnBounds.contains(mPos)){
                            settingsScreen.toggleThemePopup();
                        }
                                    
                    }
                }
            }
            
            else if (currentState == AppState::VISUALIZER and activeDS == 2){
                if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()){
                    if (keyEvent->code == sf::Keyboard::Key::Escape){
                        currentState = AppState::SHRINKING;
                        animProgress = 1.0f;
                        startRect = mainMenu.getCardBounds(activeDS);
                        continue;
                    }
                }
                            
                rbtVisualizer.update(event);
                            
                if (rbtVisualizer.checkReturnHome()){
                    currentState = AppState::SHRINKING;
                    animProgress = 1.0f;
                    startRect = mainMenu.getCardBounds(activeDS);
                }
            }
        }

        window.clear(sf::Color(40, 40, 40));
        window.setView(mainView);
        
        if (currentState == AppState::WELCOME){
            sf::RectangleShape welcomeBg(fullScreenRect.size);
            
            welcomeBg.setFillColor(ThemeManager::current.screenBg);
            window.draw(welcomeBg);
            
            welcomeScreen.update(mousePos, nullopt);
            welcomeScreen.draw(window);
            
            if (welcomeScreen.isStartPressed()){
                currentState = AppState::MENU;
            }

            else if (welcomeScreen.isSettingPressed()){
                currentState = AppState::SETTINGS;
                welcomeScreen.resetSetting();
            }
        }

        else if (currentState == AppState::SETTINGS){
            sf::RectangleShape settingsBg(fullScreenRect.size);
            settingsBg.setFillColor(ThemeManager::current.screenBg);
            window.draw(settingsBg);
            
            settingsScreen.update(mousePos, nullopt);
            settingsScreen.draw(window);
            
            if (settingsScreen.mGoBack){
                currentState = AppState::WELCOME;
                settingsScreen.mGoBack = false;
            }
        }

        else if (currentState == AppState::MENU){
            sf::RectangleShape menuBg(fullScreenRect.size);
            menuBg.setFillColor(ThemeManager::current.screenBg);
            window.draw(menuBg);

            auto selected = mainMenu.update(window);
            mainMenu.draw(window);
            
            if (mainMenu.mGoBack){
                currentState = AppState::WELCOME;
                welcomeScreen.resetStart();
                mainMenu.mGoBack = false;
            }
                    
            if (mainMenu.getActiveDS() == 2){
                rbtVisualizer.render(false);
            }
            
            if (selected.has_value() and selected.value() == 2){
                activeDS = 2;
                startRect = mainMenu.getCardBounds(activeDS);
                activeBgColor = mainMenu.getCardColor(activeDS);
                currentState = AppState::EXPANDING;
                animProgress = 0.f;
            }
        }
        
        else if (currentState == AppState::EXPANDING or currentState == AppState::SHRINKING){
            if (currentState == AppState::EXPANDING){
                animProgress += 0.04f;
            } else {
                animProgress -= 0.04f;
            }

            if (animProgress >= 1.0f){
                animProgress = 1.0f;
                currentState = AppState::VISUALIZER;
            }
            else if (animProgress <= 0.0f){
                animProgress = 0.0f;
                currentState = AppState::MENU;
                
                if (activeDS == 2 and rbtVisualizer.isEmpty()){
                    rbtVisualizer.generateRandomTree();
                }
            }
                    
            rbtVisualizer.setTransitionProgress(animProgress);
            
            sf::FloatRect currentRect(
                sf::Vector2f(smoothLerp(startRect.position.x, fullScreenRect.position.x, animProgress), smoothLerp(startRect.position.y, fullScreenRect.position.y, animProgress)),
                sf::Vector2f(smoothLerp(startRect.size.x, fullScreenRect.size.x, animProgress), smoothLerp(startRect.size.y, fullScreenRect.size.y, animProgress))
            );

            sf::RectangleShape menuBg(fullScreenRect.size);
            menuBg.setFillColor(ThemeManager::current.screenBg);
            window.draw(menuBg);

            sf::RectangleShape animBg(currentRect.size);
            animBg.setPosition(currentRect.position);
            animBg.setFillColor(ThemeManager::current.bg);
            window.draw(animBg);
            
            rbtVisualizer.render(false);
        }
        
        else if (currentState == AppState::VISUALIZER){
            sf::RectangleShape fullBg(fullScreenRect.size);
            fullBg.setFillColor(ThemeManager::current.bg);
            window.draw(fullBg);
            
            rbtVisualizer.render(true);
        }
        
        window.display();
    }
    return 0;
}

#include "UI/Visualizer.h"

int main() {
    Visualizer visualizer;
    visualizer.run();
    return 0;
}
