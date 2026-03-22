#pragma once
#include "../../config/Common.h"

class DSU {
    private:
    std::vector<int> parent;
    public:
    DSU(int n);
    int find(int i);
    bool unite(int i,int j); 
};
