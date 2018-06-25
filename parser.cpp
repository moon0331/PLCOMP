#include <iostream>
#include <string>
#include <fstream>

#include "parser.h"

#define NUM_OF_STATES 141


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

extern vector<State> stateArr;	//원래 state.cpp 거

vector<string> tok(string line) {
	vector<string> v;
	int len = line.length();
	string s = "";
	int count = 0;
	for (int i = 0; i <= len; i++) {
		if (line[i] == ',' || line[i]=='\0') { //콤마 나오면
			v.push_back(s);
			s = "";
			count++;
		}
		else {
			s += line[i];
		}
	}
	cout << count<<"번"<<endl;
	v.erase(v.begin());
	int size = v.size(); //34
	int newSize = ActionToken.size() + GotoToken.size(); //34
	cout << size << newSize << endl;
	for (int i = 0; i < size; i++) {
		cout << "[" << i << "] " << v[i] << " | ";
	}
	return v;
}

Parser::Parser() {
	cout << "parser 초기화" << endl;
	stack.push({ 0,"$" });
	cout << "스택 초기화" << endl;
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
			cout << idx << "번째 : [" << ActionToken[idx] << "], [" << str[idx] << "]" << endl;
			//cout << stateArr[i].getActionTable().size()<<"사이즈는 " << endl;
			stateArr[i].getActionTable().insert(pair<string, actNum>(ActionToken[idx], str[idx]));
			idx++;
		}
		cout << "이젠 goto에 넣자" << endl;
		while (idx < ActionToken.size() + GotoToken.size()) {
			int val = idx - ActionToken.size();
			cout << "idx = " << idx << ", val = " << val << endl;
			cout << idx << "번째 : [" << GotoToken[val] << "], [" << str[idx] << "]" << endl;
			//cout << stateArr[i].getActionTable().size()<<"사이즈는 " << endl;
			stateArr[i].getGotoTable().insert(pair<string, int>(GotoToken[val], atoi(str[idx].c_str()))); //atoi 수정 필요
			idx++;
		}
		cout << "한 턴 끝" << endl;
	}
	cout << "기본 끝" << endl;
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
		Tuple tuple = stack.top();
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
	stack.push({});
}

void Parser::reduce() {

}

bool Parser::isFinalState() {
	return true; //일단 나중에 수정해야
}