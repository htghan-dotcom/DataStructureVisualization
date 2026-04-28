#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <optional>
#include "WelcomeScreen.h"
#include "SettingsScreen.h"
#include "Menu.h"
<<<<<<< HEAD:src/main.cpp
#include "DoublyLinkedList/DLLVisualizer.h"
#include "HashChaining/HashVisualizer.h"
#include "RedBlackTree/RBTVisualizer.h"
#include "Common.h"
=======
#include "RedBlackTree/RBTVisualizer.h"
#include "config/Common.h"
#include "UI/Visualizer.h" // Nhúng Visualizer của bạn
>>>>>>> MinimumSpanningTree-MNhat:include/src/main.cpp

enum class MainAppState { WELCOME, MENU, SETTINGS, EXPANDING, VISUALIZER, SHRINKING };

sf::View getLetterboxView(sf::View view, unsigned int windowWidth, unsigned int windowHeight){
    if (windowHeight == 0) windowHeight = 1;
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

float smoothLerpApp(float a, float b, float t){
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
    DLLVisualizer dllVisualizer(window);
    HashVisualizer hashVisualizer(window);
    RBTVisualizer rbtVisualizer(window);

    sf::Music bgMusic;
    if (!bgMusic.openFromFile("assets/audio/bg_music.ogg")){
        std::cerr << "Cannot load bg_music.ogg" << std::endl;
    } else {
        bgMusic.setLooping(true);
        bgMusic.setVolume(40.f);
        bgMusic.play();
    }
    
    MainAppState currentState = MainAppState::WELCOME;

    int activeDS = -1;
    float animProgress = 0.f;
    sf::FloatRect startRect;
    sf::FloatRect fullScreenRect(sf::Vector2f(0.f, 0.f), sf::Vector2f(LOGIC_W, LOGIC_H));
    sf::Color activeBgColor;

    while (window.isOpen()){
        sf::Vector2f worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        sf::Vector2i mousePos(static_cast<int>(worldPos.x), static_cast<int>(worldPos.y));

        // Per-frame update for DLL (needed for animation/autoplay)
        if (currentState == AppState::VISUALIZER && activeDS == 0){
            dllVisualizer.update(std::nullopt);
            if (dllVisualizer.checkReturnHome()){
                currentState = AppState::SHRINKING;
                animProgress = 1.0f;
                startRect = mainMenu.getCardBounds(activeDS);
            }
        }
        
<<<<<<< HEAD:src/main.cpp
        while (const auto event = window.pollEvent()){
            if (event->getIf<sf::Event::Closed>()){
=======
        while (const std::optional<sf::Event> event = window.pollEvent()){
            if (event->is<sf::Event::Closed>()){
>>>>>>> MinimumSpanningTree-MNhat:include/src/main.cpp
                window.close();
            }

            if (const auto* resizeEvent = event->getIf<sf::Event::Resized>()){
                mainView = getLetterboxView(mainView, resizeEvent->size.x, resizeEvent->size.y);
                window.setView(mainView);
            }
            
<<<<<<< HEAD:src/main.cpp
            if (currentState == AppState::MENU || currentState == AppState::SETTINGS){
=======
            if (currentState == MainAppState::MENU || currentState == MainAppState::SETTINGS){
>>>>>>> MinimumSpanningTree-MNhat:include/src/main.cpp
                if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()){
                    if (keyEvent->code == sf::Keyboard::Key::Escape){
                        if (currentState == MainAppState::SETTINGS) settingsScreen.mGoBack = false;
                        currentState = MainAppState::WELCOME;
                        welcomeScreen.resetStart();
                    }
                }
            }
            
            if (currentState == MainAppState::WELCOME){
                welcomeScreen.update(mousePos, event);
            }
            else if (currentState == MainAppState::SETTINGS){
                settingsScreen.update(mousePos, event);
<<<<<<< HEAD:src/main.cpp
                            
=======
>>>>>>> MinimumSpanningTree-MNhat:include/src/main.cpp
                if (event && event->is<sf::Event::MouseButtonPressed>()){
                    auto mouseEvent = event->getIf<sf::Event::MouseButtonPressed>();
                    if (mouseEvent->button == sf::Mouse::Button::Left){
                        sf::Vector2f mPos(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
<<<<<<< HEAD:src/main.cpp
                                    
=======
>>>>>>> MinimumSpanningTree-MNhat:include/src/main.cpp
                        sf::FloatRect colorBtnBounds(sf::Vector2f(494.f, 715.f), sf::Vector2f(145.f, 145.f));
                        if (colorBtnBounds.contains(mPos)){
                            settingsScreen.toggleThemePopup();
                        }
                    }
                }
            }
<<<<<<< HEAD:src/main.cpp

            // ── DLL visualizer (DS 0) ──
            if (currentState == AppState::VISUALIZER && activeDS == 0){
                if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()){
                    if (keyEvent->code == sf::Keyboard::Key::Escape){
                        currentState = AppState::SHRINKING;
                        animProgress = 1.0f;
                        startRect = mainMenu.getCardBounds(activeDS);
                        continue;
                    }
                }
                dllVisualizer.update(event);
            }
            
            // ── Hash visualizer (DS 1) ──
            if (currentState == AppState::VISUALIZER && activeDS == 1){
                if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()){
                    if (keyEvent->code == sf::Keyboard::Key::Escape){
                        currentState = AppState::SHRINKING;
                        animProgress = 1.0f;
                        startRect = mainMenu.getCardBounds(activeDS);
                        continue;
                    }
                }
                hashVisualizer.update(event);
                if (hashVisualizer.checkReturnHome()){
                    currentState = AppState::SHRINKING;
                    animProgress = 1.0f;
                    startRect = mainMenu.getCardBounds(activeDS);
                }
            }
            // ── RBT visualizer (DS 2) ──
            if (currentState == AppState::VISUALIZER && activeDS == 2){
=======
            else if (currentState == MainAppState::VISUALIZER && activeDS == 2){
>>>>>>> MinimumSpanningTree-MNhat:include/src/main.cpp
                if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()){
                    if (keyEvent->code == sf::Keyboard::Key::Escape){
                        currentState = MainAppState::SHRINKING;
                        animProgress = 1.0f;
                        startRect = mainMenu.getCardBounds(activeDS);
                        continue;
                    }
                }
                rbtVisualizer.update(event);
                if (rbtVisualizer.checkReturnHome()){
                    currentState = MainAppState::SHRINKING;
                    animProgress = 1.0f;
                    startRect = mainMenu.getCardBounds(activeDS);
                }
            }
        }

        window.clear(sf::Color(40, 40, 40));
        window.setView(mainView);
        
        if (currentState == MainAppState::WELCOME){
            sf::RectangleShape welcomeBg(fullScreenRect.size);
            welcomeBg.setFillColor(ThemeManager::current.screenBg);
            window.draw(welcomeBg);
            
            welcomeScreen.update(mousePos, std::nullopt);
            welcomeScreen.draw(window);
            
            if (welcomeScreen.isStartPressed()){
                currentState = MainAppState::MENU;
            }
            else if (welcomeScreen.isSettingPressed()){
                currentState = MainAppState::SETTINGS;
                welcomeScreen.resetSetting();
            }
        }
        else if (currentState == MainAppState::SETTINGS){
            sf::RectangleShape settingsBg(fullScreenRect.size);
            settingsBg.setFillColor(ThemeManager::current.screenBg);
            window.draw(settingsBg);
            
            settingsScreen.update(mousePos, std::nullopt);
            settingsScreen.draw(window);
            
            if (settingsScreen.mGoBack){
                currentState = MainAppState::WELCOME;
                settingsScreen.mGoBack = false;
            }
        }
        else if (currentState == MainAppState::MENU){
            sf::RectangleShape menuBg(fullScreenRect.size);
            menuBg.setFillColor(ThemeManager::current.screenBg);
            window.draw(menuBg);

            auto selected = mainMenu.update(window);
            mainMenu.draw(window);
            
            if (mainMenu.mGoBack){
                currentState = MainAppState::WELCOME;
                welcomeScreen.resetStart();
                mainMenu.mGoBack = false;
            }
            
<<<<<<< HEAD:src/main.cpp
            // Preview render on menu card hover
            if (mainMenu.getActiveDS() == 0)
                dllVisualizer.render(false);
            else if (mainMenu.getActiveDS() == 1)
                hashVisualizer.render(false);
            else if (mainMenu.getActiveDS() == 2)
                rbtVisualizer.render(false);
            
            if (selected.has_value()){
                int ds = selected.value();
                if (ds == 0 || ds == 1 || ds == 2){
                    activeDS = ds;
                    startRect    = mainMenu.getCardBounds(activeDS);
                    activeBgColor= mainMenu.getCardColor(activeDS);
                    currentState = AppState::EXPANDING;
                    animProgress = 0.f;
                }
=======
            if (selected.has_value()){
                activeDS = selected.value(); // Lưu lại vị trí (2 là RBT, 3 là Graph/MST)
                startRect = mainMenu.getCardBounds(activeDS);
                activeBgColor = mainMenu.getCardColor(activeDS);
                currentState = MainAppState::EXPANDING;
                animProgress = 0.f;
>>>>>>> MinimumSpanningTree-MNhat:include/src/main.cpp
            }
        }
        else if (currentState == MainAppState::EXPANDING || currentState == MainAppState::SHRINKING){
            if (currentState == MainAppState::EXPANDING){
                animProgress += 0.04f;
            } else {
                animProgress -= 0.04f;
            }
            
            if (animProgress >= 1.0f){
                animProgress = 1.0f;
                currentState = MainAppState::VISUALIZER;
            }
            else if (animProgress <= 0.0f){
                animProgress = 0.0f;
<<<<<<< HEAD:src/main.cpp
                currentState = AppState::MENU;
                
                if (activeDS == 0 and dllVisualizer.isEmpty()){
                    dllVisualizer.generateRandomList();
                }
                
                if (activeDS == 2 and rbtVisualizer.isEmpty()){
                    rbtVisualizer.generateRandomTree();
                }
            }
            
            dllVisualizer.setTransitionProgress(animProgress);
            hashVisualizer.setTransitionProgress(animProgress);
            rbtVisualizer.setTransitionProgress(animProgress);
            
            sf::FloatRect currentRect(sf::Vector2f(smoothLerp(startRect.position.x,
                                                              fullScreenRect.position.x,
                                                              animProgress),
                                                   smoothLerp(startRect.position.y,
                                                              fullScreenRect.position.y,
                                                              animProgress)
                                                   ),
                                      sf::Vector2f(smoothLerp(startRect.size.x,
                                                              fullScreenRect.size.x,
                                                              animProgress),
                                                   smoothLerp(startRect.size.y,
                                                              fullScreenRect.size.y,
                                                              animProgress))
                                      );
            
=======
                currentState = MainAppState::MENU;
                if (activeDS == 2 && rbtVisualizer.isEmpty()){
                    rbtVisualizer.generateRandomTree();
                }
            }
                    
            if (activeDS == 2) rbtVisualizer.setTransitionProgress(animProgress);
            
            sf::FloatRect currentRect(
                sf::Vector2f(smoothLerpApp(startRect.position.x, fullScreenRect.position.x, animProgress), smoothLerpApp(startRect.position.y, fullScreenRect.position.y, animProgress)),
                sf::Vector2f(smoothLerpApp(startRect.size.x, fullScreenRect.size.x, animProgress), smoothLerpApp(startRect.size.y, fullScreenRect.size.y, animProgress))
            );

>>>>>>> MinimumSpanningTree-MNhat:include/src/main.cpp
            sf::RectangleShape menuBg(fullScreenRect.size);
            menuBg.setFillColor(ThemeManager::current.screenBg);
            window.draw(menuBg);
            
            sf::RectangleShape animBg(currentRect.size);
            animBg.setPosition(currentRect.position);
            animBg.setFillColor(ThemeManager::current.bg);
            window.draw(animBg);
            
<<<<<<< HEAD:src/main.cpp
            switch (activeDS){
                case 0:
                    dllVisualizer.render(false);
                    break;
                case 1:
                    hashVisualizer.render(false);
                    break;
                case 2:
                    rbtVisualizer.render(false);
                    break;
                case 3:
                    break;
            }
        }
        

        else if (currentState == AppState::VISUALIZER){
            sf::RectangleShape fullBg(fullScreenRect.size);
            fullBg.setFillColor(ThemeManager::current.bg);
            window.draw(fullBg);
            
            switch (activeDS){
                case 0:
                    dllVisualizer.render(true);
                    break;
                case 1:
                    hashVisualizer.render(true);
                    break;
                case 2:
                    rbtVisualizer.render(true);
                    break;
                case 3:
                    break;
=======
            if (activeDS == 2) rbtVisualizer.render(false);
        }
        else if (currentState == MainAppState::VISUALIZER){
            if (activeDS == 2) {
                sf::RectangleShape fullBg(fullScreenRect.size);
                fullBg.setFillColor(ThemeManager::current.bg);
                window.draw(fullBg);
                rbtVisualizer.render(true);
            } 
            else if (activeDS == 3) {
                // Tích hợp: Bàn giao quyền điều khiển window cho Visualizer của bạn
                Visualizer mstVisualizer;
                mstVisualizer.run(window);
                
                // Phục hồi lại mainView sau khi Visualizer có thể đã thay đổi kích thước view
                window.setView(mainView);

                // Khi bạn bấm nút "Back" trong MST Visualizer, vòng lặp run() sẽ kết thúc (return)
                // và luồng code sẽ chạy tiếp tục ở đây, ta thực hiện thu nhỏ màn hình về Menu.
                currentState = MainAppState::SHRINKING;
                animProgress = 1.0f;
                startRect = mainMenu.getCardBounds(activeDS);
>>>>>>> MinimumSpanningTree-MNhat:include/src/main.cpp
            }
        }
        
        window.display();
    }
    return 0;
}
