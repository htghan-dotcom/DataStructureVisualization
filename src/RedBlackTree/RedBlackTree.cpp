#include "Common.h"
#include "RedBlackTree.h"

RedBlackTree::RedBlackTree(){
    mpRoot = nullptr;
    mCurrentStep = -1;
    mVisualizationSpeed = 500;
}

RedBlackTree::~RedBlackTree(){
    clearTree(mpRoot);
}

void RedBlackTree::rotateLeft(RedBlackNode *x){
    RedBlackNode *y = x -> getRight();
    
    x -> setRight(y -> getLeft());
    if (y -> getLeft()){
        y -> getLeft() -> setParent(x);
    }
    
    y -> setParent(x -> getParent());
    
    if (x == mpRoot){
        mpRoot = y;
    }
    
    else if (x == x -> getParent() -> getLeft()){
        x -> getParent() -> setLeft(y);
    }
    
    else {
        x -> getParent() -> setRight(y);
    }
    
    y -> setLeft(x);
    x -> setParent(y);
    
    saveStep("Rotate left at node " + to_string(x -> getVal()), x -> getVal());
}

void RedBlackTree::rotateRight(RedBlackNode *x){
    RedBlackNode *y = x -> getLeft();
    
    x -> setLeft(y -> getRight());
    if (y -> getRight()){
        y -> getRight() -> setParent(x);
    }
    
    y -> setParent(x -> getParent());
    
    if (x == mpRoot){
        mpRoot = y;
    }
    
    else if (x == x -> getParent() -> getRight()){
        x -> getParent() -> setRight(y);
    }
    
    else {
        x -> getParent() -> setLeft(y);
    }
    
    y -> setRight(x);
    x -> setParent(y);
    
    saveStep("Rotate right at node " + to_string(x -> getVal()), x -> getVal());
}

void RedBlackTree::fixInsertion(RedBlackNode *node){
    if (!node -> getParent() or !node -> getParent() -> isRed()){
        mpRoot -> setColor(Color::BLACK);
        return;
    }
    
    RedBlackNode *parent = node -> getParent();
    RedBlackNode *grandparent = parent -> getParent();
    
    if (parent == grandparent -> getLeft()){
        RedBlackNode *uncle = grandparent -> getRight();
        
        // Case 1: uncle is Black - rotate
        if (!uncle or !uncle -> isRed()){
            if (node == parent -> getRight()){
                rotateLeft(parent);
                fixInsertion(parent);
            }
            else {
                parent -> setColor(Color::BLACK);
                grandparent -> setColor(Color::RED);
                rotateRight(grandparent);
            }
        }
        // Case 2: uncle is Red - recolor
        else {
            parent -> setColor(Color::BLACK);
            uncle -> setColor(Color::BLACK);
            grandparent -> setColor(Color::RED);
            
            fixInsertion(grandparent);
        }
    }
    else {
        RedBlackNode *uncle = grandparent -> getLeft();
        if (!uncle or !uncle -> isRed()){
            if (node == parent -> getLeft()){
                rotateRight(parent);
                fixInsertion(parent);
            }
            else {
                parent -> setColor(Color::BLACK);
                grandparent -> setColor(Color::RED);
                rotateLeft(grandparent);
            }
        }
        else {
            parent -> setColor(Color::BLACK);
            uncle -> setColor(Color::BLACK);
            grandparent -> setColor(Color::RED);
            
            fixInsertion(grandparent);
        }
    }
    
    mpRoot -> setColor(Color::BLACK);
}

