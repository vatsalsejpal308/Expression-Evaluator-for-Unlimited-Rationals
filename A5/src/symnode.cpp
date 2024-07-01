/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "symnode.h"
//Write your code below this line

SymNode::SymNode(){
    key = "";
    height = -1;
    address = -1;

    par = nullptr;
    left = nullptr;
    right = nullptr;
}

SymNode::SymNode(string k){
    key = k;
    height = 0;
    address = -1;

    par = nullptr;
    left = nullptr;
    right = nullptr;
}

void setHeight(SymNode* node){
    int leftHeight = -1 , rightHeight = -1;
    if(node -> left){
        leftHeight = node -> left -> height;
    }
    if(node -> right){
        rightHeight = node -> right -> height;
    }
    node -> height = 1 + max(leftHeight , rightHeight);
}

SymNode* SymNode::LeftLeftRotation(){
    if(!this){
        return nullptr;
    }

    if(!this -> right){
        return this;
    }

    SymNode* node = this -> right;

    this -> right = node -> left;

    if(node -> left){
        node -> left -> par = this;
    }

    node -> left = this;
    par = node;

    setHeight(this);
    setHeight(node);

    return node;
}

SymNode* SymNode::RightRightRotation(){
    if(!this){
        return nullptr;
    }

    if(!this -> left){
        return this;
    }

    SymNode* node = left;

    left = node -> right;
    if(node -> right){
        node -> right -> par = this;
    }

    node -> right = this;
    par = node;

    setHeight(this);
    setHeight(node);

    return node;
}

SymNode* SymNode::LeftRightRotation(){
    left = left -> LeftLeftRotation();
    return RightRightRotation();
}

SymNode* SymNode::RightLeftRotation(){
    right = right -> RightRightRotation();
    return LeftLeftRotation();
}

SymNode::~SymNode(){

}