#include "DoublyLinkedList.h"
#include <fstream>
#include <iostream>

DoublyLinkedList::DoublyLinkedList() : head(nullptr), tail(nullptr), size(0) {}

DoublyLinkedList::~DoublyLinkedList() {
    clear(); //clear linked list
}

//insertion at tail of list
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
}

//insertion at head of list
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
}

//deletion function, return true if delete successfully, false if value not exist
bool DoublyLinkedList::deleteValue(int value) {
    if (!head) return false;

    Node* current = head;
    while (current) {
        if (current->data == value) {
            // Nếu là Node đầu
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
            return true; 
        }
        current = current->next;
    }
    return false; 
}

//search value, return index
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

//update old value
bool DoublyLinkedList::update(int oldValue, int newValue) {
    Node* current = head;
    while (current) {
        if (current->data == oldValue) {
            current->data = newValue;
            return true;
        }
        current = current->next;
    }
    return false;
}

//create new list from file
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

//clear linked list
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

//For frontend
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