#include "Common.h"
#include "RedBlackTree/RedBlackTree.h"

const vector<string> PSEUDO_SEARCH = {
    "current = root",
    "while current != NULL:",
    "   if val == current.val: return current // found",
    "   if val < current.val: current = current.left",
    "   if val > current.val: current = current.right",
    "return NULL // not found"
};

const vector<string> PSEUDO_INSERT = {
    "BST insert node X (RED)",
    "while RED-RED conflict with parent P:\n    checking uncle U",
    "ensure root is BLACK"
};

const vector<string> PSEUDO_FIXINSERTION = {
    "checking unlce U:",
    "   if U is RED:\n      recolor P and U to BLACK, G to RED & check G",
    "   elif U is BLACK & X is inner child:\n      rotate at P & check P",
    "   else U is BLACK & X is outer child:\n      rotate at G & swap color of G and P"
};

const vector<string> PSEUDO_DELETE = {
    "BST delete node V, replacement U",
    "if either V or U is RED:\n   recolor U to BLACK",
    "else while V and U is BLACK (double BLACK conflict):\n   check sibling S of U",
    "if U is root:\n   recolor root to BLACK"
};

const vector<string> PSEUDO_FIXDELETION1 = {
    "checking sibling S of U:",
    "   if S is RED:\n      rotate at parent P, swap color of P and S",
    "   else S is BLACK:\n      check nephews"
};

const vector<string> PSEUDO_FIXDELETION2 = {
    "checking nephews:",
    "   if S is BLACK and both nephews are BLACK:\n      recolor S to RED & check P",
    "   elif S is BLACK and outer nephew O is RED:\n      rotate at P & recolor O to BLACK",
    "   else S is BLACK and only inner nephew I is RED:\n      rotate at S & swap color of S and I"
};

