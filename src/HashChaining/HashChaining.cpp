#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "HashChaining/HashChaining.h"

using namespace std;

// ── Capture current table as snapshot ──────────────────────────
vector<ChainSnapshot> HashChaining::captureSnapshot() const {
    vector<ChainSnapshot> snap(size);
    for (int i = 0; i < size; i++) {
        Node* cur = table[i];
        while (cur) {
            snap[i].vals.push_back(cur->val);
            cur = cur->next;
        }
    }
    return snap;
}

// ── Helper: push a step with current snapshot ──────────────────
// (inline lambda in each method — avoids repetition)
#define PUSH_STEP(desc, lid, bucket, val, fnd) \
    steps.push_back({(desc), (lid), (bucket), (val), (fnd), captureSnapshot()})

// ── Hash function ──────────────────────────────────────────────
int HashChaining::hashFunction(int key) {
    return key % size;
}

// ── Add ────────────────────────────────────────────────────────
void HashChaining::addInternal(int key, bool keepSteps) {
    if (!keepSteps) steps.clear();
    int idx = hashFunction(key);

    PUSH_STEP("Computing hash: " + to_string(key) + " % "
              + to_string(size) + " = " + to_string(idx),
              0, -1, -1, false);
    PUSH_STEP("Go to bucket [" + to_string(idx) + "]",
              1, idx, -1, false);

    // snapshot BEFORE inserting → shows bucket still empty/old
    PUSH_STEP("Inserting " + to_string(key)
              + " at head of bucket [" + to_string(idx) + "]",
              2, idx, -1, false);

    // Actually insert
    Node* p = new Node(key);
    p->next = table[idx];
    table[idx] = p;

    // snapshot AFTER inserting → node is now visible
    PUSH_STEP("Done. Node " + to_string(key) + " added at bucket ["
              + to_string(idx) + "].",
              3, idx, key, false);
}

// ── Delete ─────────────────────────────────────────────────────
bool HashChaining::deleteInternal(int key, bool keepSteps) {
    if (!keepSteps) steps.clear();
    int idx = hashFunction(key);

    PUSH_STEP("Computing hash: " + to_string(key) + " % "
              + to_string(size) + " = " + to_string(idx),
              9, -1, -1, false);
    PUSH_STEP("Go to bucket [" + to_string(idx) + "]",
              10, idx, -1, false);

    if (!table[idx]) {
        PUSH_STEP("Bucket [" + to_string(idx) + "] is empty. Not found.",
                  11, idx, -1, false);
        return false;
    }

    // Delete head
    if (table[idx]->val == key) {
        // snapshot WITH node still present (highlight it)
        PUSH_STEP("Found " + to_string(key) + " at head of bucket. Deleting...",
                  12, idx, key, true);

        Node* tmp = table[idx];
        table[idx] = table[idx]->next;
        delete tmp;

        // snapshot AFTER deletion
        PUSH_STEP("Deleted " + to_string(key) + " successfully.",
                  13, idx, -1, false);
        return true;
    }

    // Traverse
    Node* cur = table[idx];
    while (cur->next && cur->next->val != key) {
        PUSH_STEP("Checking node " + to_string(cur->next->val),
                  14, idx, cur->next->val, false);
        cur = cur->next;
    }

    if (!cur->next) {
        PUSH_STEP("Not found.", 15, idx, -1, false);
        return false;
    }

    PUSH_STEP("Found " + to_string(key) + " in the middle. Unlinking...",
              16, idx, key, true);

    Node* tmp = cur->next;
    cur->next = tmp->next;
    delete tmp;

    PUSH_STEP("Deleted " + to_string(key) + " successfully.",
              13, idx, -1, false);
    return true;
}

// ── Search ─────────────────────────────────────────────────────
int HashChaining::searchImpl(int key) {
    steps.clear();
    int idx = hashFunction(key);

    PUSH_STEP("Hash(" + to_string(key) + ") = " + to_string(idx),
              4, -1, -1, false);
    PUSH_STEP("Go to bucket [" + to_string(idx) + "]",
              5, idx, -1, false);

    Node* cur = table[idx];
    while (cur) {
        PUSH_STEP("Checking node " + to_string(cur->val),
                  6, idx, cur->val, false);
        if (cur->val == key) {
            PUSH_STEP("Found " + to_string(key) + " at bucket ["
                      + to_string(idx) + "]!",
                      7, idx, key, true);
            return idx;
        }
        cur = cur->next;
    }
    PUSH_STEP("Not found.", 8, idx, -1, false);
    return -1;
}

// ── Constructor / Destructor ────────────────────────────────────
HashChaining::HashChaining(int tableSize) : size(tableSize) {
    table.assign(size, nullptr);
}

HashChaining::~HashChaining() { clear(); }

// ── Public API ──────────────────────────────────────────────────
void HashChaining::add(int key)          { addInternal(key, false); }
int  HashChaining::search(int key)       { return searchImpl(key); }
bool HashChaining::deleteNode(int key)   { return deleteInternal(key, false); }

void HashChaining::update(int oldKey, int newKey) {
    steps.clear();
    PUSH_STEP("Updating: delete " + to_string(oldKey) + " first...", 0, -1, -1, false);
    if (deleteInternal(oldKey, true)) {
        PUSH_STEP("Done deleting. Now adding " + to_string(newKey) + "...", 0, -1, -1, false);
        addInternal(newKey, true);
    } else {
        PUSH_STEP("Not found " + to_string(oldKey) + ". Cancel updating.", 8, -1, -1, false);
    }
}

void HashChaining::clear() {
    for (int i = 0; i < size; i++) {
        Node* cur = table[i];
        while (cur) { Node* tmp = cur; cur = cur->next; delete tmp; }
        table[i] = nullptr;
    }
}

void HashChaining::generateRandom(int count) {
    clear();
    steps.clear();
    PUSH_STEP("Generating " + to_string(count) + " random elements...", 0, -1, -1, false);
    for (int i = 0; i < count; i++) {
        int val = rand() % 1000;
        addInternal(val, true);
    }
    PUSH_STEP("Done generating randomly.", -1, -1, -1, false);
}

void HashChaining::loadFromFile(const string& filename) {
    clear();
    steps.clear();
    ifstream fin(filename);
    if (!fin.is_open()) {
        PUSH_STEP("Cannot open file: " + filename, -1, -1, -1, false);
        return;
    }
    PUSH_STEP("Reading file: " + filename, -1, -1, -1, false);
    int val;
    while (fin >> val) addInternal(val, true);
    fin.close();
    PUSH_STEP("Done reading file.", -1, -1, -1, false);
}

vector<Node*>      HashChaining::getTable() const { return table; }
int                HashChaining::getSize()  const { return size; }
vector<HashStepInfo> HashChaining::getSteps() const { return steps; }