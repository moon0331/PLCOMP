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

const vector<string> reserved = { "IF", "THEN", "ELSE", "WHILE", "RETURN" };
const vector<string> vtype = { "INT", "CHAR" };
const vector<string> terminals = { "(",		")",	";",	",",	"{",	"}",	"=",	">",	"<",	"+",	"*"};

//    "(",        ")",        ";",
//    ",",        "{",        "}",
//    "=",
//    ">",        "<",        "+",        "*",
//    "num",        "$"

extern SymbolTable symbolTable; //using extern to use symbolTable which is located in main.cpp

bool index(const Information& a, const Information& b) {
	if (a.getIdx() < b.getIdx())
		return true;
	else return false;
}

bool num_len(const string& a, const string& b) { //sorting string
	if (a.length() > b.length()) return true;
	else if (a.length() == b.length()) {
		return a < b;
	}
	else return false;
}

//sorting (1. length of symbol name, 2. alphabetical order)
bool var_len(const Information& a, const Information& b) {
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
//    cout << "isMultiToken?" << endl;
	int size = (int)terminals.size();
	for (int i = 0; i < size; i++) {
		const string& s = terminals[i];
		if (token.find(s) != string::npos && token.size()!=s.size()) {
//            cout << "not a single token as " << s << " exists in " << token << endl;
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
//            cout << "seperated " << tok << endl;
			/*if(tok=="INT" || tok=="CHAR")
				real_tokens.push_back(tok);
			else if (regex_match(tok, regex("([0-9])*"))) {
				real_tokens.push_back("num");
			}
			else {
				real_tokens.push_back("word");
			}*/
			real_tokens.push_back(tok);
		}
		else {
			tok += token[idx];
//            cout << "seperated (one character) " << tok << endl;
			real_tokens.push_back(tok);
			idx++;
		}
	}
}

void functionScan(ifstream& input, vector<string>& inputTape, vector<string>& code, int i, const regex regWord) {

	vector<Information>& symTable = symbolTable.table;
    
    string line;
    getline(input, line);
    if(line.find("BEGIN") != 0){
        cout<<"ERROR: No Function Starts with BEGIN"<<endl;
        exit(0);
    }

	//tokenize
	while (true) {
		string line;
		getline(input, line);
        if(line=="")
            continue;
        
        if(line.find("END")==0){
            return;
        }
        
		code.push_back(line);
        
		char newLine[1024]; //temp
		strcpy(newLine, line.c_str());
		char* tokken = strtok(newLine, " ");
		bool isOpen = false;
		int startOfSymbolTable = (int)symTable.size();
		while (tokken) {
			char token[129];
			strcpy(token, tokken);
			vector<string> real_tokens;
			if (isMultiToken(string(token), real_tokens)) {
				addRealToken(string(token), real_tokens);
			}
			else {
				real_tokens.push_back(string(token));
			}
			for (int i = 0; i < (int)real_tokens.size(); i++) {
				string myWord = real_tokens[i];
				if (!strcmp(myWord.c_str(), "(")) isOpen = true;
				if (!strcmp(myWord.c_str(), ")")) isOpen = false;
				if (regex_match(myWord, regWord) && find(inputTape.begin(), inputTape.end(), myWord) == inputTape.end() && !isReservedWord(myWord)) {
					string type = "";
					if (isOpen) {
						type = "function parameter";
					}
					else if (inputTape.size() >= 1 && (inputTape[inputTape.size() - 1] == "INT" || inputTape[inputTape.size() - 1] == "CHAR")) {
						type = inputTape[inputTape.size() - 1];
					}

					if (inputTape.size()>0 && inputTape[inputTape.size() - 1] == "(") {
						//symTable[startOfSymbolTable - 2].addType("function name");
						//find correspond name
						//and update data
						string s = inputTape[inputTape.size() - 2];
//                        cout << s << " is function name" << endl;
						if (symbolTable.findName(s)) {
							symbolTable.getInfo(s)->addType(", function name");
						}
//                        cout << "it's is function name";
					}
//                    cout << endl << myWord << "--------------------------------------------push" << endl;
					symbolTable.push(Information(symbolTable.getNum(), myWord, type, "NAME", "WORD"));
					startOfSymbolTable++;
				}
				inputTape.push_back(myWord);
				tokken = strtok(NULL, " ");
			}
		}

	}
}

