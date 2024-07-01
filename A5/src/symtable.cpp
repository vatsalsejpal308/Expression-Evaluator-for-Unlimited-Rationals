/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "symtable.h"

//Write your code below this line

SymbolTable::SymbolTable(){
    size = 0;
    root = nullptr;
}

int balanceCoefficient(SymNode* node){
    int leftHeight = -1 , rightHeight = -1;
    if(node -> left){
        leftHeight = node -> left -> height;
    }
    if(node -> right){
        rightHeight = node -> right -> height;
    }
    return leftHeight - rightHeight;
}

void setterHeight(SymNode* node){
    int leftHeight = -1 , rightHeight = -1;
    if(node -> left){
        leftHeight = node -> left -> height;
    }
    if(node -> right){
        rightHeight = node -> right -> height;
    }
    node -> height = 1 + max(leftHeight , rightHeight);
}

SymNode* getNode(string& k , SymNode* root){
    SymNode* curr = root;
    while(curr){
        if(curr -> key == k){
            return curr;
        }else if(curr -> key < k){
            curr = curr -> right;
        }else{
            curr = curr -> left;
        }
    }
    return nullptr;
}

SymNode* insertHelper(SymNode* node , string& k , int& size){
    if(!node){
        size++;
        return new SymNode(k);
    }

    if(k < node -> key){
        SymNode* child = insertHelper(node -> left , k , size);
        node -> left = child;
        child -> par = node;
    }else if(k > node -> key){
        SymNode* child = insertHelper(node -> right , k , size);
        node -> right = child;
        child -> par = node;
    }else{
        return node;
    }

    setterHeight(node);
    int coefficient = balanceCoefficient(node);

    if(coefficient > 1 && node -> left){
        if(k < node -> left -> key){
            node = node -> RightRightRotation();
        }else{
            node = node -> LeftRightRotation();
        }
    }

    if(coefficient < -1 && node -> right){
        if(k > node -> right -> key){
            node = node -> LeftLeftRotation();
        }else{
            node = node -> LeftRightRotation();
        }
    }

    return node;
}

void SymbolTable::insert(string k){
    root = insertHelper(root , k , size);
}

SymNode* findMin(SymNode* node){
    while(node -> left){
        node = node -> left;
    }
    return node;
}

SymNode* removeHelper(SymNode* node , string& k , int& size){
    if(!node){
        return nullptr;
    }

    if(k < node -> key){
        node -> left = removeHelper(node -> left , k , size);
    }else if(k > node -> key){
        node -> right = removeHelper(node -> right , k , size);
    }else{
        if(!node -> left || !node -> right){
            SymNode* temp = nullptr;
            if(node -> left){
                temp = node -> left;
            }else{
                temp = node -> right;
            }

            if(!temp){
                temp = node;
                node = nullptr;
            }else{
                *node = *temp;
            }
            
            delete temp;
            temp = nullptr;
            size--;
        }else{
            SymNode* temp = findMin(node -> right);
            node -> key = temp -> key;
            node -> address = temp -> address;
            node -> right = removeHelper(node -> right , temp -> key , size);
        }
    }

    if(!node){
        return nullptr;
    }

    setterHeight(node);
    int coefficient = balanceCoefficient(node);

    if(coefficient > 1 && node -> left){
        if(balanceCoefficient(node -> left) >= 0){
            node = node -> RightRightRotation();
        }else{
            node = node -> LeftRightRotation();
        }
    }

    if(coefficient < -1 && node -> right){
        if(balanceCoefficient(node -> right) <= 0){
            node = node -> LeftLeftRotation();
        }else{
            node = node -> RightLeftRotation();
        }
    }

    return node;

}

void SymbolTable::remove(string k){
    root = removeHelper(root , k , size);
}

int SymbolTable::search(string k){
    SymNode* node = getNode(k , root);
    if(!node){
        return -2;
    }else{
        return node -> address;
    }
}

void SymbolTable::assign_address(string k,int idx){
    SymNode* node = getNode(k , root);
    if(node){
        node -> address = idx;
    }
}

int SymbolTable::get_size(){
    return size;
}

SymNode* SymbolTable::get_root(){
    return root;
}

SymbolTable::~SymbolTable(){
    if(!root){
        return;
    }
    SymNode** queue = new SymNode*[size];
    int s = 0 , e = 0;
    queue[e++] = root;

    while(s != e){
        int levelSize = e - s;
        for(int i = 0 ; i < levelSize ; i++){
            SymNode* front_node = queue[s++];
            if(front_node -> left){
                queue[e++] = front_node -> left;
            }
            if(front_node -> right){
                queue[e++] = front_node -> right;
            }
        }
    }

    for(int i = 0 ; i < size ; i++){
        if(queue[i]){
            delete queue[i];
            queue[i] = nullptr;
        }
    }

    delete [] queue;
}
