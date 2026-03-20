#pragma once
#include "Common.h"
#include "RedBlackNode.h"

struct StepState{
    string description;
    vector<int> treeData;
    vector<string> nodeColors;
    int highlightedNode;
};

class RedBlackTree{
private:
    RedBlackNode *mpRoot;
    vector<StepState> mStepHistory;
    int mCurrentStep;
    int mVisualizationSpeed;
    
    // Rotate operations
    void rotateLeft(RedBlackNode *x);
    void rotateRight(RedBlackNode *x);
    
    // Fix violations
    void fixInsertion(RedBlackNode *node);
    void fixDeletion(RedBlackNode *node);
    
    RedBlackNode *findMin(RedBlackNode *root);
    RedBlackNode *findMax(RedBlackNode *root);
    RedBlackNode *searchNode(int val);
    void transplant(RedBlackNode *x, RedBlackNode *y);
        
    void clearTree(RedBlackNode *root);
    void inorderCollect(RedBlackNode *root, vector<int>& data, vector<string>& colors);
    int getHeight(RedBlackNode *root);
    int getSize(RedBlackNode *root);
    
    void saveStep(string description, int highlightedNode = -1);

public:
    RedBlackTree();
    ~RedBlackTree();
    
    // Initialization methods
    void initialize();
    void initializeFromFile(string filename);
    void initializeRandom(int count, int minVal, int maxVal);
    
    // Core operations
    bool insert(int val);
    bool remove(int val);
    bool update(int oldVal, int newVal);
    bool search(int val);
    
    // Visualization controls
    void nextStep();
    void previousStep();
    void goToFinalStep();
    void runAtOnce();
    void setVisualizationSpeed(int speed);
    
    // Display methods (for debugging only)
    void display();
    void displayCurrentStep();
    void displayStepInfo();
    
    // Utility methods
    bool isEmpty();
    int getRootData();
    int getTreeHeight();
    int getNodeCount();
    vector<StepState> getStepHistory();
    int getCurrentStep();
    int getVisualizationSpeed();
    
    bool validateRBTree();
    
    string exportToJSON();
};
