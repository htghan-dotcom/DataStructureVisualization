#include "DSU.h"

DSU::DSU(int n) : parent_(n), rank_(n, 0) {
    for (int i = 0; i < n; ++i) {
        parent_[i] = i;
    }
}

int DSU::find(int i) {
    if (parent_[i] == i) {
        return i;
    }
    parent_[i] = find(parent_[i]);
    return parent_[i];
}

DSUStepMeta DSU::inspectAndUnite(int u, int v) {
    const int rootU = find(u);
    const int rootV = find(v);

    DSUStepMeta meta;
    meta.u = u;
    meta.v = v;
    meta.rootU = rootU;
    meta.rootV = rootV;

    if (rootU == rootV) {
        meta.merged = false;
        return meta;
    }

    if (rank_[rootU] < rank_[rootV]) {
        parent_[rootU] = rootV;
    } else if (rank_[rootU] > rank_[rootV]) {
        parent_[rootV] = rootU;
    } else {
        parent_[rootV] = rootU;
        ++rank_[rootU];
    }

    meta.merged = true;
    return meta;
}
