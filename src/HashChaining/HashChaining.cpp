#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "HashChaining/HashChaining.h"

using namespace std;

int HashChaining::hashFunction(int key) {
    return key % size;
}

void HashChaining::addInternal(int key, bool keepSteps) {
    if (!keepSteps) steps.clear();
    int idx = hashFunction(key);

    steps.push_back({"Computing hash: " + to_string(key) + " % "
                    + to_string(size) + " = " + to_string(idx), 0, -1, -1, false});
    steps.push_back({"Go to bucket [" + to_string(idx) + "]", 1, idx, -1, false});
    steps.push_back({"Inserting " + to_string(key)
                    + " at head of bucket [" + to_string(idx) + "]", 2, idx, -1, false});

    Node* p = new Node(key);
    p->next = table[idx];
    table[idx] = p;

    steps.push_back({"Done. Node " + to_string(key) + " added at bucket ["
                    + to_string(idx) + "].", 3, idx, key, false});
}

bool HashChaining::deleteInternal(int key, bool keepSteps) {
    if (!keepSteps) steps.clear();
    int idx = hashFunction(key);

    steps.push_back({"Computing hash: " + to_string(key) + " % "
                    + to_string(size) + " = " + to_string(idx), 9, -1, -1, false});
    steps.push_back({"Go to bucket [" + to_string(idx) + "]", 10, idx, -1, false});

    if (!table[idx]) {
        steps.push_back({"Bucket [" + to_string(idx) + "] is empty. Not found.",
                        11, idx, -1, false});
        return false;
    }

    // Delete head
    if (table[idx]->val == key) {
        steps.push_back({"Found " + to_string(key) + " at head of bucket. Deleting...",
                        12, idx, key, true});
        Node* tmp = table[idx];
        table[idx] = table[idx]->next;
        delete tmp;
        steps.push_back({"Deleted " + to_string(key) + " successfully.", 13, idx, -1, false});
        return true;
    }

    // Traverse
    Node* cur = table[idx];
    while (cur->next && cur->next->val != key) {
        steps.push_back({"Checking node " + to_string(cur->next->val),
                        14, idx, cur->next->val, false});
        cur = cur->next;
    }

    if (!cur->next) {
        steps.push_back({"Not found.", 15, idx, -1, false});
        return false;
    }

    steps.push_back({"Found " + to_string(key) + " in the middle. Unlinking...",
                    16, idx, key, true});
    Node* tmp = cur->next;
    cur->next = tmp->next;
    delete tmp;
    steps.push_back({"Deleted " + to_string(key) + " successfully.", 13, idx, -1, false});
    return true;
}

int HashChaining::searchImpl(int key) {
    steps.clear();
    int idx = hashFunction(key);

    steps.push_back({"Hash(" + to_string(key) + ") = " + to_string(idx),
                    4, -1, -1, false});
    steps.push_back({"Go to bucket [" + to_string(idx) + "]", 5, idx, -1, false});

    Node* cur = table[idx];
    while (cur) {
        steps.push_back({"Checking node " + to_string(cur->val),
                        6, idx, cur->val, false});
        if (cur->val == key) {
            steps.push_back({"Found " + to_string(key) + " at bucket ["
                            + to_string(idx) + "]!", 7, idx, key, true});
            return idx;
        }
        cur = cur->next;
    }
    steps.push_back({"Not found.", 8, idx, -1, false});
    return -1;
}

HashChaining::HashChaining(int tableSize) : size(tableSize) {
    table.assign(size, nullptr);
}

HashChaining::~HashChaining() {
    clear();
}

void HashChaining::add(int key) {
    addInternal(key, false); 
}
int  HashChaining::search(int key) { 
    return searchImpl(key); 
}
bool HashChaining::deleteNode(int key) { 
    return deleteInternal(key, false); 
}

void HashChaining::update(int oldKey, int newKey) {
    steps.clear();
    steps.push_back({"Updating: delete " + to_string(oldKey) + " first...",
                    0, -1, -1, false});
    if (deleteInternal(oldKey, true)) {
        steps.push_back({"Done deleting. Now adding " + to_string(newKey) + "...",
                        0, -1, -1, false});
        addInternal(newKey, true);
    } else {
        steps.push_back({"Not found " + to_string(oldKey) + ". Cancel updating.",
                        8, -1, -1, false});
    }
}

void HashChaining::clear() {
    for (int i = 0; i < size; i++) {
        Node* cur = table[i];
        while (cur) {
            Node* tmp = cur;
            cur = cur->next;
            delete tmp;
        }
        table[i] = nullptr;
    }
}

void HashChaining::generateRandom(int count) {
    clear();
    steps.clear();
    steps.push_back({"Generating " + to_string(count) + " random elements...",
                    0, -1, -1, false});
    for (int i = 0; i < count; i++) {
        int val = rand() % 1000;
        addInternal(val, true);
    }
    steps.push_back({"Done generating randomly.", -1, -1, -1, false});
}

void HashChaining::loadFromFile(const string& filename) {
    clear();
    steps.clear();
    ifstream fin(filename);
    if (!fin.is_open()) {
        steps.push_back({"Cannot open file: " + filename, -1, -1, -1, false});
        return;
    }
    steps.push_back({"Reading file: " + filename, -1, -1, -1, false});
    int val;
    while (fin >> val) addInternal(val, true);
    fin.close();
    steps.push_back({"Done reading file.", -1, -1, -1, false});
}

vector<Node*> HashChaining::getTable() const { return table; }
int HashChaining::getSize()  const { return size; }
vector<HashStepInfo> HashChaining::getSteps() const { return steps; }