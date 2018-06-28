#include "AST.h"

AST::AST(){
    root = NULL;
}

void AST::create_ast(vector<string>& inputTape){
    for(auto it=begin(inputTape); it!=end(inputTape); it++){
        add_node(*it);
    }
}

void AST::add_node(string val){
    if(root == NULL){
        Node *newNode = new Node();
        root = newNode;
    }
}
