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
    // Step 1: Create new node at temporary position (outside the list)
    DLLStepState step1;
    step1.description = "Create new node with value " + std::to_string(value);
    step1.pseudoCode = "newNode = createNode(value)\nnewNode.next = null\nnewNode.prev = null";
    step1.cppCode = "Node* newNode = new Node(value);\nnewNode->next = nullptr;\nnewNode->prev = nullptr;";
    step1.activeLine = 0;
    
    // Add existing nodes
    Node* current = head;
    while (current) {
        step1.nodes.push_back(DLLNodeSnapshot{current->data, false});
        current = current->next;
    }
    // Add new node at temporary position (will be highlighted)
    step1.nodes.push_back(DLLNodeSnapshot{value, true});
    
    mStepHistory.push_back(step1);
    
    // Step 2: If list is empty, just set as head and tail
    if (!head) {
        Node* newNode = new Node(value);
        head = tail = newNode;
        size++;
        
        DLLStepState step2;
        step2.description = "List is empty, set as head and tail";
        step2.pseudoCode = "if head == null:\n    head = newNode\n    tail = newNode";
        step2.cppCode = "if (!head) {\n    head = tail = newNode;\n}";
        step2.activeLine = 0;
        step2.nodes.push_back(DLLNodeSnapshot{value, true});
        
        mStepHistory.push_back(step2);
        mCurrentStep = mStepHistory.size() - 1;
        return;
    }
    
    // Step 3: Traverse to find tail (highlight each node during traversal)
    current = head;
    int nodeIndex = 0;
    while (current != tail) {
        DLLStepState traverseStep;
        traverseStep.description = "Traverse to tail - checking node " + std::to_string(nodeIndex);
        traverseStep.pseudoCode = "current = head\nwhile current.next != null:\n    current = current.next";
        traverseStep.cppCode = "Node* current = head;\nwhile (current->next) {\n    current = current->next;\n}";
        traverseStep.activeLine = 1;
        
        // Add all nodes, highlight current traversal node
        Node* temp = head;
        int tempIndex = 0;
        while (temp) {
            traverseStep.nodes.push_back(DLLNodeSnapshot{temp->data, (tempIndex == nodeIndex)});
            temp = temp->next;
            tempIndex++;
        }
        // Add new node (not highlighted during traversal)
        traverseStep.nodes.push_back(DLLNodeSnapshot{value, false});
        
        mStepHistory.push_back(traverseStep);
        current = current->next;
        nodeIndex++;
    }
    
    // Step 4: Connect tail to new node
    DLLStepState connectStep;
    connectStep.description = "Connect tail to new node";
    connectStep.pseudoCode = "tail.next = newNode\nnewNode.prev = tail\ntail = newNode";
    connectStep.cppCode = "tail->next = newNode;\nnewNode->prev = tail;\ntail = newNode;";
    connectStep.activeLine = 0;
    
    // Add all existing nodes, highlight the tail
    current = head;
    nodeIndex = 0;
    while (current) {
        connectStep.nodes.push_back(DLLNodeSnapshot{current->data, (current == tail)});
        current = current->next;
        nodeIndex++;
    }
    // Add new node (highlighted as it's being connected)
    connectStep.nodes.push_back(DLLNodeSnapshot{value, true});
    
    mStepHistory.push_back(connectStep);
    
    // Actually perform the insertion
    Node* newNode = new Node(value);
    tail->next = newNode;
    newNode->prev = tail;
    tail = newNode;
    size++;
    
    // Step 5: Final state
    DLLStepState finalStep;
    finalStep.description = "Insertion complete - " + std::to_string(value) + " added at tail";
    finalStep.pseudoCode = "Insertion completed successfully";
    finalStep.cppCode = "// Node inserted at tail";
    finalStep.activeLine = -1;
    
    current = head;
    while (current) {
        finalStep.nodes.push_back(DLLNodeSnapshot{current->data, false});
        current = current->next;
    }
    
    mStepHistory.push_back(finalStep);
    mCurrentStep = mStepHistory.size() - 1;
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
    if (!head) {
        DLLStepState emptyStep;
        emptyStep.description = "Cannot delete - list is empty";
        emptyStep.pseudoCode = "if head == null:\n    return false";
        emptyStep.cppCode = "if (!head) return false;";
        emptyStep.activeLine = 0;
        mStepHistory.push_back(emptyStep);
        mCurrentStep = mStepHistory.size() - 1;
        return false;
    }

    int foundIndex = search(value);
    if (foundIndex < 0) {
        return false;
    }

    Node* target = head;
    for (int i = 0; i < foundIndex; ++i) {
        target = target->next;
    }

    DLLStepState deleteStep;
    if (target == head && target == tail) {
        deleteStep.description = "Deleting the only node in the list";
        deleteStep.pseudoCode = "head = null\ntail = null";
        deleteStep.cppCode = "head = tail = nullptr;";
        deleteStep.activeLine = 0;
    } else if (target == head) {
        deleteStep.description = "Deleting head node";
        deleteStep.pseudoCode = "head = head.next\nhead.prev = null";
        deleteStep.cppCode = "head = head->next;\nif (head) head->prev = nullptr;";
        deleteStep.activeLine = 0;
    } else if (target == tail) {
        deleteStep.description = "Deleting tail node";
        deleteStep.pseudoCode = "tail = tail.prev\ntail.next = null";
        deleteStep.cppCode = "tail = tail->prev;\ntail->next = nullptr;";
        deleteStep.activeLine = 0;
    } else {
        deleteStep.description = "Deleting middle node";
        deleteStep.pseudoCode = "target.prev.next = target.next\ntarget.next.prev = target.prev";
        deleteStep.cppCode = "target->prev->next = target->next;\ntarget->next->prev = target->prev;";
        deleteStep.activeLine = 0;
    }

    Node* temp = head;
    while (temp) {
        deleteStep.nodes.push_back(DLLNodeSnapshot{temp->data, temp == target});
        temp = temp->next;
    }
    mStepHistory.push_back(deleteStep);

    if (target == head && target == tail) {
        delete target;
        head = tail = nullptr;
    } else if (target == head) {
        head = target->next;
        if (head) head->prev = nullptr;
        delete target;
    } else if (target == tail) {
        tail = target->prev;
        if (tail) tail->next = nullptr;
        delete target;
    } else {
        target->prev->next = target->next;
        target->next->prev = target->prev;
        delete target;
    }

    size--;

    DLLStepState finalStep;
    finalStep.description = "Deletion complete - " + std::to_string(value) + " removed";
    finalStep.pseudoCode = "Deletion completed successfully";
    finalStep.cppCode = "// Node deleted";
    finalStep.activeLine = -1;
    
    temp = head;
    while (temp) {
        finalStep.nodes.push_back(DLLNodeSnapshot{temp->data, false});
        temp = temp->next;
    }
    mStepHistory.push_back(finalStep);
    mCurrentStep = static_cast<int>(mStepHistory.size()) - 1;
    return true;
}

