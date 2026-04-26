#include "Common.h"
#include "RedBlackTree/RedBlackNode.h"

RedBlackNode::RedBlackNode(int val){
    mVal = val;
    mColor = Color::RED;
    mpLeft = nullptr;
    mpRight = nullptr;
    mpParent = nullptr;
    mIsDummy = false;
}

int RedBlackNode::getVal() const {return mVal;}

Color RedBlackNode::getColor() const {return mColor;}

RedBlackNode *RedBlackNode::getLeft() const {return mpLeft;}

RedBlackNode *RedBlackNode::getRight() const {return mpRight;}

RedBlackNode *RedBlackNode::getParent() const {return mpParent;}

void RedBlackNode::setVal(int val){
    mVal = val;
}

void RedBlackNode::setColor(Color color){
    mColor = color;
}

void RedBlackNode::setLeft(RedBlackNode *left){
    mpLeft = left;
}

void RedBlackNode::setRight(RedBlackNode *right){
    mpRight = right;
}

void RedBlackNode::setParent(RedBlackNode *parent){
    mpParent = parent;
}

bool RedBlackNode::isRed() const {
    return mColor == Color::RED;
}

bool RedBlackNode::isDummy() const {
    return mIsDummy;
}

void RedBlackNode::setDummy(bool dummy){
    mIsDummy = dummy;
}
