#include "Common.h"
#include "RedBlackTree/RedBlackTree.h"

RedBlackTree::RedBlackTree(){
    mpRoot = nullptr;
    mpBackupRoot = nullptr;
    mCurrentStep = -1;
    mVisualizationSpeed = 500;
}

RedBlackTree::~RedBlackTree(){
    clearTree(mpRoot);
    clearTree(mpBackupRoot);
}

void RedBlackTree::rotateLeft(RedBlackNode *x){
    RedBlackNode *y = x->getRight();
    
    x->setRight(y->getLeft());
    if (y->getLeft()){
        y->getLeft()->setParent(x);
    }
    
    y->setParent(x->getParent());
    
    if (x == mpRoot){
        mpRoot = y;
    }
    
    else if (x == x->getParent()->getLeft()){
        x->getParent()->setLeft(y);
    }
    
    else {
        x->getParent()->setRight(y);
    }
    
    y->setLeft(x);
    x->setParent(y);
}

void RedBlackTree::rotateRight(RedBlackNode *x){
    RedBlackNode *y = x->getLeft();
    
    x->setLeft(y->getRight());
    if (y->getRight()){
        y->getRight()->setParent(x);
    }
    
    y->setParent(x->getParent());
    
    if (x == mpRoot){
        mpRoot = y;
    }
    
    else if (x == x->getParent()->getRight()){
        x->getParent()->setRight(y);
    }
    
    else {
        x->getParent()->setLeft(y);
    }
    
    y->setRight(x);
    x->setParent(y);
}

void RedBlackTree::fixInsertion(RedBlackNode *node){
    if (!node->getParent() or !node->getParent()->isRed()){
        if (node == mpRoot and node->getColor() != Color::BLACK){
            mpRoot->setColor(Color::BLACK);
            saveStep("Root must be BLACK. \nRecolor Root to BLACK", mpRoot->getVal());
        }
        return;
    }
    
    RedBlackNode *parent = node->getParent();
    RedBlackNode *grandparent = parent->getParent();
    
    saveStep("RED-RED conflict between " + to_string(node->getVal()) + "\nand Parent " + to_string(parent->getVal()) + ".\nChecking Uncle", parent->getVal());

    if (parent == grandparent->getLeft()){
        RedBlackNode *uncle = grandparent->getRight();
        
        if (!uncle or !uncle->isRed()){
            string uncleName = uncle ? to_string(uncle->getVal()) : "NULL";
            saveStep("Uncle " + uncleName + " is BLACK.\nRotate.", uncle ? uncle->getVal() : grandparent->getVal());
            
            if (node == parent->getRight()){
                saveStep("Node is RIGHT child (LR case).\nRotate LEFT at Parent " + to_string(parent->getVal()), parent->getVal());
                rotateLeft(parent);
                fixInsertion(parent);
            } else {
                saveStep("Node is LEFT child (LL case).\nRotate RIGHT at Grandparent " + to_string(grandparent->getVal()), grandparent->getVal());
                parent->setColor(Color::BLACK);
                grandparent->setColor(Color::RED);
                rotateRight(grandparent);
            }
        } else {
            saveStep("Uncle " + to_string(uncle->getVal()) + " is RED.\nRecolor.", uncle->getVal());
            
            parent->setColor(Color::BLACK);
            uncle->setColor(Color::BLACK);
            grandparent->setColor(Color::RED);
            
            saveStep("Recolor Parent & Uncle to BLACK,\nGrandparent to RED", grandparent->getVal());
            
            fixInsertion(grandparent);
        }
    } else {
        RedBlackNode *uncle = grandparent->getLeft();
        
        if (!uncle or !uncle->isRed()){
            string uncleName = uncle ? to_string(uncle->getVal()) : "NULL";
            saveStep("Uncle " + uncleName + " is BLACK.\nRotate.", uncle ? uncle->getVal() : grandparent->getVal());
            
            if (node == parent->getLeft()){
                saveStep("Node is LEFT child (RL case).\nRotate RIGHT at Parent " + to_string(parent->getVal()), parent->getVal());
                rotateRight(parent);
                fixInsertion(parent);
            } else {
                saveStep("Node is RIGHT child (RR case).\nRotate LEFT at Grandparent " + to_string(grandparent->getVal()), grandparent->getVal());
                parent->setColor(Color::BLACK);
                grandparent->setColor(Color::RED);
                rotateLeft(grandparent);
            }
        } else {
            saveStep("Uncle " + to_string(uncle->getVal()) + " is RED.\nRecolor.", uncle->getVal());
            
            parent->setColor(Color::BLACK);
            uncle->setColor(Color::BLACK);
            grandparent->setColor(Color::RED);
            
            saveStep("Recolor Parent & Uncle to BLACK,\nGrandparent to RED", grandparent->getVal());
            
            fixInsertion(grandparent);
        }
    }
    
    if (mpRoot->isRed()){
        mpRoot->setColor(Color::BLACK);
        saveStep("Fix completed. Recolor ROOT back to BLACK.", mpRoot->getVal());
    }
}

