#pragma once
#include "RedBlackNode.h"
#include <string>
#include <vector>

struct NodeSnapshot {
    int val;
    bool isRed;
    int leftVal;
    int rightVal;
    int parentVal;
    bool isDummy;
};

struct StepState {
    std::string description;
    std::vector<int> treeData;
    std::vector<std::string> nodeColors;
    int highlightedNode;
    
    std::vector<NodeSnapshot> nodes;
    
    std::vector<std::string> codeLines;
    int activeLine = -1;
};

class RedBlackTree {
private:
    RedBlackNode *mpRoot;
    std::vector<StepState> mStepHistory;
    int mCurrentStep;
    int mVisualizationSpeed;
    
    void rotateLeft(RedBlackNode *x);
    void rotateRight(RedBlackNode *x);
    
    void fixInsertion(RedBlackNode *node);
    void fixDeletion(RedBlackNode *node);
    
    RedBlackNode *findMin(RedBlackNode *root);
    RedBlackNode *searchNode(int val);
    void transplant(RedBlackNode *x, RedBlackNode *y);
        
    void clearTree(RedBlackNode *root);
    void collectSnapshot(RedBlackNode *node, std::vector<NodeSnapshot>& nodes);
    void inorderCollect(RedBlackNode *root, std::vector<int>& data, std::vector<std::string>& colors);
    
    void saveStep(const std::string& description, int highlightedNode = -1, int activeLine = -1, const std::vector<std::string>& codeLines = {});
    RedBlackNode *mpBackupRoot;
    RedBlackNode *cloneNode(RedBlackNode *node, RedBlackNode *parent);

public:
    RedBlackTree();
    ~RedBlackTree();
    
    void initialize();
    void initializeFromFile(std::string filename);
    
    bool insert(int val);
    bool remove(int val);
    bool update(int oldVal, int newVal);
    bool search(int val);
    
    void setCurrentStep(int step);
    void nextStep();
    void previousStep();
    void goToFinalStep();
    void setVisualizationSpeed(int speed);
        
    bool isEmpty();
    std::vector<StepState> getStepHistory();
    int getCurrentStep();
    int getVisualizationSpeed();
    
        
    void resetHistory(std::string initialMessage);
    void backup();
    void restore();
};
