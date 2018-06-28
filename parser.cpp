#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>
#include <map>

#include "parser.h"

//#define NUM_OF_COL    38
//#define NUM_OF_STATES 184

string enum_to_str[] = {
    "STATE", "(", ")", ";", ",", "INT", "CHAR", "{", "}", "IF", "THEN",
    "ELSE", "WHILE", "=", "RETURN", ">", "<", "+", "*", "num", "word", "$", "S",
    "prog", "decls", "decl", "words", "vtype", "block", "slist", "stat", "cond",
    "expr", "term", "fact", "n", "w", "acc"
};

void Parser::create_LR_TABLE(){
    cout<<"\nREADING TABLE FROM CSV"<<endl<<endl;
    for(int i=0; i<NUM_OF_STATES; i++){
        for(int j=0; j<NUM_OF_COL; j++){
            LR_TABLE[i][j] = 0;
        }
    }
    
    ifstream csv_read("Transition_table-v3.csv");
    // PARSE THE CSV FILE AND MAKE A LR_TABLE
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
    grammars.push_back(Grammar{"S", vector<string>{"prog"}});
    grammars.push_back(Grammar{"prog", vector<string>{"vtype", "w", "(", "words", ")", "block"}});
    
    grammars.push_back(Grammar{"decls", vector<string>{"decls", "decl"}});
    grammars.push_back(Grammar{"decls", vector<string>{""}});
    
    grammars.push_back(Grammar{"decl", vector<string>{"vtype", "words", ";"}});
    
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
        // COLUMN NUMBER THAT CORRESPONDS TO STRING
        MAP_LR_TABLE[enum_to_str[i]] =  i;
    }
    
	cout << "PARSER INITIALIZATION" << endl;
    sstack.push_back(new Tuple{0, "$", vector<Tuple*>{}});
}

void Parser::parse(vector<string>& inputTape) {
    vector<string>::iterator it = inputTape.begin();
    int current_state = 0;
    
    while(sstack.size()!=0){
        string handle;
        if(it == end(inputTape)){
            handle = "$";
        }else{
            handle = *it;
        }
        current_state = sstack.back()->stateNum;
        
        int nextDestination = LR_TABLE[current_state][MAP_LR_TABLE.find(handle)->second];
		if (nextDestination >= 0){
            if(nextDestination == 777){
                cout<<"\nACCEPTED"<<endl;
                
//                for(auto it=begin(sstack); it!=end(sstack); it++){
//                    cout<<"STACK: "<<(*it)->str<<endl;
//                }
                break;
            }
			shift(nextDestination, handle);
//            cout<<"HANDLE: "<<handle<<endl;
			it = next(it, 1);
		}
		else {
			reduce(nextDestination);
//            cout<<"REDUCE #: "<<abs(nextDestination)<<endl;
		}
    }
}

void Parser::shift(int nextDestination, string handle) {
    Tuple * temp = new Tuple{nextDestination, handle, vector<Tuple*>{}};
    sstack.push_back(temp);
}

void Parser::reduce(int nextDestination) {
	int current_state = abs(nextDestination);
	string lhs = grammars[current_state].left_side;  // LEFT GRAMMAR
	vector<string> rhs = grammars[current_state].right_side;  // RIGHT GRAMMAR
	int size = (int)rhs.size();
    
    Tuple *newTuple = new Tuple{};  // TUPLE TO BE ADDED
    
    // If rhs is empty, it should not be popped.
    for(int i=0; i<size; i++){
        if(rhs[i] == ""){
            // Empty tuple is inserted
            Tuple *empty_tuple = new Tuple{current_state, "", vector<Tuple*>{}};
            newTuple->children.insert(begin(newTuple->children), empty_tuple);
            continue;
        }
        Tuple *t = sstack[(int)sstack.size()-1];  // the last element to be popped
        sstack.pop_back();
        
        // Insert at front
        newTuple->children.insert(begin(newTuple->children), t);
    }

	int top_original = sstack.back()->stateNum;
	current_state = LR_TABLE[top_original][MAP_LR_TABLE.find(lhs)->second];  // next state to go
    newTuple->stateNum = current_state;
    newTuple->str = lhs;
    sstack.push_back(newTuple);
}

bool Parser::isFinalState() {
	return true;
}