void RedBlackTree::fixDeletion(RedBlackNode *node){
    if (!node){return;}
    
    if (node == mpRoot){
        node->setColor(Color::BLACK);
        saveStep("Double-BLACK reached ROOT.\nRecolor ROOT to BLACK.", mpRoot->getVal());
        return;
    }
    
    RedBlackNode *parent = node->getParent();
    if (!parent){return;}
    RedBlackNode *sibling;
    
    if (node == parent->getLeft()){
        sibling = parent->getRight();
        if (!sibling){
            fixDeletion(parent);
            return;
        }
        
        if (sibling and sibling->isRed()){
            saveStep("Sibling " + to_string(sibling->getVal()) + " is RED.\nRecolor & Rotate LEFT at Parent.", parent->getVal());
            parent->setColor(Color::RED);
            sibling->setColor(Color::BLACK);
            rotateLeft(parent);
            fixDeletion(node);
            return;
        }
        
        if (sibling and (!sibling->getLeft() or !sibling->getLeft()->isRed()) and (!sibling->getRight() or !sibling->getRight()->isRed())){
            saveStep("Sibling " + to_string(sibling->getVal()) + " and Nephews are BLACK.\nRecolor Sibling to RED.", sibling->getVal());
            sibling->setColor(Color::RED);
            
            if (parent->isRed()){
                saveStep("Parent is RED.\nRecolor Parent to BLACK.", parent->getVal());
                parent->setColor(Color::BLACK);
            } else {
                saveStep("Parent is BLACK.\nDouble-Black moves up to Parent.", parent->getVal());
                fixDeletion(parent);
            }
            return;
        }
        
        saveStep("Sibling is BLACK.\nChecking Nephews.", sibling->getVal());
        
        if (!sibling->getRight() or !sibling->getRight()->isRed()){
            saveStep("RIGHT Nephew is BLACK.\nRecolor & Rotate RIGHT at Sibling.", sibling->getVal());
            sibling->getLeft()->setColor(Color::BLACK);
            sibling->setColor(Color::RED);
            rotateRight(sibling);
            sibling = parent->getRight();
        }
        
        saveStep("RIGHT Nephew is RED.\nRecolor & Rotate LEFT at Parent.", parent->getVal());
        sibling->setColor(parent->getColor());
        parent->setColor(Color::BLACK);
        sibling->getRight()->setColor(Color::BLACK);
        rotateLeft(parent);
        
        if (node){node->setColor(Color::BLACK);}
    } else {
        sibling = parent->getLeft();
        if (!sibling){
            fixDeletion(parent);
            return;
        }
        
        if (sibling and sibling->isRed()){
            saveStep("Sibling " + to_string(sibling->getVal()) + " is RED.\nRecolor & Rotate RIGHT at Parent.", parent->getVal());
            parent->setColor(Color::RED);
            sibling->setColor(Color::BLACK);
            rotateRight(parent);
            fixDeletion(node);
            return;
        }
        
        if (sibling and (!sibling->getRight() or !sibling->getRight()->isRed()) and (!sibling->getLeft() or !sibling->getLeft()->isRed())){
            saveStep("Sibling " + to_string(sibling->getVal()) + " and Nephews are BLACK.\nRecolor Sibling to RED.", sibling->getVal());
            sibling->setColor(Color::RED);
            
            if (parent->isRed()){
                saveStep("Parent is RED.\nRecolor Parent to BLACK.", parent->getVal());
                parent->setColor(Color::BLACK);
            } else {
                saveStep("Parent is BLACK.\nDouble-BLACK moves up to Parent.", parent->getVal());
                fixDeletion(parent);
            }
            return;
        }
        
        saveStep("Sibling is BLACK.\nChecking Nephews", sibling->getVal());
        
        if (!sibling->getLeft() or !sibling->getLeft()->isRed()){
            saveStep("LEFT Nephew is BLACK.\nRecolor & Rotate LEFT at Sibling.", sibling->getVal());
            sibling->getRight()->setColor(Color::BLACK);
            sibling->setColor(Color::RED);
            rotateLeft(sibling);
            sibling = parent->getLeft();
        }
        
        saveStep("LEFT Nephew is RED.\nRecolor & Rotate RIGHT at Parent.", parent->getVal());
        sibling->setColor(parent->getColor());
        parent->setColor(Color::BLACK);
        sibling->getLeft()->setColor(Color::BLACK);
        rotateRight(parent);
        
        if (node){node->setColor(Color::BLACK);}
    }
}

