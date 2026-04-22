#pragma once

enum class Color { RED, BLACK };

class RedBlackNode {
private:
    int mVal;
    Color mColor;
    RedBlackNode *mpLeft, *mpRight, *mpParent;
    bool mIsDummy;
    
public:
    RedBlackNode(int val);
    
    int getVal() const;
    Color getColor() const;
    RedBlackNode *getLeft() const;
    RedBlackNode *getRight() const;
    RedBlackNode *getParent() const;

    void setVal(int val);
    void setColor(Color color);
    void setLeft(RedBlackNode *left);
    void setRight(RedBlackNode *right);
    void setParent(RedBlackNode *parent);
    
    bool isRed() const;
    
    bool isDummy() const;
    void setDummy(bool dummy);
};
