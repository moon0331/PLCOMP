#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include <string>
#include <algorithm>

#include "scanner.h"
#include "symboltable.h"

using namespace std;

const vector<string> reserved = { "IF", "THEN", "ELSE", "WHILE", "RETURN" };	//예약어 전역 설정
const vector<string> vtype = { "INT", "CHAR" };									//예약어 전역 설정

extern SymbolTable symbolTable; //심볼테이블 전역으로 설정한 것을 여기서도 쓰겠다는 의미로 extern 사용

bool var_len(const Information& a, const Information& b) { //sorting의 기준 (1. symbol의 길이 내림차순, 2. 알파벳순 오름차순)
	int a_len = a.getname().length(), b_len = b.getname().length();
	if (a_len > b_len) {
		return true;
	}
	else if (a_len == b_len) {
		return a.getname() < b.getname();
	}
	else
		return false;
}

string changeStringToID(string l) { //symbol을 $(number) 꼴로 바꿈
	string line = l;
	int num = symbolTable.getNum();
	stable_sort(symbolTable.table.begin(), symbolTable.table.end(), var_len);
	for (int i = 0; i < num; i++) {
		string var = symbolTable.table[i].getname();
		int var_len = (int)var.length();
		string id = symbolTable.table[i].getID();
		int id_len = (int)id.length();
		size_t pos = 0;
        cout<<"Check to change "<<var<<" to "<<id<<endl;
//        cout << var << "를 " << id << "로 바꾸기 위한 체크" << endl;
		// asdf를 $0로 바꾸기 --> word나 num으로 바꿔야
		while ((pos=line.find(var,pos)) != string::npos) {
			line.replace(pos, var_len, id);
			pos += id_len;
		}
	}
	return line;
}

bool isReservedWord(string token) { //이 token이 예약어인가?
	if (find(reserved.begin(), reserved.end(), token) != reserved.end()) {
		return true;
	}
	else if (find(vtype.begin(), vtype.end(), token) != vtype.end()) {
		return true;
	}
	else return false;
}

string* tokenize(string line) {
	cout << "++++++++++++++++++++++++++++++++++++++++" << endl;
	//const regex reg("([0-9|A-z])*");
	const regex reg("([A-z])*");
	int size = (int)line.size();
	int seperate = 0;
	for (int i = 0; i < size; i++) {
		if (line[i] == ' ')
			seperate++;
	}
    cout << "NUM of Empty String: " << seperate << endl;
	string* token = new string[seperate+1];
	string temp;
	string type;
	string parenthesis;
	int num = 0;
	for (int i = 0, number = 0; i < size; i++) {
		if (line[i] == ' ') {
			cout << "token [" << number++ << "] : " << temp;

			if (temp == "(" || temp=="{") {
				parenthesis = temp;
			}
			else if (temp == ")" || temp == "}") {
				parenthesis = "";
			}

			if (temp == "INT" || temp == "CHAR") { //자료형 정의
				type = temp;
			}
			else if (type == "" && parenthesis=="(") { //괄호 안에 있는지 확인
				type = "function parameter";
			}
			else if (regex_match(temp, reg) && !isReservedWord(temp)) {
				cout << "\t(symbol) ";
				if (!symbolTable.findName(temp)) {
					string name = "$" + to_string(symbolTable.getNum());
					symbolTable.push(Information(temp, type, name, "WORD") );
				}
				type = "";
			}
			temp = "";
			cout << endl;
		}
		else {
			temp += line[i];
		}
	}

	cout << "++++++++++++++++++++++++++++++++++++++++" << endl;

	for (int i = 0; i < num; i++) {
        cout << i << "th Token: " << token[i] <<endl;
	}
	return token;
}

void Scanner::scan(ifstream& input, ofstream& output) {
	string line;
	cout << line << endl;

	string* aaa;

	for (int i = 1; !input.eof(); i++) {
		getline(input, line);
		cout << "[" << i << "]" << line << " || NUM of Characters: " << line.length() << endl;
		aaa = changeStringToToken(line);

		string newLine=changeStringToID(line);

		cout << line << endl;
		cout << newLine << endl;
		output << newLine << endl;
		//cout << line << "asdf" << endl;
		//output << newLine << endl;
	}
}

string* Scanner::changeStringToToken(string line) {
	string* str = tokenize(line);
	return str;
	//return string(line);
}