void RedBlackTree::fixDeletion(RedBlackNode *node){
    if (!node){return;}
    
    if (node == mpRoot){
        node -> setColor(Color::BLACK);
        return;
    }
    
    RedBlackNode *parent = node -> getParent();
    if (!parent){return;}
    RedBlackNode *sibling;
    
    if (node == parent -> getLeft()){
        sibling = parent -> getRight();
        
        if (!sibling){
            fixDeletion(parent);
            return;
        }
        
        if (sibling and sibling -> isRed()){
            saveStep("Sibling RED - rotate left at parent", parent -> getVal());
            parent -> setColor(Color::RED);
            sibling -> setColor(Color::BLACK);
            rotateLeft(parent);
            fixDeletion(node);
            return;
        }
        
        if (sibling and (!sibling -> getLeft() or !sibling -> getLeft() -> isRed()) and (!sibling -> getRight() or !sibling -> getRight() -> isRed())){
            saveStep("Sibling BLACK, both nephews BLACK - recolor sibling", sibling -> getVal());
            sibling -> setColor(Color::RED);
            
            if (parent -> isRed()){
                saveStep("parent RED - done", parent -> getVal());
                parent -> setColor(Color::BLACK);
            }
            else {
                saveStep("parent BLACK - recurse on parern", parent -> getVal());
                fixDeletion(parent);
            }
            return;
        }
        
        saveStep("Sibling BLACK with nephew RED", sibling -> getVal());
        if (!sibling -> getRight() or !sibling -> getRight() -> isRed()){
            saveStep("SIbling is right child and its left child is RED - rotate right at sibling", sibling -> getVal());
            sibling -> getLeft() -> setColor(Color::BLACK);
            sibling -> setColor(Color::RED);
            rotateRight(sibling);
        }
        
        saveStep("Sibling is right child and its right child is RED - rotate left at parent", parent -> getVal());
        sibling -> setColor(parent -> getColor());
        parent -> setColor(Color::BLACK);
        sibling -> getRight() -> setColor(Color::BLACK);
        rotateLeft(parent);
        
        if (node){node -> setColor(Color::BLACK);}
    }
    else {
        sibling = parent -> getLeft();
        
        if (!sibling){
            fixDeletion(parent);
            return;
        }
        
        if (sibling and sibling -> isRed()){
            saveStep("Sibling RED - rotate right at parent", parent -> getVal());
            parent -> setColor(Color::RED);
            sibling -> setColor(Color::BLACK);
            rotateRight(parent);
            fixDeletion(node);
            return;
        }
        
        if (sibling and (!sibling -> getRight() or !sibling -> getRight() -> isRed()) and (!sibling -> getLeft() or !sibling -> getLeft() -> isRed())){
            saveStep("Sibling BLACK, both nephews BLACK - recolor sibling", sibling -> getVal());
            sibling -> setColor(Color::RED);
            
            if (parent -> isRed()){
                saveStep("parent RED - done", parent -> getVal());
                parent -> setColor(Color::BLACK);
            }
            else {
                saveStep("parent BLACK - recurse on parent", parent -> getVal());
                fixDeletion(parent);
            }
            return;
        }
        
        saveStep("Sibling BLACK with nephew RED", sibling -> getVal());
        if (!sibling -> getLeft() or !sibling -> getLeft() -> isRed()){
            saveStep("SIbling is left child and its right child is RED - rotate left at sibling", sibling -> getVal());
            sibling -> getLeft() -> setColor(Color::BLACK);
            sibling -> setColor(Color::RED);
            rotateLeft(sibling);
        }
        
        saveStep("Sibling is left child and its left child is RED - rotate right at parent", parent -> getVal());
        sibling -> setColor(parent -> getColor());
        parent -> setColor(Color::BLACK);
        sibling -> getRight() -> setColor(Color::BLACK);
        rotateRight(parent);
        
        if (node){node -> setColor(Color::BLACK);}
    }
}

RedBlackNode *RedBlackTree::findMin(RedBlackNode *root){
    if (!root){return nullptr;}
    
    RedBlackNode *current = root;
    while (current -> getLeft()){
        current = current -> getLeft();
    }
    
    return current;
}

RedBlackNode *RedBlackTree::findMax(RedBlackNode *root){
    if (!root){return nullptr;}
    
    RedBlackNode *current = root;
    while (current -> getRight()){
        current = current -> getRight();
    }
    
    return current;
}

RedBlackNode *RedBlackTree::searchNode(int val){
    RedBlackNode *current = mpRoot;
    saveStep("Search for " + to_string(val), -1);
    
    while (current){
        if (current -> getVal() == val){
            saveStep("Found node " + to_string(val), val);
            return current;

        }
        if (val < current -> getVal()){
            current = current -> getLeft();
        }
        else {
            current = current -> getRight();
        }
    }
    
    saveStep("Value " + to_string(val) + " not found", -1);
    return nullptr;
}

void RedBlackTree::transplant(RedBlackNode *x, RedBlackNode *y){
    if (x == mpRoot){
        mpRoot = y;
    }
    else if (x == x -> getParent() -> getLeft()){
        x -> getParent() -> setLeft(y);
    }
    else {
        x -> getParent() -> setRight(y);
    }
    
    if (y){
        y -> setParent(x -> getParent());
    }
}


void RedBlackTree::clearTree(RedBlackNode *root){
    if (!root){return;}
    
    clearTree(root -> getLeft());
    clearTree(root -> getRight());
    
    delete root;
}

void RedBlackTree::inorderCollect(RedBlackNode *root, vector<int>& data, vector<string>& colors){
    if (!root){return;}
    
    inorderCollect(root -> getLeft(), data, colors);
    
    data.push_back(root -> getVal());
    if (root -> isRed()){
        colors.push_back("RED");
    }
    else {
        colors.push_back("BLACK");
    }
    
    inorderCollect(root -> getRight(), data, colors);
}

