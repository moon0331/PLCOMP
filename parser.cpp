#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>
#include <map>

#include "parser.h"

#define NUM_OF_COL    38
#define NUM_OF_STATES 177

// LENGTH of LR_TABLE: 38
//enum ENUM_LR_TABLE {
//    STATE, OPEN_P, CLOSE_P, TYPE, SEMI, COMMA, INT, CHAR, OPEN_B, CLOSE_B, IF,
//    THEN, ELSE, WHILE, EQUAL, RETURN, BIGGER, SMALLER, PLUS, MULTIPLY, N, W, DOLLAR,
//    P, DS, D, WS, V, B, SL, S, C, E, T, F, ACC
//};

//map<string, int> MAP_LR_TABLE;

string enum_to_str[] = {
    "STATE", "(", ")", "type", ";", ",", "INT", "CHAR", "{", "}", "IF", "THEN",
    "ELSE", "WHILE", "=", "RETURN", ">", "<", "+", "*", "num", "word", "$",
    "prog", "decls", "decl", "words", "vtype", "block", "slist", "stat", "cond",
    "expr", "term", "fact", "n", "w", "acc"
};

//int LR_TABLE[NUM_OF_STATES][NUM_OF_COL];


//const vector<string> ActionToken = {
//    "word",        "(",        ")",        "type",        ";",
//    ",",        "INT",        "CHAR",        "{",        "}",
//    "IF",        "THEN",        "ELSE",        "WHILE",    "=",
//    "RETURN",    ">",        "<",        "+",        "*",
//    "num",        "$"
//};
//
//const vector<string> GotoToken = {
//    "prog",        "decls",    "decl",        "words",    "vtype",
//    "block",    "slist",    "stat",        "cond",        "expr",
//    "term",        "fact"
//};

//extern vector<State> stateArr;

//vector<string> tok(string line) {
//    vector<string> v;
//    int len = (int)line.length();
//    string s = "";
//    int count = 0;
//    for (int i = 0; i <= len; i++) {
//        if (line[i] == ',' || line[i]=='\0') {
//            v.push_back(s);
//            s = "";
//            count++;
//        }
//        else {
//            s += line[i];
//        }
//    }
//    cout << count<<"th"<<endl;
//    v.erase(v.begin());
//    int size = (int)v.size(); //34
//    int newSize = (int)ActionToken.size() + (int)GotoToken.size(); //34
//    cout << size << newSize << endl;
//    for (int i = 0; i < size; i++) {
//        cout << "[" << i << "] " << v[i] << " | ";
//    }
//    return v;
//}

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
	sstack.push({ 0,"$" });
    
    /*
	ifstream file("Transition_table-v2.csv");
	for (int i = 0; i < NUM_OF_STATES; i++) {
		stateArr.push_back(State(i));
	}
	for (int i = 0; i < NUM_OF_STATES; i++) {
		string line;
		getline(file, line);
		vector<string> str = tok(line);
		int idx = 0;
		while (idx < ActionToken.size()) {
			cout << endl;
			cout << idx << "th: [" << ActionToken[idx] << "], [" << str[idx] << "]" << endl;
			stateArr[i].getActionTable().insert(pair<string, actNum>(ActionToken[idx], str[idx]));
			idx++;
		}
		cout << "\nPut into goto" << endl;
		while (idx < ActionToken.size() + GotoToken.size()) {
			int val = idx - (int)ActionToken.size();
			cout << "idx = " << idx << ", val = " << val << endl;
			cout << idx << "th : [" << GotoToken[val] << "], [" << str[idx] << "]" << endl;
			stateArr[i].getGotoTable().insert(pair<string, int>(GotoToken[val], atoi(str[idx].c_str()))); //atoi 수정 필요
			idx++;
		}
		cout << "TURN END" << endl;
	}
	cout << "BASIC END" << endl;
	file.close();
     */
}

void Parser::parse(ifstream& scanFile, ofstream& codeFile, vector<string>& inputTape) {
    vector<string>::iterator it = inputTape.begin();
    int current_state = 0;
    // SHIFT_REDUCE PARSING
    while(it!=end(inputTape)){
        string handle = *it;
        int nextDestination = LR_TABLE[current_state][MAP_LR_TABLE.find(handle)->second];
        cout<<handle<<endl;
		if (nextDestination >= 0){
			shift(nextDestination, handle);
			it = next(it, 1);
		}
		else {
			reduce(nextDestination);
		}
    }

//    while (!scanFile.eof()) {
//        string line;
//        getline(scanFile, line);
//        str += line;
//    }
//    str += "$";
//    cout << str << endl;
//
//    while (true) {
//        Tuple tuple = sstack.top();
//        int stateNo = tuple.stateNum;
//        string s_r = tuple.str;
//        break;
//    }
}

void Parser::shift(int nextDestination, string handle) {
	/*string s = "";
	int i = 0;
	while (str[0] != ' ') {
	str = str.substr(1);
	}
	for (i = 0; str[i] != ' '; i++) {
	s += str[i];
	}
	str = str.substr(i+1);
	sstack.push({});*/

	sstack.push({ nextDestination, handle });
}

void Parser::reduce(int nextDestination) {
	int current_state = abs(nextDestination);
	string lhs = grammars[current_state].left_side;
	vector<string> rhs = grammars[current_state].right_side;
	int size = rhs.size();

	while (size > 0) {
		sstack.pop();
		size--;
	}
	sstack.pop();
	int top_original = sstack.top().stateNum;
	current_state = LR_TABLE[top_original][MAP_LR_TABLE.find(lhs)->second];
	sstack.push({current_state, grammars[current_state].left_side });
}

bool Parser::isFinalState() {
	return true; //일단 나중에 수정해야
}