vector<string> Scanner::scan(ifstream& input, ofstream& output) {
	vector<Information>& symTable = symbolTable.table;
	vector<string> inputTape;	//return value
	vector<string> code;
	string line;

	const regex regNum("([0-9])*");
	const regex regWord("([A-z])*"); //가져가야

    functionScan(input, inputTape, code, 1, regWord);

	// change symbols into word
	sort(symTable.begin(), symTable.end(), var_len);

    cout << "\n=========NOW SYMBOLS=========" << endl;
    cout << "number of symbols : " << symTable.size() << endl;

	for (int i = 0; i < (int)symTable.size(); i++) {
		cout << "[" << symTable[i].getname() << "]" << endl;
	}

	// change token : symbol to word
	for (int i = 0; i < (int)symTable.size(); i++) {
		string var = symTable[i].getname();
		for (int lineNum = 0; lineNum < (int)code.size(); lineNum++) {
			string& myLine = code[lineNum];
			auto pos = myLine.find(var, 0); //where var is located
			while (pos != string::npos) {
				if (var.length() >= 5) {
					myLine.replace(pos, var.length(), "word");
					pos += 4;
				}
				else { //if length is 1
					bool changeable = false;

					const string WORD("word"), NUM("num");

					int wordIdx = WORD.find(var, 0);
					int numIdx = NUM.find(var, 0);

					if (wordIdx == string::npos && numIdx == string::npos) changeable = true;
					else if (wordIdx != string::npos) {
						for (int x = 0; x < WORD.length(); x++) { // for w, o, r, d
							int y = pos - wordIdx + x;
							//cout << WORD[x] << " AND " << myLine[y] << endl;
							if (WORD[x] != myLine[y]) {
								changeable = true;
//                                break;
							}
						}
					}
					else if (numIdx != string::npos) {
						for (int x = 0; x < NUM.length(); x++) { // for n,u,m
							int y = pos - wordIdx + x;
							if (NUM[x] != myLine[y]) {
								changeable = true;
//                                break;
							}
						}
					}

					if (changeable) {
						//cout << "CHANGE " << var << "into " << "word" << endl;
						myLine.replace(pos, var.length(), "word");
						pos += 4;
					}
					else {
						//cout << "NOT CHANGE" << endl;
						pos++;
					}
				}
				//cout << "next find" << endl;
				pos = myLine.find(var, pos);
				//cout << "next pos is " << endl;
			}
		}
	}

	// change token : number to num
	vector<string> numValue;
	for (int i = 0; i < (int)inputTape.size(); i++) {
		if (regex_match(inputTape[i], regNum)) {
			numValue.push_back(inputTape[i]);
		}
	}

	sort(numValue.begin(), numValue.end(), num_len);

	for (int i = 0; i < (int)numValue.size(); i++) {
		for (int c = 0; c < (int)code.size(); c++) {
			string& num = numValue[i];
			string& myLine = code[c];
			if (regex_match(num, regNum)) {
				auto pos = myLine.find(num, 0);
				while (pos != string::npos) {
					myLine.replace(pos, num.length(), "num");
//                    cout << "line " << c << " changed : " << myLine << endl;
					pos += 3;
					pos = myLine.find(num, pos);
				}
			}
		}
	}

	//symbol to word
	for (int i = 0; i < (int)symbolTable.table.size(); i++) {
		string sym = symbolTable.table[i].getname();
		for (int j = 0; j < (int)inputTape.size(); j++) {
			string inp = inputTape[j];
			if (sym == inp) {
				inputTape[j] = "word";
			}
		}
	}

	for (int i = 0; i < (int)inputTape.size(); i++) {
		if (regex_match(inputTape[i], regNum)) {
			inputTape[i] = "num";
		}
	}

	for (int i = 0; i < (int)code.size(); i++) {
		output << code[i] << endl;
	}
    sort(symTable.begin(), symTable.end(), var_len);

	return inputTape;
}
