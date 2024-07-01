/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "eppcompiler.h"
#include<iostream>
//Write your code below this line

EPPCompiler::EPPCompiler(){
    memory_size = 0;
    output_file = "";
}

EPPCompiler::EPPCompiler(string out_file,int mem_limit){
    memory_size = mem_limit;
    output_file = out_file;

    for(int i = 0 ; i < mem_limit ; i++){
        least_mem_loc.push_heap(i);
    }
}

void EPPCompiler::compile(vector<vector<string>> code){
    int size = code.size();
    vector<string> all_commands;
    
    for(int i = 0 ; i < size ; i++){
        targ.parse(code[i]);
        string token = code[i][0];

        if(token == "del"){
            int index = targ.last_deleted;
            least_mem_loc.push_heap(index);
            all_commands.push_back("DEL = mem[" + to_string(index) + "]");
        }else if(token == "ret"){
            vector<string> targ_commands = generate_targ_commands();
            for(int j = targ_commands.size() - 1 ; j >= 0 ; j--){
                all_commands.push_back(targ_commands[j]);
            }
            break;
        }else{
            int index = targ.symtable -> search(token);
            if(index == -1){
                index = least_mem_loc.get_min();
                least_mem_loc.pop();
            }
            targ.symtable -> assign_address(token , index);

            vector<string> targ_commands = generate_targ_commands();
            for(int j = targ_commands.size() - 1 ; j >= 0 ; j--){
                all_commands.push_back(targ_commands[j]);
            }
        }
    }
    write_to_file(all_commands);
}

void traversalHelper(vector<string>& commands , ExprTreeNode* node , SymbolTable*& symbol_table){
    if(!node){
        return;
    }else{
        if(node -> type == "ADD" || node -> type == "SUB" || node -> type == "MUL" || node -> type == "DIV" ){
            commands.push_back(node -> type);
        }else if(node -> type == "VAL"){
            commands.push_back("PUSH " + to_string(node -> num));
        }else{
            int index = symbol_table -> search(node -> id);
            commands.push_back("PUSH mem[" + to_string(index) + "]");
        }

        traversalHelper(commands , node -> left , symbol_table);
        traversalHelper(commands , node -> right , symbol_table);
    }
}

vector<string> EPPCompiler::generate_targ_commands(){
    vector<string> commands;
    ExprTreeNode* root = targ.expr_trees[targ.expr_trees.size() - 1];

    string left_type = root -> left -> type;
    if(left_type == "RET"){
        commands.push_back("RET = POP");
        traversalHelper(commands , root -> right , targ.symtable);
    }else{
        int index = targ.symtable -> search(root -> left -> id);
        commands.push_back("mem[" + to_string(index) + "] = POP");
        traversalHelper(commands , root -> right , targ.symtable);
    }
    return commands;
}

void EPPCompiler::write_to_file(vector<string> commands){
    ofstream outputFile(output_file);
    for(int i = 0 ; i < commands.size() ; i++){
        outputFile<<commands[i]<<endl;
    }
    outputFile.close();
}

EPPCompiler::~EPPCompiler(){
    //no code required
}

int main(){
    return 0;
}
