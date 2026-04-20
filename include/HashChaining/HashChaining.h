#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

struct Node{
    int val;
    Node* next;
    Node(int x){
        val = x;
        next = nullptr;
    }
};

struct HashStepInfo{
    string description; // for user
    int lineID; // the code line needs highlighted (-1 = no highlight)
    int targetBucket; // focused bucket (-1 = none)
    int targetVal; // focused node value (-1 = none)
    bool found; // found or not
};

class HashChaining {
private:
    int size;
    vector<Node*> table;
    vector<HashStepInfo> steps;

    int hashFunction(int key);
    void addInternal(int key, bool keepSteps);
    bool deleteInternal(int key, bool keepSteps);
    int  searchImpl(int key);

public:
    HashChaining(int tableSize);
    ~HashChaining();

    void add(int key);
    int  search(int key);
    bool deleteNode(int key);
    void update(int oldKey, int newKey);

    void clear();
    void generateRandom(int count);
    void loadFromFile(const string& filename);


    vector<Node*> getTable() const;
    int getSize() const;
    vector<HashStepInfo> getSteps() const;
};
