#include <iostream>
#include <string>
#include <fstream>

#include "parser.h"

#define NUM_OF_STATES 160

extern vector<State> stateArr;	//���� state.cpp ��

Parser::Parser() {
	cout << "parser �ʱ�ȭ" << endl;
	stack.push({ 0,"$" });
	cout << "���� �ʱ�ȭ" << endl;
}

void Parser::parse(ifstream& scanFile, ofstream& codeFile) {

	for (int i = 0; i < NUM_OF_STATES; i++) {
		stateArr.push_back(State(i));
		//stateArr[i] ���� �Է�
	}

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
	//ó��
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