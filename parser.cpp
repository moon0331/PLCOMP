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

extern vector<State> stateArr;	//���� state.cpp ��

vector<string> tok(string line) {
	vector<string> v;
	int len = line.length();
	string s = "";
	int count = 0;
	for (int i = 0; i <= len; i++) {
		if (line[i] == ',' || line[i]=='\0') { //�޸� ������
			v.push_back(s);
			s = "";
			count++;
		}
		else {
			s += line[i];
		}
	}
	cout << count<<"��"<<endl;
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
	cout << "parser �ʱ�ȭ" << endl;
	stack.push({ 0,"$" });
	cout << "���� �ʱ�ȭ" << endl;
	ifstream file("Compiler Grammar - Transition Table.csv");

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
			//cout << idx << "������" << endl;
			cout << idx << "��° : [" << ActionToken[idx] << "], [" << str[idx] << "]" << endl;
			//cout << stateArr[i].getActionTable().size()<<"������� " << endl;
			stateArr[i].getActionTable().insert(pair<string, actNum>(ActionToken[idx], str[idx]));
			idx++;
		}
		cout << "���� goto�� ����" << endl;
		while (idx < ActionToken.size() + GotoToken.size()) {
			int val = idx - ActionToken.size();
			cout << "idx = " << idx << ", val = " << val << endl;
			cout << idx << "��° : [" << GotoToken[val] << "], [" << str[idx] << "]" << endl;
			//cout << stateArr[i].getActionTable().size()<<"������� " << endl;
			stateArr[i].getGotoTable().insert(pair<string, int>(GotoToken[val], atoi(str[idx].c_str()))); //atoi ���� �ʿ�
			idx++;
		}
		cout << "�� �� ��" << endl;
	}
	cout << "�⺻ ��" << endl;
}

void Parser::parse(ifstream& scanFile, ofstream& codeFile) {

	while (!scanFile.eof()) {
		string line;
		getline(scanFile, line);
		str += line;
	}
	str += "$";
	cout << str << endl; //str ����

	while (true) {
		Tuple tuple = stack.top();
		int stateNo = tuple.stateNum;
		string s_r = tuple.str;
		break;
	}
	//shift, reduct ó�� �ؾ���
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
	return true; //�ϴ� ���߿� �����ؾ�
}