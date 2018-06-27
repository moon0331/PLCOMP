#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>
#include <map>

#include "parser.h"

#define NUM_OF_COL    38
#define NUM_OF_STATES 177

string enum_to_str[] = {
    "STATE", "(", ")", "type", ";", ",", "INT", "CHAR", "{", "}", "IF", "THEN",
    "ELSE", "WHILE", "=", "RETURN", ">", "<", "+", "*", "num", "word", "$",
    "prog", "decls", "decl", "words", "vtype", "block", "slist", "stat", "cond",
    "expr", "term", "fact", "n", "w", "acc"
};

void Parser::create_LR_TABLE(){
    cout<<"READING TABLE FROM CSV"<<endl;
    for(int i=0; i<NUM_OF_STATES; i++){
        for(int j=0; j<NUM_OF_COL; j++){
            LR_TABLE[i][j] = 0;
        }
    }
    
    ifstream csv_read("Transition_table-v2.csv"); // CHANGE THE PATH HERE
    if(csv_read.is_open()){
        string line;
        while (!csv_read.eof()) {
            getline(csv_read, line);
            int line_length = (int)line.length();
            int state_num=0;
            
            int i;  // positioned at , or space character
            
            for(i=0; i<line_length; i++){
                if(line[i]==','){
                    state_num = stoi(line.substr(0, i));
                    LR_TABLE[state_num][0] = state_num;
                    i++;
                    break;
                }
            }
            
            int pos=1; // pointing at LR_TABLE column
            
            while(i<=line_length-1){
                int j;
                if(line[i]=='s' || line[i]=='r'){  // line[i] == 's'
                    int action_num;
                    for(j=i+1; j<line_length; j++){
                        if(line[j]==','){
                            j--;
                            break;
                        }
                    }
                    if(line[i]=='s'){
                        action_num = stoi(line.substr(i+1, j-i+1));
                    }
                    else{
                        action_num = -stoi(line.substr(i+1, j-i+1));
                    }
                    LR_TABLE[state_num][pos] = action_num;
//                    cout<<"STATE: "<<state_num<<" / ACTION: "<<enum_to_str[pos]<<" / ACTION_NUM: "<<action_num<<endl;
                    i = j+1;
                }
                else if(line[i]==','){
                    pos++;
                    i++;
                }
                else{  // line[i] is a number for a GOTO TABLE
                    int goto_num;
                    for(j=i+1; j<line_length; j++){
                        if(line[j]==','){
                            j--;
                            break;
                        }
                    }
                    string sub_str = line.substr(i, j-i+1);
                    if(sub_str!="acc" && sub_str!="\r" && sub_str!="\n"){
                        goto_num = stoi(sub_str);
                    }else if(sub_str=="acc"){  // acc
                        goto_num=777;
                    }else{  // \r or \n
                        break;
                    }
                    LR_TABLE[state_num][pos] = goto_num;
//                    cout<<"STATE: "<<state_num<<" / GOTO: "<<enum_to_str[pos]<<" / GOTO_NUM: "<<goto_num<<endl;
                    i=j+1;
                }
            }
        }
    }else{
        cout<<"FAILED TO OPEN CSV FILE."<<endl;
        exit(0);
    }
}

