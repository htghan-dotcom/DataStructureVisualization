#pragma once
#include <vector>
#include <string>


struct Node {
    int data;
    Node* prev;
    Node* next;
    Node(int val) : data(val), prev(nullptr), next(nullptr) {}
};

class DoublyLinkedList {
private:
    Node* head;
    Node* tail;
    int size;

public:
    DoublyLinkedList();
    ~DoublyLinkedList(); 

    
    void insertTail(int value);
    void insertHead(int value);
    
    
    bool deleteValue(int value); 
    
    int search(int value) const; 
    
    bool update(int oldValue, int newValue); 
    
    bool initFromFile(const std::string& filename);

    void clear(); 

    //For frontend
    std::vector<int> getElements() const; 
    int getSize() const;
};