const vector<string> PSEUDO_FIXDELETION3 = {
    "checking parent P:",
    "   if P is RED:\n      recolor P to BLACK",
    "   else P is BLACK:\n      P is U & check sibling S of U"
};


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
    // ENSURE ROOT ALWAYS BLACK
    if (!node->getParent() or !node->getParent()->isRed()){
        saveStep("ROOT must be BLACK. \nRecolor ROOT to BLACK", mpRoot->getVal(), 2, PSEUDO_INSERT);

        mpRoot->setColor(Color::BLACK);
        return;
    }
    
    RedBlackNode *parent = node->getParent();
    RedBlackNode *grandparent = parent->getParent();
    
    // RED-RED conflict
    saveStep("RED-RED conflict between " + to_string(node->getVal()) + " and parent " + to_string(parent->getVal()) + ".\nChecking uncle", parent->getVal(), 1, PSEUDO_INSERT);

    if (parent == grandparent->getLeft()){
        // LEFT parent
        RedBlackNode *uncle = grandparent->getRight();
        
        if (!uncle or !uncle->isRed()){
            // BLACK uncle
            bool isDummyUncle = false;
            
            if (!uncle){
                uncle = new RedBlackNode(0);
                uncle->setDummy(true);
                uncle->setColor(Color::BLACK);
                uncle->setParent(grandparent);
                grandparent->setRight(uncle);
                isDummyUncle = true;
            }
            
            string uncleName = isDummyUncle ? "NULL" : to_string(uncle->getVal());
            saveStep("Uncle " + uncleName + " is BLACK.\nRotate.", uncle->getVal(), 0, PSEUDO_FIXINSERTION);
            
            if (node == parent->getRight()){
                // LEFT-RIGHT case
                saveStep("Node is RIGHT child (inner).\nRotate LEFT at parent " + to_string(parent->getVal()), parent->getVal(), 2, PSEUDO_FIXINSERTION);
                
                rotateLeft(parent);
                fixInsertion(parent);
            } else {
                // LEFT-LEFT case
                saveStep("Node is LEFT child (outer).\nRotate RIGHT at grandparent " + to_string(grandparent->getVal()), grandparent->getVal(), 3, PSEUDO_FIXINSERTION);
                
                parent->setColor(Color::BLACK);
                grandparent->setColor(Color::RED);
                rotateRight(grandparent);
            }
            
            if (isDummyUncle){
                grandparent->setRight(nullptr);
                delete uncle;
                uncle = nullptr;
            }
        } else {
            // RED unlce
            saveStep("Uncle " + to_string(uncle->getVal()) + " is RED.\nRecolor.", uncle->getVal(), 0, PSEUDO_FIXINSERTION);
            
            saveStep("Recolor parent & uncle to BLACK,\ngrandparent to RED", grandparent->getVal(), 1, PSEUDO_FIXINSERTION);
            
            parent->setColor(Color::BLACK);
            uncle->setColor(Color::BLACK);
            grandparent->setColor(Color::RED);
            
            
            fixInsertion(grandparent);
        }
    } else {
        // RIGHT parent
        RedBlackNode *uncle = grandparent->getLeft();
        
        if (!uncle or !uncle->isRed()){
            // BLACK uncle
            bool isDummyUncle = false;
            
            if (!uncle){
                uncle = new RedBlackNode(0);
                uncle->setDummy(true);
                uncle->setColor(Color::BLACK);
                uncle->setParent(grandparent);
                grandparent->setLeft(uncle);
                isDummyUncle = true;
            }
            
            string uncleName = isDummyUncle ? "NULL" : to_string(uncle->getVal());
            saveStep("Uncle " + uncleName + " is BLACK.\nRotate.", uncle->getVal(), 0, PSEUDO_FIXINSERTION);
            
            if (node == parent->getLeft()){
                // RIGHT-LEFT case
                saveStep("Node is LEFT child (inner).\nRotate RIGHT at parent " + to_string(parent->getVal()), parent->getVal(), 2, PSEUDO_FIXINSERTION);
                
                rotateRight(parent);
                fixInsertion(parent);
            } else {
                // RIGHT-RIGHT case
                saveStep("Node is RIGHT child (outer).\nRotate LEFT at grandparent " + to_string(grandparent->getVal()), grandparent->getVal(), 3, PSEUDO_FIXINSERTION);
                
                parent->setColor(Color::BLACK);
                grandparent->setColor(Color::RED);
                rotateLeft(grandparent);
            }
            
            if (isDummyUncle){
                grandparent->setLeft(nullptr);
                delete uncle;
                uncle = nullptr;
            }
            
        } else {
            // RED uncle
            saveStep("Uncle " + to_string(uncle->getVal()) + " is RED.\nRecolor.", uncle->getVal(), 0, PSEUDO_FIXINSERTION);
            
            saveStep("RecolorpParent & uncle to BLACK,\ngrandparent to RED", grandparent->getVal(), 1, PSEUDO_FIXINSERTION);
            
            parent->setColor(Color::BLACK);
            uncle->setColor(Color::BLACK);
            grandparent->setColor(Color::RED);
            
            fixInsertion(grandparent);
        }
    }
    
    // ENSURE ROOT ALWAYS BLACK
    if (mpRoot->isRed()){
        saveStep("Fix completed. Recolor ROOT back to BLACK.", mpRoot->getVal(), 2, PSEUDO_INSERT);
        
        mpRoot->setColor(Color::BLACK);
    }
}

