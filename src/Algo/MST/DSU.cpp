#include"DSU.h"
DSU::DSU(int n) {
    parent.resize(n);
    for (int i = 0; i < n; i++) {
        parent[i] = i; 
    }
}

int DSU::find(int i) { 
    if (parent[i] == i)
        return i;
    return parent[i] = find(parent[i]); 
}

bool DSU::unite(int i, int j) {
    int root_i = find(i);
    int root_j = find(j);

    if (root_i != root_j) {
        parent[root_i] = root_j; 
        return true;
    }
    return false; 
}