int DoublyLinkedList::search(int value) {
    const Node* left = head;
    const Node* right = tail;
    int leftIndex = 0;
    int rightIndex = size - 1;

    while (left && right && leftIndex <= rightIndex) {
        DLLStepState searchStep;
        searchStep.description = "Searching for value " + std::to_string(value);
        searchStep.pseudoCode = "left = head\nright = tail\nwhile left != null and right != null and leftIndex <= rightIndex:\n    if left.data == x: return left\n    if right.data == x: return right\n    left = left.next\n    right = right.prev";
        searchStep.cppCode = "const Node* left = head;\nconst Node* right = tail;\nint leftIndex = 0;\nint rightIndex = size - 1;\nwhile (left && right && leftIndex <= rightIndex) {\n    if (left->data == value) return leftIndex;\n    if (right->data == value) return rightIndex;\n    left = left->next;\n    right = right->prev;\n    leftIndex++;\n    rightIndex--;\n}";
        searchStep.activeLine = 1;

        const Node* temp = head;
        int idx = 0;
        while (temp) {
            bool highlighted = (idx == leftIndex) || (idx == rightIndex);
            searchStep.nodes.push_back(DLLNodeSnapshot{temp->data, highlighted});
            temp = temp->next;
            idx++;
        }

        mStepHistory.push_back(searchStep);

        if (left->data == value) {
            DLLStepState successStep;
            successStep.description = std::to_string(value) + " is successfully searched at index " + std::to_string(leftIndex);
            successStep.pseudoCode = "Found at left pointer";
            successStep.cppCode = "// Value found at left pointer";
            successStep.activeLine = -1;
            
            const Node* tempNode = head;
            int tempIdx = 0;
            while (tempNode) {
                successStep.nodes.push_back(DLLNodeSnapshot{tempNode->data, (tempIdx == leftIndex)});
                tempNode = tempNode->next;
                tempIdx++;
            }
            
            mStepHistory.push_back(successStep);
            mCurrentStep = static_cast<int>(mStepHistory.size()) - 1;
            return leftIndex;
        }
        if (right->data == value) {
            DLLStepState successStep;
            successStep.description = std::to_string(value) + " is successfully searched at index " + std::to_string(rightIndex);
            successStep.pseudoCode = "Found at right pointer";
            successStep.cppCode = "// Value found at right pointer";
            successStep.activeLine = -1;
            
            const Node* tempNode = head;
            int tempIdx = 0;
            while (tempNode) {
                successStep.nodes.push_back(DLLNodeSnapshot{tempNode->data, (tempIdx == rightIndex)});
                tempNode = tempNode->next;
                tempIdx++;
            }
            
            mStepHistory.push_back(successStep);
            mCurrentStep = static_cast<int>(mStepHistory.size()) - 1;
            return rightIndex;
        }
        if (left == right || left->prev == right) {
            break;
        }
        left = left->next;
        right = right->prev;
        leftIndex++;
        rightIndex--;
    }

    DLLStepState notFoundStep;
    notFoundStep.description = "Value " + std::to_string(value) + " is not found";
    notFoundStep.pseudoCode = "Value x is not found";
    notFoundStep.cppCode = "// Value not found";
    notFoundStep.activeLine = -1;

    const Node* temp = head;
    while (temp) {
        notFoundStep.nodes.push_back(DLLNodeSnapshot{temp->data, false});
        temp = temp->next;
    }

    mStepHistory.push_back(notFoundStep);
    mCurrentStep = static_cast<int>(mStepHistory.size()) - 1;
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