void Parser::create_grammar(){
    grammars.push_back(Grammar{"prog", vector<string>{"vtype", "w", "(", "words", ")", "block"}});
    
    grammars.push_back(Grammar{"decls", vector<string>{"decls", "decl"}});
    grammars.push_back(Grammar{"decls", vector<string>{""}});
    
    grammars.push_back(Grammar{"decl", vector<string>{"type", "words", ";"}});
    
    grammars.push_back(Grammar{"words", vector<string>{"words", ",", "w"}});
    grammars.push_back(Grammar{"words", vector<string>{"w"}});
    
    grammars.push_back(Grammar{"vtype", vector<string>{"INT"}});
    grammars.push_back(Grammar{"vtype", vector<string>{"CHAR"}});
    grammars.push_back(Grammar{"vtype", vector<string>{""}});
    
    grammars.push_back(Grammar{"block", vector<string>{"{", "decls", "slist", "}"}});
    grammars.push_back(Grammar{"block", vector<string>{""}});
    
    grammars.push_back(Grammar{"slist", vector<string>{"slist", "stat"}});
    grammars.push_back(Grammar{"slist", vector<string>{"stat"}});
    
    grammars.push_back(Grammar{"stat", vector<string>{"block"}});
    grammars.push_back(Grammar{"stat", vector<string>{"IF", "cond", "THEN", "block", "ELSE", "block"}});
    grammars.push_back(Grammar{"stat", vector<string>{"WHILE", "cond", "block"}});
    grammars.push_back(Grammar{"stat", vector<string>{"w", "=", "cond", ";"}});
    grammars.push_back(Grammar{"stat", vector<string>{"RETURN", "cond", ";"}});
    grammars.push_back(Grammar{"stat", vector<string>{""}});
    
    grammars.push_back(Grammar{"cond", vector<string>{"expr", ">", "expr"}});
    grammars.push_back(Grammar{"cond", vector<string>{"expr", "<", "expr"}});
    
    grammars.push_back(Grammar{"expr", vector<string>{"term"}});
    grammars.push_back(Grammar{"expr", vector<string>{"term", "+", "term"}});
    
    grammars.push_back(Grammar{"term", vector<string>{"fact"}});
    grammars.push_back(Grammar{"term", vector<string>{"fact", "*", "fact"}});
    
    grammars.push_back(Grammar{"fact", vector<string>{"n"}});
    grammars.push_back(Grammar{"fact", vector<string>{"w"}});
    
    grammars.push_back(Grammar{"n", vector<string>{"n", "num"}});
    grammars.push_back(Grammar{"n", vector<string>{""}});
    
    grammars.push_back(Grammar{"w", vector<string>{"w", "word"}});
    grammars.push_back(Grammar{"w", vector<string>{""}});
}

Parser::Parser() {
    create_LR_TABLE();
    create_grammar();
    
    for(int i=0; i<NUM_OF_COL; i++){
        MAP_LR_TABLE[enum_to_str[i]] =  i;
    }
    
	cout << "parser INITIALIZATION" << endl;
    sstack.push_back(new Tuple{0, "$", vector<Tuple*>{}});
}

void Parser::parse(vector<string>& inputTape) {
    vector<string>::iterator it = inputTape.begin();
    int current_state = 0;
    
    while(it!=end(inputTape)){
        string handle = *it;
        current_state = sstack.back()->stateNum;
        
        int nextDestination = LR_TABLE[current_state][MAP_LR_TABLE.find(handle)->second];
		if (nextDestination >= 0){
			shift(nextDestination, handle);
            cout<<"HANDLE: "<<handle<<endl;
			it = next(it, 1);
		}
		else {
			reduce(nextDestination);
            cout<<"REDUCE #: "<<abs(nextDestination)<<endl;
		}
    }
    if(it==end(inputTape) || (int)sstack.size()==0){
        // CHECK THE PARSE TREE HERE
        cout<<"ACCEPTED"<<endl;
    }
}

void Parser::shift(int nextDestination, string handle) {
    sstack.push_back(new Tuple{ nextDestination, handle, vector<Tuple*>{}});
}

void Parser::reduce(int nextDestination) {
	int current_state = abs(nextDestination);
	string lhs = grammars[current_state].left_side;
	vector<string> rhs = grammars[current_state].right_side;
	int size = (int)rhs.size();
    
    Tuple *newTuple = new Tuple{};
    
    // If rhs is empty, it should not be popped.
    for(int i=0; i<size; i++){
        if(rhs[i] == ""){
            // Connect empty tuple
            Tuple *empty_tuple = new Tuple{current_state, "", vector<Tuple*>{}};
            newTuple->children.insert(begin(newTuple->children), empty_tuple);
            continue;
        }
        Tuple *t = sstack[(int)sstack.size()-1];
        
        sstack.pop_back();
        // insert at front
        newTuple->children.insert(begin(newTuple->children), t);
//        cout<<"INSERTED: state #"<<t->stateNum<<endl;
    }

	int top_original = sstack.back()->stateNum;
	current_state = LR_TABLE[top_original][MAP_LR_TABLE.find(lhs)->second];
    newTuple->stateNum = current_state;
    newTuple->str = lhs;
    sstack.push_back(newTuple);
    cout<<"New Tuple Inserted: state #"<<newTuple->stateNum<<endl;
    cout<<"New Tuple Inserted: lhs: "<<newTuple->str<<endl<<endl;
}

bool Parser::isFinalState() {
	return true;
}

