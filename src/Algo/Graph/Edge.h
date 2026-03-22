#pragma once

struct Edge{
    int from;
    int to;
    int weight;
    Edge(int u,int v,int w){
        from=u;
        to=v;
        weight=w;
    }
    bool operator < (const Edge& other) const{
        return weight < other.weight;
    }
};