RedBlackNode *RedBlackTree::findMin(RedBlackNode *root){
    if (!root){return nullptr;}
    
    RedBlackNode *current = root;
    while (current->getLeft()){
        current = current->getLeft();
    }
    
    return current;
}

RedBlackNode *RedBlackTree::searchNode(int val){
    RedBlackNode *current = mpRoot;
    saveStep("Search for " + to_string(val), -1);
    
    while (current){
        if (current->getVal() == val){
            saveStep("Found node " + to_string(val), val);
            return current;

        }
        if (val < current->getVal()){
            current = current->getLeft();
        } else {
            current = current->getRight();
        }
    }
    
    saveStep("Value " + to_string(val) + " not found", -1);
    return nullptr;
}

void RedBlackTree::transplant(RedBlackNode *x, RedBlackNode *y){
    if (x == mpRoot){
        mpRoot = y;
    }
    else if (x == x->getParent()->getLeft()){
        x->getParent()->setLeft(y);
    } else {
        x->getParent()->setRight(y);
    }
    
    if (y){
        y->setParent(x->getParent());
    }
}


void RedBlackTree::clearTree(RedBlackNode *root){
    if (!root){return;}
    
    clearTree(root->getLeft());
    clearTree(root->getRight());
    
    delete root;
}

void RedBlackTree::collectSnapshot(RedBlackNode *node, vector<NodeSnapshot> &nodes){
    if (!node){return;}
    
    NodeSnapshot snap;
    snap.val = node->getVal();
    snap.isRed = node->isRed();
    snap.isDummy = node->isDummy();
    
    if (node->getLeft()){
        snap.leftVal = node->getLeft()->getVal();
    } else {snap.leftVal = -1;}
    
    if (node->getRight()){
        snap.rightVal = node->getRight()->getVal();
    } else {snap.rightVal = -1;}
    
    if (node->getParent()){
        snap.parentVal = node->getParent()->getVal();
    } else {snap.parentVal = -1;}
    
    nodes.push_back(snap);
    
    collectSnapshot(node->getLeft(), nodes);
    collectSnapshot(node->getRight(), nodes);
}

void RedBlackTree::inorderCollect(RedBlackNode *root, vector<int>& data, vector<string>& colors){
    if (!root){return;}
    
    inorderCollect(root->getLeft(), data, colors);
    
    data.push_back(root->getVal());
    if (root->isRed()){
        colors.push_back("RED");
    }
    else {
        colors.push_back("BLACK");
    }
    
    inorderCollect(root->getRight(), data, colors);
}

void RedBlackTree::saveStep(string description, int highlightedNode){
    StepState step;
    step.description = description;
    step.highlightedNode = highlightedNode;
    
    inorderCollect(mpRoot, step.treeData, step.nodeColors);
    collectSnapshot(mpRoot, step.nodes);
    
    if (mCurrentStep >= 0 and mCurrentStep < static_cast<int>(mStepHistory.size() - 1)){
        mStepHistory.erase(mStepHistory.begin() + mCurrentStep + 1, mStepHistory.end());
    }
    
    mStepHistory.push_back(step);
    mCurrentStep = static_cast<int>(mStepHistory.size() - 1);
}

void RedBlackTree::initialize(){
    clearTree(mpRoot);
    mpRoot = nullptr;
    mStepHistory.clear();
    mCurrentStep = -1;
    
    saveStep("Initialized empty tree");
    cout << "Tree initialized" << endl;
}

void RedBlackTree::initializeFromFile(string filename){
    initialize();
    ifstream file(filename);
    if (!file.is_open()){
        cout << "Cannot open file:" << filename << endl;
        return;
    }
    
    int val;
    int count = 0;
    while (file >> val){
        insert(val);
        count++;
    }
    
    file.close();
    cout << "Successfully loaded " << count << " values from " << filename << endl;
    
    saveStep("Initialized from file: " + filename);
}

