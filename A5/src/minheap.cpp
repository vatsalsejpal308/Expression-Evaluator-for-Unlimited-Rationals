/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "minheap.h"

//Write your code below this line

int getHeight(HeapNode* root){
    int h = -1;
    while(root){
        h++;
        root = root -> left;
    }
    return h;
}

int pow(int a , int b){
    int ans = 1;
    for(int i = 0 ; i < b ; i++){
        ans = ans * a;
    }
    return ans;
}

HeapNode* getNode(HeapNode* root , int size , int h){
    int nLastLevel = size - (pow(2 , h) - 1);
    int maxLastLevel = pow(2 , h);
    if(nLastLevel == maxLastLevel){
        nLastLevel = 0;
        maxLastLevel = 2 * maxLastLevel;
    }

    HeapNode* curr = root;
    while(curr -> left && curr -> right){
        if(nLastLevel < maxLastLevel / 2){
            curr = curr -> left;
            nLastLevel = nLastLevel;
            maxLastLevel = maxLastLevel / 2;
        }else{
            curr = curr -> right;
            nLastLevel = nLastLevel - maxLastLevel / 2;
            maxLastLevel = maxLastLevel / 2;
        }
    }
    return curr;
}

HeapNode* getLastNode(HeapNode* root, int size , int h){
    int nLastLevel = size - (pow(2 , h) - 1);
    int maxLastLevel = pow(2 , h);

    HeapNode* curr = root , * parent = nullptr;
    while(curr){
        parent = curr;
        if(nLastLevel <= maxLastLevel / 2){
            curr = curr -> left;
            nLastLevel = nLastLevel;
            maxLastLevel = maxLastLevel / 2;
        }else{
            curr = curr -> right;
            nLastLevel = nLastLevel - maxLastLevel / 2;
            maxLastLevel = maxLastLevel / 2;
        }
    }
    return parent;
}

MinHeap::MinHeap(){
    size = 0;
    root = nullptr;
}

void MinHeap::push_heap(int num){
    HeapNode* node = new HeapNode(num);
    if(size == 0){
        root = node;
        size++;
        return;
    }
    int h = getHeight(root);

    HeapNode* parent_node = getNode(root , size , h);
    size++;
    if(!parent_node -> left){
        parent_node -> left = node;
    }else{
        parent_node -> right = node;
    }
    node -> par = parent_node;

    while(node -> par && node -> val < node -> par -> val){

        int swap = node -> val;
        node -> val = node -> par -> val;
        node -> par -> val = swap;

        node = node -> par;
    }

}

int MinHeap::get_min(){
    if(size == 0){
        return -1;
    }
    return root -> val;
}

void MinHeap::pop(){
    if (size == 0){
        return;
    }else if(size == 1){
        size = 0;
        delete root;
        root = nullptr;
        return;
    }

    int height = getHeight(root);
    HeapNode* nodeToDel = getLastNode(root , size , height);

    root -> val = nodeToDel -> val;
    if(nodeToDel == nodeToDel -> par -> left){
        nodeToDel -> par -> left = nullptr;
    }else{
        nodeToDel -> par -> right = nullptr;
    }
    delete nodeToDel;
    nodeToDel = nullptr;
    size--;

    HeapNode* curr = root;
    while(curr -> left){
        HeapNode* minNode = curr;
        if(minNode -> val > curr -> left -> val){
            minNode = curr -> left;
        }

        if(curr -> right && minNode -> val > curr -> right -> val){
            minNode = curr -> right;
        }

        if(minNode -> val == curr -> val){
            break;
        }else{
            int swap = curr -> val;
            curr -> val = minNode -> val;
            minNode -> val = swap;

            curr = minNode;
        }
    }
}

MinHeap::~MinHeap(){
    if(size != 0){
        HeapNode** heap = new HeapNode*[size];

        int s = 0 , e = 0;
        heap[e++] = root;
        
        while(s != e){
            int levelSize = e - s;
            for(int i = 0 ; i < levelSize ; i++){
                HeapNode* front_node = heap[s++];

                if(front_node -> left){
                    heap[e++] = front_node -> left;
                }

                if(front_node -> right){
                    heap[e++] = front_node -> right;
                }
            }
        }

        for(int i = 0 ; i < size ; i++){
            delete heap[i];
            heap[i] = nullptr;
        }

        delete [] heap;
    }else{
        root = nullptr;
    }
}
