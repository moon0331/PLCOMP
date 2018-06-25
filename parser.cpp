#include <iostream>
#include <string>
#include <fstream>

#include "parser.h"

#define NUM_OF_STATES 141
#define NUM_OF_ACTION 22
#define NUM_OF_GOTO   12
#define NUM_OF_COL    35

// LENGTH of LR_TABLE: 35
enum ENUM_LR_TABLE {
    STATE, WORD, OPEN_P, CLOSE_P, TYPE, SEMI, COMMA, INT, CHAR, OPEN_B, CLOSE_B, IF,
    THEN, ELSE, WHILE, EQUAL, RETURN, BIGGER, SMALLER, PLUS, MULTIPLY, NUM, DOLLAR,
    P, DS, D, WS, V, B, SL, S, C, E, T, F
};

string enum_to_str[] = {
    "STATE", "word", "(", ")", "type", ";", ",", "INT", "CHAR", "{", "}", "IF",
    "THEN", "ELSE", "WHILE", "=", "RETURN", ">", "<", "+", "*", "num", "$",
    "prog", "decls", "decl", "words", "vtype", "block", "slist", "stat", "cond",
    "expr", "term", "fact"
};

int LR_TABLE[NUM_OF_STATES][NUM_OF_COL];

const vector<string> ActionToken = {
	"word",		"(",		")",		"type",		";",
	",",		"INT",		"CHAR",		"{",		"}",
	"IF",		"THEN",		"ELSE",		"WHILE",	"=",
	"RETURN",	">",		"<",		"+",		"*",
	"num",		"$"
};

const vector<string> GotoToken = {
	"prog",		"decls",	"decl",		"words",	"vtype",
	"block",	"slist",	"stat",		"cond",		"expr",
	"term",		"fact"
};

extern vector<State> stateArr;

vector<string> tok(string line) {
	vector<string> v;
	int len = (int)line.length();
	string s = "";
	int count = 0;
	for (int i = 0; i <= len; i++) {
		if (line[i] == ',' || line[i]=='\0') {
			v.push_back(s);
			s = "";
			count++;
		}
		else {
			s += line[i];
		}
	}
	cout << count<<"th"<<endl;
	v.erase(v.begin());
	int size = (int)v.size(); //34
	int newSize = (int)ActionToken.size() + (int)GotoToken.size(); //34
	cout << size << newSize << endl;
	for (int i = 0; i < size; i++) {
		cout << "[" << i << "] " << v[i] << " | ";
	}
	return v;
}

Parser::Parser() {
    // TRANSITION TABLE CREATION //////////////////////////////////////////////////////////////////////////
    cout<<"READING TABLE FROM CSV"<<endl;
    ifstream csv_read("/Users/BaeHaneul/Developer/PLCOMP/PLCOMP/Transition_table.csv");
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
                    LR_TABLE[state_num][STATE] = state_num;
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
                    cout<<"STATE: "<<state_num<<" / ACTION: "<<enum_to_str[pos]<<" / ACTION_NUM: "<<action_num<<endl;
                    i = j+1;
                }
                else if(line[i]==','){
                    pos++;
                    i++;
                }
                else{  // line[i] is a number for a GOTO TABLE
                    int goto_num;
                    for(j=i+1; j<line_length; j++){
                        if(j==','){
                            j--;
                            break;
                        }
                    }
                    goto_num = stoi(line.substr(i, j-i+1));
                    LR_TABLE[state_num][pos] = goto_num;
                    cout<<"STATE: "<<state_num<<" / GOTO: "<<enum_to_str[pos]<<" / GOTO_NUM: "<<goto_num<<endl;
                    i=j+1;
                }
            }
        }
    }else{
        cout<<"FAILED TO OPEN CSV FILE."<<endl;
        exit(0);
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////
    
	cout << "parser INITIALIZATION" << endl;
	sstack.push({ 0,"$" });
	cout << "STACK INITIALIZATION" << endl;
	ifstream file("Transition Table.csv");

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
			//cout << idx << "사이즈" << endl;
			cout << idx << "th: [" << ActionToken[idx] << "], [" << str[idx] << "]" << endl;
			//cout << stateArr[i].getActionTable().size()<<"사이즈는 " << endl;
			stateArr[i].getActionTable().insert(pair<string, actNum>(ActionToken[idx], str[idx]));
			idx++;
		}
		cout << "Put into goto" << endl;
		while (idx < ActionToken.size() + GotoToken.size()) {
			int val = idx - (int)ActionToken.size();
			cout << "idx = " << idx << ", val = " << val << endl;
			cout << idx << "th : [" << GotoToken[val] << "], [" << str[idx] << "]" << endl;
			//cout << stateArr[i].getActionTable().size()<<"사이즈는 " << endl;
			stateArr[i].getGotoTable().insert(pair<string, int>(GotoToken[val], atoi(str[idx].c_str()))); //atoi 수정 필요
			idx++;
		}
		cout << "TURN END" << endl;
	}
	cout << "BASIC END" << endl;
	file.close();
}

void Parser::parse(ifstream& scanFile, ofstream& codeFile) {

	while (!scanFile.eof()) {
		string line;
		getline(scanFile, line);
		str += line;
	}
	str += "$";
	cout << str << endl; //str 만듬

	while (true) {
		Tuple tuple = sstack.top();
		int stateNo = tuple.stateNum;
		string s_r = tuple.str;
		break;
	}
	//shift, reduct 처리 해야함
	//현재 state들은 stateArr에서 관장함. state가 변할때마다 State* state가 가리키는 방향을 바꾸면 됨.
	//shift reduce 구현 어떻게? -> stack 사용. (처음에 {0,$} 담겨있음. 이는 Tuple이라는 구조체)
	//shift, reduce 구현......
	/*while (어떤 조건) {
		if(어떤 조건)
			state = shift(); //shift의 반환형을 State*로 바꾸면?
		else if(어떤 조건)
			reduce();
	}*/
}

void Parser::shift() {
	string s = "";
	int i = 0;
	while (str[0] != ' ') {
		str = str.substr(1);
	}
	for (i = 0; str[i] != ' '; i++) {
		s += str[i];
	}
	str = str.substr(i+1);
	sstack.push({});
}

void Parser::reduce() {

}

bool Parser::isFinalState() {
	return true; //일단 나중에 수정해야
}
