/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */

#include "parser.h"

Parser::Parser(){
    symtable = new SymbolTable();
    last_deleted = -1;
}

void Parser::parse(vector<string> expression){
    vector<ExprTreeNode*> stk;

    for(int i = 2 ; i < expression.size() ; i++){
        string token = expression[i];

        if(token == "("){
            stk.push_back(nullptr);
        }else if(token == ")"){
            ExprTreeNode* rst = stk[stk.size() - 1];
            stk.pop_back();
            ExprTreeNode* root = stk[stk.size() - 1];
            stk.pop_back();
            ExprTreeNode* lst = stk[stk.size() - 1];
            stk.pop_back();

            stk.pop_back();

            root -> right = rst;
            root -> left = lst;

            stk.push_back(root); 
        }else{
            ExprTreeNode* node = nullptr;
            if(token == "+"){
                node = new ExprTreeNode("ADD" , -1);
            }else if(token == "-"){
                node = new ExprTreeNode("SUB" , -1);
            }else if(token == "*"){
                node = new ExprTreeNode("MUL" , -1);
            }else if(token == "/"){
                node = new ExprTreeNode("DIV" , -1);
            }else{
                int flag_address = symtable -> search(token);
                if(flag_address == -2){
                    node = new ExprTreeNode("VAL" , stoi(token));
                }else{
                    node = new ExprTreeNode("VAR" , -1);
                    node -> id = token;
                }
            }
            stk.push_back(node);
        }
    }
    string token0 = expression[0];
    ExprTreeNode* left_node = nullptr;

    if(token0 == "del"){
        left_node = new ExprTreeNode("DEL" , -1);
        last_deleted = symtable -> search(stk[0] -> id);

        symtable -> remove(stk[0] -> id);

    }else if(token0 == "ret"){
        left_node = new ExprTreeNode("RET" , -1);
    }else{
        left_node = new ExprTreeNode("VAR" , -1);
        left_node -> id = token0;
        symtable -> insert(token0);
    }

    ExprTreeNode* root_node = new ExprTreeNode(":=" , -1);

    root_node -> left = left_node;
    root_node -> right = stk[0];

    expr_trees.push_back(root_node);
}

void deleteTree(ExprTreeNode*& root){
    if(!root){
        return;
    }else{
        ExprTreeNode* left_child = root -> left;
        ExprTreeNode* right_child = root -> right;

        delete root;
        root = nullptr;
        
        deleteTree(left_child);
        deleteTree(right_child);
    }
}

Parser::~Parser(){
    for(int i = 0 ; i < expr_trees.size() ; i++){
        deleteTree(expr_trees[i]);
    }
    delete symtable;
}