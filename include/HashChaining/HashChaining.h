#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

struct HashNode{
    int val;
    HashNode* next;
    HashNode(int x){
        val = x;
        next = nullptr;
    }
};

// Snapshot of one bucket chain at a point in time
struct ChainSnapshot {
    vector<int> vals;   // head -> tail order
};

struct HashStepInfo{
    string description;
    int lineID;
    int targetBucket;   // highlighted bucket (-1 = none)
    int targetVal;      // highlighted node value (-1 = none)
    bool found;

    // Snapshot of the entire table at this step
    vector<ChainSnapshot> tableSnapshot;
};

class HashChaining {
private:
    int size;
    vector<HashNode*> table;
    vector<HashStepInfo> steps;
    vector<vector<vector<int>>> history;

    int hashFunction(int key);
    bool checkDuplicate(int key, int idx);
    void addInternal(int key, bool keepSteps);
    bool deleteInternal(int key, bool keepSteps);
    int  searchImpl(int key);

    // Capture current table state into a snapshot
    vector<ChainSnapshot> captureSnapshot() const;

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
    
    void clearTableUI();


    vector<HashNode*> getTable() const;
    int getSize() const;
    vector<HashStepInfo> getSteps() const;

    void saveState();
    void undo();
    bool canUndo() const { return !history.empty(); }
};
