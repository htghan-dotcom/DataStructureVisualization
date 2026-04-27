#pragma once
#include <vector>
#include <string>

struct DLLNodeSnapshot {
    int val;
    bool isHighlighted = false;
};

struct DLLStepState {
    std::vector<DLLNodeSnapshot> nodes;
    std::string description;
    std::string pseudoCode;
    std::string cppCode;
    int activeLine = -1;
};