bool RedBlackTree::insert(int val){
    RedBlackNode *newNode = new RedBlackNode(val);
    
    if (!mpRoot){
        mpRoot = newNode;
        mpRoot->setColor(Color::BLACK);
        
        saveStep("Tree is empty. Set " + to_string(val) + " as Root (BLACK)", val);
        return true;
    }
    
    RedBlackNode *current = mpRoot;
    RedBlackNode *parent = nullptr;
    
    while (current){
        parent = current;
        
        if (val < current->getVal()){
            saveStep("Compare " + to_string(val) + " < " + to_string(current->getVal()) + ". Go LEFT", current->getVal());
            current = current->getLeft();
        }
        else if (val > current->getVal()){
            saveStep("Compare " + to_string(val) + " > " + to_string(current->getVal()) + ". Go RIGHT", current->getVal());
            current = current->getRight();
        }
        else {
            delete newNode;
            saveStep("Insertion failed - Duplicate value " + to_string(val), current->getVal());
            return false;
        }
    }
    
    newNode->setParent(parent);
    if (val < parent->getVal()){
        parent->setLeft(newNode);
    }
    else {
        parent->setRight(newNode);
    }
    
    saveStep("Attach " + to_string(val) + " as child (Red)", val);
    
    fixInsertion(newNode);
    
    saveStep("Insertion completed!", val);
    
    return true;
}

bool RedBlackTree::remove(int val){
    RedBlackNode *targetNode = mpRoot;
    while (targetNode){
        if (val == targetNode->getVal()){
            break;
        }
        if (val < targetNode->getVal()){
            saveStep("Compare " + to_string(val) + " < " + to_string(targetNode->getVal()) + ". Go LEFT", targetNode->getVal());
            targetNode = targetNode->getLeft();
        } else {
            saveStep("Compare " + to_string(val) + " > " + to_string(targetNode->getVal()) + ". Go RIGHT", targetNode->getVal());
            targetNode = targetNode->getRight();
        }
    }

    if (!targetNode){
        saveStep("Remove failed!\nValue " + to_string(val) + " not found in tree.");
        return false;
    }

    RedBlackNode *replaceNode = nullptr;
    Color originalColor = targetNode->getColor();
    RedBlackNode *dummy = nullptr;
    
    if (!targetNode->getLeft() and !targetNode->getRight()){
        saveStep("Node " + to_string(val) + " is a LEAF.\nRemove directly.", targetNode->getVal());
        if (originalColor == Color::BLACK){
            dummy = new RedBlackNode(0);
            dummy->setDummy(true);
            dummy->setColor(Color::BLACK);
            replaceNode = dummy;
            transplant(targetNode, dummy);
        } else {
            transplant(targetNode, nullptr);
        }
    }
    else if (!targetNode->getLeft()){
        saveStep("Node " + to_string(val) + " has only RIGHT child.\nReplace with RIGHT child.", targetNode->getVal());
        replaceNode = targetNode->getRight();
        transplant(targetNode, targetNode->getRight());
    }
    else if (!targetNode->getRight()){
        saveStep("Node " + to_string(val) + " has only LEFT child.\nReplace with LEFT child.", targetNode->getVal());
        replaceNode = targetNode->getLeft();
        transplant(targetNode, targetNode->getLeft());
    } else {
        saveStep("Node has TWO children.\nFinding Successor", targetNode->getVal());
        RedBlackNode *successor = findMin(targetNode->getRight());
        originalColor = successor->getColor();

        if (!successor->getRight() and originalColor == Color::BLACK){
            dummy = new RedBlackNode(0);
            dummy->setDummy(true);
            dummy->setColor(Color::BLACK);
            replaceNode = dummy;
        } else {
            replaceNode = successor->getRight();
        }

        saveStep("Successor found: " + to_string(successor->getVal()) + ".\nReplacing node with Successor.", successor->getVal());

        if (successor == targetNode->getRight()){
            if (dummy) successor->setRight(dummy);
            if (replaceNode) replaceNode->setParent(successor);
        } else {
            transplant(successor, replaceNode);
            successor->setRight(targetNode->getRight());
            successor->getRight()->setParent(successor);
        }

        transplant(targetNode, successor);
        successor->setLeft(targetNode->getLeft());
        successor->getLeft()->setParent(successor);
        successor->setColor(targetNode->getColor());
    }
    
    delete targetNode;

    if (originalColor == Color::BLACK){
        if (replaceNode != nullptr and replaceNode->isRed()){
            saveStep("Replacement node " + to_string(replaceNode->getVal()) + " is RED.\nRecolor to BLACK.", replaceNode->getVal());
            replaceNode->setColor(Color::BLACK);
        } else {
            saveStep("Removed node was BLACK.\nDouble-BLACK violation! FIXING", replaceNode ? replaceNode->getVal() : -1);
            fixDeletion(replaceNode);
        }
    } else {
        saveStep("Removed node was RED.\nNo violation. Tree remains balanced.");
    }

    if (dummy){
        transplant(dummy, nullptr);
        delete dummy;
    }

    if (mpRoot){ mpRoot->setColor(Color::BLACK); }
    saveStep("Removal completed!");
        
    return true;
}


