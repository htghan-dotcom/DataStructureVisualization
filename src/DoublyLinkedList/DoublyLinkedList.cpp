#include "DoublyLinkedList/DoublyLinkedList.h"
#include <fstream>
#include <iostream>
#include <algorithm>

DoublyLinkedList::DoublyLinkedList() : head(nullptr), tail(nullptr), size(0), mCurrentStep(0), mBackup(nullptr), mVisualizationSpeed(500) {
    mStepHistory.push_back(DLLStepState{});
}

DoublyLinkedList::~DoublyLinkedList() {
    clear();
    if (mBackup) delete mBackup;
}

void DoublyLinkedList::recordStep(const std::string& description) {
    DLLStepState state;
    state.description = description;
    
    Node* current = head;
    while (current) {
        state.nodes.push_back(DLLNodeSnapshot{current->data, false});
        current = current->next;
    }
    
    mStepHistory.push_back(state);
    mCurrentStep = mStepHistory.size() - 1;
}

void DoublyLinkedList::insertTail(int value) {
    Node* newNode = new Node(value);
    if (!head) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
    size++;
    recordStep("Insert " + std::to_string(value) + " at tail");
}

void DoublyLinkedList::insertHead(int value) {
    Node* newNode = new Node(value);
    if (!head) {
        head = tail = newNode;
    } else {
        newNode->next = head;
        head->prev = newNode;
        head = newNode;
    }
    size++;
    recordStep("Insert " + std::to_string(value) + " at head");
}

bool DoublyLinkedList::deleteValue(int value) {
    if (!head) return false;

    Node* current = head;
    while (current) {
        if (current->data == value) {
            if (current == head) {
                head = current->next;
                if (head) head->prev = nullptr;
                else tail = nullptr; 
            } 
            else if (current == tail) {
                tail = current->prev;
                tail->next = nullptr;
            } 
            else {
                current->prev->next = current->next;
                current->next->prev = current->prev;
            }
            
            delete current;
            size--;
            recordStep("Delete " + std::to_string(value));
            return true; 
        }
        current = current->next;
    }
    return false; 
}

int DoublyLinkedList::search(int value) const {
    Node* current = head;
    int index = 0;
    while (current) {
        if (current->data == value) return index;
        current = current->next;
        index++;
    }
    return -1;
}

bool DoublyLinkedList::update(int oldValue, int newValue) {
    Node* current = head;
    while (current) {
        if (current->data == oldValue) {
            current->data = newValue;
            recordStep("Update " + std::to_string(oldValue) + " to " + std::to_string(newValue));
            return true;
        }
        current = current->next;
    }
    return false;
}

bool DoublyLinkedList::initFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    clear();
    int val;
    while (file >> val) {
        insertTail(val);
    }
    file.close();
    return true;
}

void DoublyLinkedList::clear() {
    Node* current = head;
    while (current) {
        Node* nextNode = current->next;
        delete current;
        current = nextNode;
    }
    head = tail = nullptr;
    size = 0;
}

std::vector<int> DoublyLinkedList::getElements() const {
    std::vector<int> elements;
    elements.reserve(size);
    Node* current = head;
    while (current) {
        elements.push_back(current->data);
        current = current->next;
    }
    return elements;
}

int DoublyLinkedList::getSize() const {
    return size;
}

void DoublyLinkedList::initialize() {
    clear();
    mStepHistory.clear();
    mStepHistory.push_back(DLLStepState{});
    mCurrentStep = 0;
}

std::vector<DLLStepState> DoublyLinkedList::getStepHistory() const {
    return mStepHistory;
}

int DoublyLinkedList::getCurrentStep() const {
    return mCurrentStep;
}

void DoublyLinkedList::setCurrentStep(int step) {
    if (step >= 0 && step < static_cast<int>(mStepHistory.size())) {
        mCurrentStep = step;
    }
}

void DoublyLinkedList::resetHistory(const std::string& initialDescription) {
    mStepHistory.clear();
    DLLStepState initialState;
    initialState.description = initialDescription;
    
    Node* current = head;
    while (current) {
        initialState.nodes.push_back(DLLNodeSnapshot{current->data, false});
        current = current->next;
    }
    
    mStepHistory.push_back(initialState);
    mCurrentStep = 0;
}

void DoublyLinkedList::backup() {
    if (mBackup) delete mBackup;
    mBackup = new DoublyLinkedList();
    
    Node* current = head;
    while (current) {
        mBackup->insertTail(current->data);
        current = current->next;
    }
    
    mBackup->mStepHistory = mStepHistory;
    mBackup->mCurrentStep = mCurrentStep;
}

void DoublyLinkedList::restore() {
    if (!mBackup) return;
    
    clear();
    Node* current = mBackup->head;
    while (current) {
        insertTail(current->data);
        current = current->next;
    }
    
    mStepHistory = mBackup->mStepHistory;
    mCurrentStep = mBackup->mCurrentStep;
}

void DoublyLinkedList::goToFinalStep() {
    if (!mStepHistory.empty()) {
        mCurrentStep = mStepHistory.size() - 1;
    }
}

void DoublyLinkedList::setVisualizationSpeed(int ms) {
    mVisualizationSpeed = ms;
}