void RedBlackTree::fixDeletion(RedBlackNode *node){
    if (!node){return;}
    
    if (node == mpRoot){
        // double-BLACK at root
        saveStep("Double-BLACK reached ROOT.\nRecolor ROOT to BLACK.", mpRoot->getVal(), 3, PSEUDO_DELETE);

        node->setColor(Color::BLACK);
        
        return;
    }
    
    RedBlackNode *parent = node->getParent();
    if (!parent){return;}
    RedBlackNode *sibling;
    
    saveStep("Double-BLACK conflict at " + to_string(node->getVal()) + ".\nChecking sibling", parent->getVal(), 2, PSEUDO_DELETE);
    
    if (node == parent->getLeft()){
        // RIGHT sibling
        sibling = parent->getRight();
        
        if (sibling->isRed()){
            // RED sibling
            saveStep("Sibling " + to_string(sibling->getVal()) + " is RED.\nRotate & Recolor.", sibling->getVal(), 0, PSEUDO_FIXDELETION1);
            
            saveStep("Rotate LEFT at parent & Recolor parent to RED and sibling to BLACK", parent->getVal(), 1, PSEUDO_FIXDELETION1);
            
            parent->setColor(Color::RED);
            sibling->setColor(Color::BLACK);
            rotateLeft(parent);
            fixDeletion(node);
            
            return;
        }
        
        // BLACK sibling
        saveStep("Sibling " + to_string(sibling->getVal()) + " is BLACK.\nChecking nephews.", sibling->getVal(), 2, PSEUDO_FIXDELETION1);
        
        if (sibling and (!sibling->getLeft() or !sibling->getLeft()->isRed()) and (!sibling->getRight() or !sibling->getRight()->isRed())){
            // both nephews are BLACK
            saveStep("Both nephews are BLACK.\nRecolor sibling to RED & Checking parent.", sibling->getVal(), 1, PSEUDO_FIXDELETION2);
            
            sibling->setColor(Color::RED);
            
            if (parent->isRed()){
                // RED parent
                saveStep("Parent is RED.\nRecolor parent to BLACK.", parent->getVal(), 1, PSEUDO_FIXDELETION3);
                
                parent->setColor(Color::BLACK);
            } else {
                // BLACK parent
                saveStep("Parent is BLACK.\nDouble-BLACK moves up to parent.", parent->getVal(), 2, PSEUDO_FIXDELETION3);
                
                fixDeletion(parent);
            }
            return;
        }
        
        if (!sibling->getRight() or !sibling->getRight()->isRed()){
            // RIGHT-LEFT case
            saveStep("RIGHT nephew is BLACK (inner RED).\nRotate RIGHT at sibling & Recolor sibling to RED and nephew to BLACK", sibling->getVal(), 3, PSEUDO_FIXDELETION2);
            
            sibling->getLeft()->setColor(Color::BLACK);
            sibling->setColor(Color::RED);
            rotateRight(sibling);
            sibling = parent->getRight();
        }
        
        // RIGHT-RIGHT case
        saveStep("RIGHT nephew is RED (outer RED).\nRotate LEFT at Parent & Recolor nephew to BLACK.", parent->getVal(), 2, PSEUDO_FIXDELETION2);
        
        sibling->setColor(parent->getColor());
        parent->setColor(Color::BLACK);
        sibling->getRight()->setColor(Color::BLACK);
        rotateLeft(parent);
        
        if (node){node->setColor(Color::BLACK);}
    } else {
        // LEFT sibling
        sibling = parent->getLeft();
        
        if (sibling and sibling->isRed()){
            // RED Sibling
            saveStep("Sibling " + to_string(sibling->getVal()) + " is RED.\nRotate & Recolor.", sibling->getVal(), 0, PSEUDO_FIXDELETION1);
            
            saveStep("Rotate RIGHT at parent & Recolor parent to RED and sibling to BLACK", parent->getVal(), 1, PSEUDO_FIXDELETION1);
            
            parent->setColor(Color::RED);
            sibling->setColor(Color::BLACK);
            rotateRight(parent);
            fixDeletion(node);
            

            return;
        }
        
        // BLACK sibling
        saveStep("Sibling " + to_string(sibling->getVal()) + " is BLACK.\nChecking nephews.", sibling->getVal(), 2, PSEUDO_FIXDELETION1);
        
        if (sibling and (!sibling->getRight() or !sibling->getRight()->isRed()) and (!sibling->getLeft() or !sibling->getLeft()->isRed())){
            // both nephews are BLACK
            saveStep("Both nephews are BLACK.\nRecolor sibling to RED & Checking parent.", sibling->getVal(), 1, PSEUDO_FIXDELETION2);
            sibling->setColor(Color::RED);
            
            
            if (parent->isRed()){
                // RED parent
                saveStep("Parent is RED.\nRecolor parent to BLACK.", parent->getVal(), 1, PSEUDO_FIXDELETION3);
                
                parent->setColor(Color::BLACK);
            } else {
                // BLACK parent
                saveStep("Parent is BLACK.\nDouble-BLACK moves up to parent.", parent->getVal(), 2, PSEUDO_FIXDELETION3);
                
                fixDeletion(parent);
            }
            return;
        }
                
        if (!sibling->getLeft() or !sibling->getLeft()->isRed()){
            // LEFT-RIGHT case
            saveStep("LEFT nephew is BLACK (inner RED).\nRotate LEFT at sibling & Recolor sibling to RED and nephew to BLACK", sibling->getVal(), 3, PSEUDO_FIXDELETION2);
            
            sibling->getRight()->setColor(Color::BLACK);
            sibling->setColor(Color::RED);
            rotateLeft(sibling);
            sibling = parent->getLeft();
        }
        
        // RIGHT-RIGHT case
        saveStep("RIGHT nephew is RED (outer RED).\nRotate LEFT at parent & Recolor nephew to BLACK.", parent->getVal(), 2, PSEUDO_FIXDELETION2);
        
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

void RedBlackTree::saveStep(string description, int highlightedNode, int activeLine, const vector<string>& codeLines){
    StepState step;
    step.description = description;
    step.highlightedNode = highlightedNode;
    
    step.codeLines = codeLines;
    step.activeLine = activeLine;
    
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
        // empty tree
        mpRoot = newNode;
        mpRoot->setColor(Color::BLACK);
        saveStep("Tree is empty. Set " + to_string(val) + " as ROOT (BLACK)", val, 0, PSEUDO_INSERT);
        return true;
    }
    
    RedBlackNode *current = mpRoot;
    RedBlackNode *parent = nullptr;
    
    saveStep("Searching for insertion position for " + to_string(val), -1, 0, PSEUDO_SEARCH);
    
    while (current){
        parent = current;
        if (val < current->getVal()){
            saveStep("Compare " + to_string(val) + " < " + to_string(current->getVal()) + ". Go LEFT", current->getVal(), 3, PSEUDO_SEARCH);
            current = current->getLeft();
        }
        
        else if (val > current->getVal()){
            saveStep("Compare " + to_string(val) + " > " + to_string(current->getVal()) + ". Go RIGHT", current->getVal(), 4, PSEUDO_SEARCH);
            current = current->getRight();
        }
        
        else {
            delete newNode;
            saveStep("Insertion failed - Duplicate value " + to_string(val), current->getVal(), 5, PSEUDO_SEARCH);
            return false;
        }
    }
    
    newNode->setParent(parent);
    if (val < parent->getVal()){
        
        parent->setLeft(newNode);
    } else {
        
        parent->setRight(newNode);
    }
    
    saveStep("Attach " + to_string(val) + " as child (RED)", val, 0, PSEUDO_INSERT);
    
    fixInsertion(newNode);
    
    saveStep("Insertion completed!", val, 3, PSEUDO_INSERT);
    return true;
}

bool RedBlackTree::remove(int val){
    RedBlackNode *targetNode = mpRoot;
    
    saveStep("Searching for node " + to_string(val) + " to delete", -1, 0, PSEUDO_SEARCH);
    
    while (targetNode){
        if (val == targetNode->getVal()){
            saveStep("Found node " + to_string(val) + ". Start deletion process.", targetNode->getVal(), 2, PSEUDO_SEARCH);
            break;
        }
        
        if (val < targetNode->getVal()){
            saveStep("Compare " + to_string(val) + " < " + to_string(targetNode->getVal()) + ". Go LEFT", targetNode->getVal(), 3, PSEUDO_SEARCH);
            targetNode = targetNode->getLeft();
        } else {
            saveStep("Compare " + to_string(val) + " > " + to_string(targetNode->getVal()) + ". Go RIGHT", targetNode->getVal(), 4, PSEUDO_SEARCH);
            targetNode = targetNode->getRight();
        }
    }

    if (!targetNode){
        saveStep("Remove failed!\nValue " + to_string(val) + " not found in tree.", -1, 5, PSEUDO_SEARCH);
        return false;
    }

    RedBlackNode *replaceNode = nullptr;
    Color originalColor = targetNode->getColor();
    RedBlackNode *dummy = nullptr;
    
    if (!targetNode->getLeft() and !targetNode->getRight()){
        // leaf node
        saveStep("Node " + to_string(val) + " is a LEAF.\nRemove directly.", targetNode->getVal(), 0, PSEUDO_DELETE);
        
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
        // one child
        saveStep("Node " + to_string(val) + " has only RIGHT child.\nReplace with RIGHT child.", targetNode->getVal(), 0, PSEUDO_DELETE);
        replaceNode = targetNode->getRight();
        transplant(targetNode, targetNode->getRight());
    }
    
    else if (!targetNode->getRight()){
        // one child
        saveStep("Node " + to_string(val) + " has only LEFT child.\nReplace with LEFT child.", targetNode->getVal(), 0, PSEUDO_DELETE);
        replaceNode = targetNode->getLeft();
        transplant(targetNode, targetNode->getLeft());
    }
    
    else {
        // two children
        saveStep("Node has TWO children.\nFinding successor", targetNode->getVal(), 0, PSEUDO_DELETE);
        RedBlackNode *successor = findMin(targetNode->getRight());
        originalColor = successor->getColor();

        if (!successor->getRight() and originalColor == Color::BLACK){
            // leaf successor
            dummy = new RedBlackNode(0);
            dummy->setDummy(true);
            dummy->setColor(Color::BLACK);
            replaceNode = dummy;
        } else {
            // has one child
            replaceNode = successor->getRight();
        }

        saveStep("Successor found: " + to_string(successor->getVal()) + ".\nReplacing node with successor.", successor->getVal(), 0, PSEUDO_DELETE);

        if (successor == targetNode->getRight()){
            if (dummy){successor->setRight(dummy);}
            if (replaceNode){replaceNode->setParent(successor);}
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
        // BLACK delete
        if (replaceNode != nullptr and replaceNode->isRed()){
            // RED replacement
            saveStep("Replacement node " + to_string(replaceNode->getVal()) + " is RED.\nRecolor to BLACK.", replaceNode->getVal(), 1, PSEUDO_DELETE);
            
            replaceNode->setColor(Color::BLACK);
        } else {
            // BLACK replacement
            saveStep("Double-BLACK conflict! FIXING", replaceNode ? replaceNode->getVal() : -1, 2, PSEUDO_DELETE);
            fixDeletion(replaceNode);
        }
    } else {
        // RED delete
        saveStep("Removed node was RED.\nTree remains balanced.", -1, 1, PSEUDO_DELETE);
    }

    if (dummy){
        transplant(dummy, nullptr);
        delete dummy;
    }

    if (mpRoot){mpRoot->setColor(Color::BLACK);}
    saveStep("Deletion completed!", -1, 3, PSEUDO_DELETE);
        
    return true;
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
        saveStep("Tree is empty. Cannot find " + to_string(val), -1, 5, PSEUDO_SEARCH);
        return false;
    }

    RedBlackNode *current = mpRoot;
    saveStep("Start searching. current = root", current->getVal(), 0, PSEUDO_SEARCH);

    while (current){
        if (val == current->getVal()){
            saveStep("Found node " + to_string(val) + "!", current->getVal(), 2, PSEUDO_SEARCH);
            return true;
        }
        
        else if (val < current->getVal()){
            saveStep("Compare " + to_string(val) + " < " + to_string(current->getVal()) + ". Go LEFT", current->getVal(), 3, PSEUDO_SEARCH);
            current = current->getLeft();
        }
        
        else {
            saveStep("Compare " + to_string(val) + " > " + to_string(current->getVal()) + ". Go RIGHT", current->getVal(), 4, PSEUDO_SEARCH);
            current = current->getRight();
        }
    }

    saveStep("Value " + to_string(val) + " not found in the tree.", -1, 5, PSEUDO_SEARCH);
    return false;
}
