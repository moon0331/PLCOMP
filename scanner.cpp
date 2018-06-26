#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include <string>
#include <algorithm>
#include <vector>
#include <cstring>

#include "scanner.h"
#include "symboltable.h"

using namespace std;

const vector<string> reserved = { "IF", "THEN", "ELSE", "WHILE", "RETURN" };	//예약어 전역 설정
const vector<string> vtype = { "INT", "CHAR" };									//예약어 전역 설정

extern SymbolTable symbolTable; //using extern to use symbolTable which is located in main.cpp

bool var_len(const Information& a, const Information& b) { //sorting (1. length of symbol name, 2. alphabetical order)
	int a_len = (int)a.getname().length(), b_len = (int)b.getname().length();
	if (a_len > b_len) {
		return true;
	}
	else if (a_len == b_len) {
		return a.getname() < b.getname();
	}
	else
		return false;
}

bool isReservedWord(string token) { //is reserved word?
	if (find(reserved.begin(), reserved.end(), token) != reserved.end()) {
		return true;
	}
	else if (find(vtype.begin(), vtype.end(), token) != vtype.end()) {
		return true;
	}
	else return false;
}

vector<string> Scanner::scan(ifstream& input, ofstream& output) {
	vector<Information>& symTable = symbolTable.table;
	vector<string> v;
	vector<string> code;
	string line;

	const regex regNum("([0-9])*");
	const regex regWord("([A-z])*");

	cout << line << endl;

	for (int i = 1; !input.eof(); i++) {
		getline(input, line);
		code.push_back(line);
		cout << "[" << i << "]" << line << " || NUM of Characters : " << line.length() << endl;
		char newLine[1024]; //temp
		strcpy(newLine, line.c_str());
		char* token = strtok(newLine, " ");
		bool isOpen = false;

		while (token) {
			cout << token << " found";
			string myWord(token);
			if (!strcmp(token, "(")) isOpen = true;
			if (!strcmp(token, ")")) isOpen = false;
			if (regex_match(myWord, regWord) && find(v.begin(), v.end(), myWord) == v.end() && !isReservedWord(myWord)) {
				string type = "";
				if (isOpen) {
					type = "function parameter";
				}

				/*else if (myWord=="INT" || myWord=="CHAR") {
				type = myWord;
				}*/

				else if (v.size() >= 1 && (v[v.size() - 1] == "INT" || v[v.size() - 1] == "CHAR")) {
					type = v[v.size() - 1];
				}
				else {
					type = "function name";
				}

				symTable.push_back(Information(myWord, type, "NAME", "WORD"));
				cout << "....SYMBOL";
			}
			v.push_back(myWord);
			cout << endl;
			token = strtok(NULL, " ");
		}
	}

	// change symbols into word
	sort(symTable.begin(), symTable.end(), var_len);

	cout << "=========now symbols=========" << endl;
	cout << "number of symbols : " << symTable.size() << endl;

	for (int i = 0; i < (int)symTable.size(); i++) {
		cout << "[" << symTable[i].getname() << "]" << endl;
	}

	cout << "============================" << endl;

	// change token : symbol to word
	for (int i = 0; i < (int)symTable.size(); i++) {
		for (int lineNum = 0; lineNum < (int)code.size(); lineNum++) {
			string var = symTable[i].getname();
			string& myLine = code[lineNum];
			auto pos = myLine.find(var, 0);
			while (pos != string::npos) {
				myLine.replace(pos, var.length(), "word");
				cout << "line " << lineNum << " changed : " << myLine << endl;
				pos += 4;
				pos = myLine.find(var, pos);
			}
		}
	}

	//change token : reserved word and vtype word to word
	for (int i = 0; i < (int)reserved.size(); i++) {
		for (int lineNum = 0; lineNum < (int)code.size(); lineNum++) {
			string var = reserved[i];
			cout << "check : " << var << endl;
			string& myLine = code[lineNum];
			auto pos = myLine.find(var, 0);
			while (pos != string::npos) {
				cout << "ASDF" << endl;
				myLine.replace(pos, var.length(), "word");
				cout << "line " << lineNum << " changed : " << myLine << endl;
				pos += 4;
				pos = myLine.find(var, pos);
			}
		}
	}

	for (int i = 0; i < (int)vtype.size(); i++) {
		for (int lineNum = 0; lineNum < (int)code.size(); lineNum++) {
			string var = vtype[i];
			cout << "체크 : " << var << endl;
			string& myLine = code[lineNum];
			auto pos = myLine.find(var, 0);
			while (pos != string::npos) {
				cout << "ASDF" << endl;
				myLine.replace(pos, var.length(), "word");
				cout << "line " << lineNum << " changed : " << myLine << endl;
				pos += 4;
				pos = myLine.find(var, pos);
			}
		}
	}

	// change token : number to num
	for (int i = 0; i < (int)v.size(); i++) {
		for (int c = 0; c < (int)code.size(); c++) {
			string& num = v[i];
			string& myLine = code[c];
			if (regex_match(num, regNum)) {
				auto pos = myLine.find(num, 0);
				while (pos != string::npos) {
					myLine.replace(pos, num.length(), "num");
					cout << "line " << c << " changed : " << myLine << endl;
					pos += 3;
					pos = myLine.find(num, pos);
				}
			}
		}
	}

	vector<string> inputTape;

	for (int i = 0; i < code.size(); i++) {
		cout << code[i] << endl;
		output << code[i] << endl;	//write data in output
		char linee[1024];
		strcpy(linee, code[i].c_str());
		char* tok = strtok(linee, " ");
		while (tok) {
			cout << tok << "토큰 입력" << endl;
			inputTape.push_back(string(tok));
			tok = strtok(NULL, " ");
		}
	}

	for (int i = 0; i < (int)v.size(); i++) {
		cout << v[i] << ", " << v[i].length() << endl;
	}

	for (int i = 0; i < inputTape.size(); i++) {
		cout << "[" << inputTape[i] << "] ";
	}

	return inputTape;
}
