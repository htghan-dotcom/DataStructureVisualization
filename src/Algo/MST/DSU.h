#pragma once

#include "../../config/Common.h"

struct DSUStepMeta {
    int u = -1;
    int v = -1;
    int rootU = -1;
    int rootV = -1;
    bool merged = false;
};

class DSU {
public:
    explicit DSU(int n);

    int find(int i);
    DSUStepMeta inspectAndUnite(int u, int v);

private:
    std::vector<int> parent_;
    std::vector<int> rank_;
};
