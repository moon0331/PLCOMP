#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include <string>
#include <algorithm>
#include <vector>
#include <cstring>
#include <cctype>

#include "scanner.h"
#include "symboltable.h"

using namespace std;

const vector<string> reserved = { "IF", "THEN", "ELSE", "WHILE", "RETURN" };	//예약어 전역 설정
const vector<string> vtype = { "INT", "CHAR" };									//예약어 전역 설정
const vector<string> terminals = { "(",		")",	";",	",",	"{",	"}",	"=",	">",	"<",	"+",	"*"};

//    "(",        ")",        ";",
//    ",",        "{",        "}",
//    "=",
//    ">",        "<",        "+",        "*",
//    "num",        "$"

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

bool isMultiToken(string token, vector<string>& real_tokens) {
	cout << "isMultiToken?" << endl;
	int size = (int)terminals.size();
	for (int i = 0; i < size; i++) {
		const string& s = terminals[i];
		cout << s << " check " << endl;
		if (token.find(s) != string::npos && token.size()!=s.size()) {
			cout << "not a single token as " << s << " exists in " << token << endl;
			return true;
		}
	}
	return false;
}

void addRealToken(string token, vector<string>& real_tokens) {
	int size = (int)token.size();
	int idx = 0;
	while (idx < size) {
		string tok;
		if (isalnum(token[idx])) {
			while (isalnum(token[idx])) {
				tok += token[idx];
				idx++;
			}
			cout << "seperated " << tok << endl;
			real_tokens.push_back(tok);
		}
		else {
			tok += token[idx];
			cout << "seperated (one character) " << tok << endl;
			real_tokens.push_back(tok);
			idx++;
		}
	}
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
		char* tokken = strtok(newLine, " ");
		bool isOpen = false;


		while (tokken) {
			char token[129];
			strcpy(token, tokken);
			cout << "[" << token << "] found......";
			vector<string> real_tokens;
			if (isMultiToken(string(token), real_tokens)) {
				cout << "not match : not alphabet or not.... need to seperate" << endl;
				addRealToken(string(token), real_tokens);
			}
			else {
				real_tokens.push_back(string(token));
			}
			for (int i = 0; i < (int)real_tokens.size(); i++) {
				string myWord = real_tokens[i];
				if (!strcmp(myWord.c_str(), "(")) isOpen = true;
				if (!strcmp(myWord.c_str(), ")")) isOpen = false;
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
				tokken = strtok(NULL, " ");
			}
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
		string var = symTable[i].getname();
		for (int lineNum = 0; lineNum < (int)code.size(); lineNum++) {
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

	cout << "what process??.........................." << endl;
	for (int i = 0; i < (int)vtype.size(); i++) {
		string var = vtype[i];
		for (int lineNum = 0; lineNum < (int)code.size(); lineNum++) {
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

	// change token : number to num
	cout << "number to NUM........................." << endl;
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
