#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include <string>
#include <algorithm>

#include "scanner.h"
#include "symboltable.h"

using namespace std;

const vector<string> reserved = { "IF", "THEN", "ELSE", "WHILE", "RETURN" };	//����� ���� ����
const vector<string> vtype = { "INT", "CHAR" };									//����� ���� ����

extern SymbolTable symbolTable; //�ɺ����̺� �������� ������ ���� ���⼭�� ���ڴٴ� �ǹ̷� extern ���

bool var_len(const Information& a, const Information& b) { //sorting�� ���� (1. symbol�� ���� ��������, 2. ���ĺ��� ��������)
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

string changeStringToID(string l) { //symbol�� $(number) �÷� �ٲ�
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
//        cout << var << "�� " << id << "�� �ٲٱ� ���� üũ" << endl;
		// asdf�� $0�� �ٲٱ� --> word�� num���� �ٲ��
		while ((pos=line.find(var,pos)) != string::npos) {
			line.replace(pos, var_len, id);
			pos += id_len;
		}
	}
	return line;
}

bool isReservedWord(string token) { //�� token�� ������ΰ�?
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

			if (temp == "INT" || temp == "CHAR") { //�ڷ��� ����
				type = temp;
			}
			else if (type == "" && parenthesis=="(") { //��ȣ �ȿ� �ִ��� Ȯ��
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