bool RedBlackTree::update(int oldVal, int newVal){
    saveStep("Updating " + to_string(oldVal) + " to " + to_string(newVal));
    
    RedBlackNode *targetNode = searchNode(oldVal);
    if (!targetNode){
        saveStep("Update failed - value " + to_string(oldVal) + " not found");
        return false;
    }
    
    if (oldVal == newVal){
        saveStep("Update skipped - value are identical", oldVal);
        return true;
    }
    
    if (searchNode(newVal)){
        saveStep("Update failed - new value " + to_string(newVal) + " already exists", newVal);
        return false;
    }
    
    if (remove(oldVal)){
        if (insert(newVal)){
            saveStep("Update successful: " + to_string(oldVal) + "->" + to_string(newVal), newVal);
            return true;
        }
    }
    
    saveStep("Update failed");
    return false;
}

void RedBlackTree::nextStep(){
    if (mStepHistory.empty()){
        cout << "No steps available" << endl;
        return;
    }
    
    if (mCurrentStep < static_cast<int>(mStepHistory.size() - 1)){
        mCurrentStep++;
        return;
    }
    
    cout << "ALready at final step" << endl;
}

void RedBlackTree::previousStep(){
    if (mStepHistory.empty()){
        cout << "No steps available" << endl;
        return;
    }
    
    if (mCurrentStep > 0){
        mCurrentStep--;
        return;
    }
    
    cout << "Already at the first step" << endl;
}

void RedBlackTree::goToFinalStep(){
    if (mStepHistory.empty()){
        cout << "No steps available" << endl;
        return;
    }
    
    mCurrentStep = static_cast<int>(mStepHistory.size() - 1);
}

void RedBlackTree::setVisualizationSpeed(int speed){
    if (speed >= 100 and speed <= 3000){
        mVisualizationSpeed = speed;
    } else {
        cout << "Speed must be between 100 and 3000ms" << endl;
    }
}

bool RedBlackTree::isEmpty(){
    return mpRoot == nullptr;
}

vector<StepState> RedBlackTree::getStepHistory(){
    return mStepHistory;
}

int RedBlackTree::getCurrentStep(){
    return mCurrentStep;
}

int RedBlackTree::getVisualizationSpeed(){
    return mVisualizationSpeed;
}

void RedBlackTree::setCurrentStep(int step){
    if (step >= 0 and step < static_cast<int>(mStepHistory.size())){
        mCurrentStep = step;
    }
}

void RedBlackTree::resetHistory(string initialMessage){
    mStepHistory.clear();
    mCurrentStep = -1;
    saveStep(initialMessage);
}

RedBlackNode* RedBlackTree::cloneNode(RedBlackNode* node, RedBlackNode* parent){
    if (!node) return nullptr;
    RedBlackNode* newNode = new RedBlackNode(node->getVal());
    newNode->setColor(node->getColor());
    newNode->setParent(parent);
    newNode->setDummy(node->isDummy());
    newNode->setLeft(cloneNode(node->getLeft(), newNode));
    newNode->setRight(cloneNode(node->getRight(), newNode));
    return newNode;
}

void RedBlackTree::backup(){
    clearTree(mpBackupRoot);
    mpBackupRoot = cloneNode(mpRoot, nullptr);
}

void RedBlackTree::restore(){
    if (!mpBackupRoot) return;
    clearTree(mpRoot);
    mpRoot = cloneNode(mpBackupRoot, nullptr);
    resetHistory("Undo successfully.");
}

bool RedBlackTree::search(int val){
    if (!mpRoot){
        saveStep("Tree is empty. Cannot find " + to_string(val), -1);
        return false;
    }

    RedBlackNode *current = mpRoot;

    while (current){
        if (val == current->getVal()){
            saveStep("Found node " + to_string(val) + "!", current->getVal());
            return true;
        }
        else if (val < current->getVal()){
            saveStep("Compare " + to_string(val) + " < " + to_string(current->getVal()) + ". Go LEFT", current->getVal());
            current = current->getLeft();
        }
        else {
            saveStep("Compare " + to_string(val) + " > " + to_string(current->getVal()) + ". Go RIGHT", current->getVal());
            current = current->getRight();
        }
    }

    saveStep("Value " + to_string(val) + " not found in the tree.", -1);
    return false;
}
