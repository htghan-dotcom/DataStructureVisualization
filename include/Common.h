#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <thread>
#include <chrono>
#include <algorithm>
#include <set>
#include <functional>
#include <queue>
#include <cmath>

using namespace std;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

inline sf::ConvexShape createRoundedRect(sf::Vector2f size, float radius){
    radius = std::min(radius, std::min(size.x / 2.0f, size.y / 2.0f));
    int pointsPerCorner = 15;
    sf::ConvexShape shape(pointsPerCorner * 4);
    int index = 0;
    
    for (int i = 0; i < pointsPerCorner; ++i){
        float angle = i * (M_PI / 2) / (pointsPerCorner - 1);
        shape.setPoint(index++, sf::Vector2f(size.x - radius + radius * sin(angle), radius - radius * cos(angle)));
    }
    
    for (int i = 0; i < pointsPerCorner; ++i){
        float angle = M_PI / 2 + i * (M_PI / 2) / (pointsPerCorner - 1);
        shape.setPoint(index++, sf::Vector2f(size.x - radius + radius * sin(angle), size.y - radius - radius * cos(angle)));
    }
    
    for (int i = 0; i < pointsPerCorner; ++i){
        float angle = M_PI + i * (M_PI / 2) / (pointsPerCorner - 1);
        shape.setPoint(index++, sf::Vector2f(radius + radius * sin(angle), size.y - radius - radius * cos(angle)));
    }
    
    for (int i = 0; i < pointsPerCorner; ++i){
        float angle = 3 * M_PI / 2 + i * (M_PI / 2) / (pointsPerCorner - 1);
        shape.setPoint(index++, sf::Vector2f(radius + radius * sin(angle), radius - radius * cos(angle)));
    }
    
    return shape;
}