int RedBlackTree::getHeight(RedBlackNode *root){
    if (!root){return 0;}
    
    return max(getHeight(root -> getLeft()), getHeight(root -> getRight())) + 1;
}

int RedBlackTree::getSize(RedBlackNode *root){
    if (!root){return 0;}
    
    return 1 + getSize(root -> getLeft()) + getSize(root -> getRight());
}

void RedBlackTree::saveStep(string description, int highlightedNode){
    StepState step;
    step.description = description;
    step.highlightedNode = highlightedNode;
    
    inorderCollect(mpRoot, step.treeData, step.nodeColors);
    
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

void RedBlackTree::initializeRandom(int count, int minVal, int maxVal){
    initialize();
    
    random_device rd;
    mt19937 gen(rd());
    
    if (count <= 0){
        cout << "Invalid count: " << count;
        
        uniform_int_distribution<> countDist(1, 50);
        count = countDist(gen);
        
        cout << ". Using random count = " << count << endl;
    }
    
    if (minVal > maxVal or (minVal == 0 and maxVal == 0)){
        uniform_int_distribution<> rangeDist(1, 1000);
        minVal = rangeDist(gen);
        maxVal = minVal + count + rangeDist(gen);
        
        cout << "Invalid range. Using random min = " << minVal << " and max = " << maxVal << endl;
    }
    
    uniform_int_distribution<> valueDist(minVal, maxVal);
    
    int inserted = 0;
    for (int i = 0; i<count; i++){
        int val = valueDist(gen);
        if (insert(val)){
            inserted++;
        }
    }
    
    cout << "Successfully randomly generated tree with " << inserted << " values" << endl;
    saveStep("Random init: " + to_string(inserted) + "values");
}

bool RedBlackTree::insert(int val){
    saveStep("Inserting " + to_string(val));
    
    RedBlackNode *newNode = new RedBlackNode(val);
    
    if (!mpRoot){
        mpRoot = newNode;
        mpRoot -> setColor(Color::BLACK);
        
        saveStep("Inserted as root (black)", val);
        
        return true;
    }
    
    // Find insertion node
    RedBlackNode *current = mpRoot;
    RedBlackNode *parent = nullptr;
    
    while (current){
        parent = current;
        
        if (val < current -> getVal()){
            current = current -> getLeft();
        }
        else if (val > current -> getVal()){
            current = current -> getRight();
        }
        else {
            delete newNode;
            saveStep("Insertion failed - duplicate value " + to_string(val));
            return false;
        }
    }
    
    // Insert node
    newNode -> setParent(parent);
    if (val < parent -> getVal()){
        parent -> setLeft(newNode);
    }
    else {
        parent -> setRight(newNode);
    }
    
    saveStep("Inserted node " + to_string(val) + " (red)", val);
    
    fixInsertion(newNode);
    
    return true;
}

bool RedBlackTree::remove(int val){
    saveStep("Removing " + to_string(val));
    
    RedBlackNode *targetNode = searchNode(val);
    if (!targetNode){
        saveStep("Remove failed - value " + to_string(val) + " not found");
        return false;
    }
    
    RedBlackNode *replaceNode = nullptr;
    Color originalColor = targetNode -> getColor();
    
    if (!targetNode -> getLeft() and !targetNode -> getRight()){
        saveStep("Node is leaf", targetNode -> getVal());
        replaceNode = nullptr;
        transplant(targetNode, nullptr);
    }
    
    else if (!targetNode -> getLeft()){
        saveStep("Node only has right child", targetNode -> getVal());
        replaceNode = targetNode -> getRight();
        transplant(targetNode, targetNode -> getRight());
    }
    
    else if (!targetNode -> getRight()){
        saveStep("Node only has left child", targetNode -> getVal());
        replaceNode = targetNode -> getLeft();
        transplant(targetNode, targetNode -> getLeft());
    }
    
    else {
        saveStep("Node has two chilrend - finding successor", targetNode -> getVal());
        RedBlackNode *successor = findMin(targetNode -> getRight());
        originalColor = successor -> getColor();
        replaceNode = successor -> getRight();
        
        saveStep("Successor found " + to_string(successor -> getVal()), successor -> getVal());
        
        if (successor == targetNode -> getRight()){
            replaceNode = successor -> getRight();
        }
        else {
            transplant(successor, successor -> getRight());
            successor -> setRight(targetNode -> getRight());
            successor -> getRight() -> setParent(successor);
        }
        
        transplant(targetNode, successor);
        
        successor -> setLeft(targetNode -> getLeft());
        successor -> getLeft() -> setParent(successor);
        
        successor -> setColor(targetNode -> getColor());
        
        saveStep("Replace node with successor", successor -> getVal());
    }
    
    delete targetNode;
    saveStep("Node deleted");
    
    if (originalColor == Color::BLACK){
        saveStep("Delete BLACK node - fixing double BLACK");
        fixDeletion(replaceNode);
    }
    else {
        saveStep("Delete RED node - no fix needed");
    }
    
    if (mpRoot){mpRoot -> setColor(Color::BLACK);}
    saveStep("Removal completed");
    
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
            saveStep("Update successful: " + to_string(oldVal) + " -> " + to_string(newVal), newVal);
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
        displayCurrentStep();
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
        displayCurrentStep();
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
    displayCurrentStep();
}

void RedBlackTree::runAtOnce(){
    if (mStepHistory.empty()){
        cout << "No steps to visualize" << endl;
        return;
    }
    
    cout << "Run visualization from step 0 to " << static_cast<int>(mStepHistory.size() - 1) << endl;
    
    for (int i = 0; i<static_cast<int>(mStepHistory.size() - 1); i++){
        mCurrentStep = i;
        displayCurrentStep();
        
        this_thread::sleep_for((chrono::milliseconds(mVisualizationSpeed)));
        
        cout << string(50, '-') << endl;
        
    }
}

void RedBlackTree::setVisualizationSpeed(int speed){
    if (speed >= 100 and speed <= 3000){
        mVisualizationSpeed = speed;
    }
    else {
        cout << "Speed must be between 100 and 3000ms" << endl;
    }
}

void RedBlackTree::display(){
    if (!mpRoot){
        cout << "Tree is empty" << endl;
        return;
    }
    
    cout << "Red-Black Tree (inorder traversal)" << endl;
    vector<int> data;
    vector<string> colors;
    inorderCollect(mpRoot, data, colors);
    
    for (size_t i = 0; i<data.size(); i++){
        cout << data[i] << " (" << colors[i] << ") ";
    }
    cout << endl;
    cout << "Tree height: " << getTreeHeight() << endl;
    cout << "Node count: " << getNodeCount() << endl;
}

void RedBlackTree::displayCurrentStep(){
    if (mStepHistory.empty() or mCurrentStep < 0 or mCurrentStep >= static_cast<int>(mStepHistory.size())){
        cout << "No step to display" << endl;
        return;
    }
    
    StepState& step = mStepHistory[mCurrentStep];
    
    cout << "=== Step " << mCurrentStep << " ===" << endl;
    cout << "Description: " << step.description << endl;
    cout << "Tree: ";
    
    for (size_t i = 0; i < step.treeData.size(); i++) {
        if (step.treeData[i] == step.highlightedNode) {
            cout << "[" << step.treeData[i] << "(" << step.nodeColors[i] << ")] ";
        }
        else {
            cout << step.treeData[i] << "(" << step.nodeColors[i] << ") ";
        }
    }
    cout << endl;
}

void RedBlackTree::displayStepInfo() {
    cout << "Current step: " << mCurrentStep + 1 << "/" << mStepHistory.size() << endl;
    cout << "Visualization speed: " << mVisualizationSpeed << "ms" << endl;
}

bool RedBlackTree::isEmpty(){
    return mpRoot == nullptr;
}

int RedBlackTree::getRootData(){
    if (!mpRoot){
        return -1;
    }
    
    return mpRoot -> getVal();
}

int RedBlackTree::getTreeHeight(){
    return getHeight(mpRoot);
}

int RedBlackTree::getNodeCount(){
    return getSize(mpRoot);
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

string RedBlackTree::exportToJSON() {
    string json = "{\n";
    json += "  \"steps\": [\n";
    
    for (size_t i = 0; i < mStepHistory.size(); i++) {
        StepState& step = mStepHistory[i];
        json += "    {\n";
        
        // Description (thoát ký tự đặc biệt)
        string desc = step.description;
        size_t pos = 0;
        while ((pos = desc.find('"', pos)) != string::npos) {
            desc.replace(pos, 1, "\\\"");
            pos += 2;
        }
        json += "      \"description\": \"" + desc + "\",\n";
        
        // Highlighted node
        json += "      \"highlightedNode\": " + to_string(step.highlightedNode) + ",\n";
        
        // Tree data
        json += "      \"treeData\": [";
        for (size_t j = 0; j < step.treeData.size(); j++) {
            json += to_string(step.treeData[j]);
            if (j < step.treeData.size() - 1) json += ", ";
        }
        json += "],\n";
        
        // Node colors
        json += "      \"nodeColors\": [";
        for (size_t j = 0; j < step.nodeColors.size(); j++) {
            json += "\"" + step.nodeColors[j] + "\"";
            if (j < step.nodeColors.size() - 1) json += ", ";
        }
        json += "]\n";
        
        json += "    }";
        if (i < mStepHistory.size() - 1) json += ",";
        json += "\n";
    }
    
    json += "  ]\n";
    json += "}\n";
    return json;
}


