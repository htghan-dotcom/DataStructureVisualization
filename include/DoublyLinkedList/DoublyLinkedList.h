#pragma once
#include <vector>
#include <string>
#include "DLLStepState.h"


struct DoublyLinkedListNode {
    int data;
    DoublyLinkedListNode* prev;
    DoublyLinkedListNode* next;
    DoublyLinkedListNode(int val) : data(val), prev(nullptr), next(nullptr) {}
};

class DoublyLinkedList {
private:
    DoublyLinkedListNode* head;
    DoublyLinkedListNode* tail;
    int size;
    
    std::vector<DLLStepState> mStepHistory;
    int mCurrentStep;
    DoublyLinkedList* mBackup;
    int mVisualizationSpeed;

public:
    DoublyLinkedList();
    ~DoublyLinkedList(); 

    void insertTail(int value);
    void insertHead(int value);
    
    bool deleteValue(int value); 
    
    int search(int value);
    
    bool update(int oldValue, int newValue); 
    
    bool initFromFile(const std::string& filename);

    void clear(); 

    //For frontend
    std::vector<int> getElements() const; 
    int getSize() const;
    
    // For visualization
    void recordStep(const std::string& description);
    std::vector<DLLStepState> getStepHistory() const;
    int getCurrentStep() const;
    void setCurrentStep(int step);
    void resetHistory(const std::string& initialDescription);
    void initialize();
    void backup();
    void restore();
    void goToFinalStep();
    int getVisualizationSpeed() const;
    void setVisualizationSpeed(int ms);
};
