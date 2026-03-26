#pragma once

struct Edge {
    int from;
    int to;
    int weight;

    bool operator<(const Edge& other) const {
        if (weight != other.weight) {
            return weight < other.weight;
        }
        if (from != other.from) {
            return from < other.from;
        }
        return to < other.to;
    }

    bool operator==(const Edge& other) const {
        return from == other.from && to == other.to && weight == other.weight;
    }
};
