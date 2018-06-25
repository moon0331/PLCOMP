#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include <string>
#include <algorithm>
#include <vector>

#include "scanner.h"
#include "symboltable.h"

using namespace std;

const vector<string> reserved = { "IF", "THEN", "ELSE", "WHILE", "RETURN" };	//예약어 전역 설정
const vector<string> vtype = { "INT", "CHAR" };									//예약어 전역 설정

extern SymbolTable symbolTable; //심볼테이블 전역으로 설정한 것을 여기서도 쓰겠다는 의미로 extern 사용

bool var_len(const Information& a, const Information& b) { //sorting의 기준 (1. symbol의 길이 내림차순, 2. 알파벳순 오름차순)
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
        
        int line_length = (int)line.length();
        int num_semi_colon=0;
        for(int j=0; j<line_length; j++){
            if(line[j] == ';'){
                num_semi_colon++;
            }
        }
        
        while(1){
            pos = line.find(var, pos);
            if(pos >= line_length+1){
                break;
            }
            else{
                line.replace(pos, var_len, id);
                pos += id_len;
            }
        }
//        while ((pos=line.find(var,pos)) != string::npos) {
//            line.replace(pos, var_len, id);
//            pos += id_len;
//        }
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

vector<string> tokenize(string line) {
    int line_length = (int)line.length();
    vector<string> token;
    
    // EMPTY LINE
    if(line_length==0)
        return token;
    
    // LEADING SPACES REMOVAL
    int pos=0;
    for(pos=0; pos<line_length; pos++){
        if(line[pos]!=' ')
            break;
    }
    // pos: Index after leading spaces
    
	cout << "++++++++++++++++++++++++++++++++++++++++" << endl;
	//const regex reg("([0-9|A-z])*");
	const regex reg("([A-z])*");
//    int size = (int)line.size();
    int seperate = 0;
    for (int i = pos; i < line_length; i++) {
        if (line[i] == ' ')
            seperate++;
    }
    cout << "NUM of Empty String: " << seperate << endl;
    
//    string* token = new string[seperate+1];
    string temp;
	string type;
//    char parenthesis;
	int number=pos;
    
    // LINE SPLIT
    // e.g. line = "  printf ( a, b ) int var;"
    for(int i=pos; i<line_length; i++){
        if(line[i] == '(' || line[i] =='{' || line[i] ==')' || line[i] =='}'){
            string str(1, line[i]);
            token.push_back(str);
        }
        else if(line.substr(i, 4) == "CHAR"){  // UPPERCASE
            token.push_back("CHAR");
        }
        else if(line.substr(i, 3) == "INT"){
            token.push_back("INT");
        }
        else if(line[i]==' '){
            continue;
        }
        else{
            int j;
            for(j=i+1; j<line_length; j++){
                if(line[j]==' ')
                    break;
            }
            // i ~ j-1
            string sub_str = line.substr(i, j-i);
            token.push_back(sub_str);
            i = j;
        }
    }
    
//    for (int i = pos ; i < line_length; i++) {
//        if (line[i] == ' ') {
//            cout << "token [" << number++ << "] : " << temp;
//
//            if (temp == "(" || temp=="{") {
//                parenthesis = temp;
//            }
//            else if (temp == ")" || temp == "}") {
//                parenthesis = "";
//            }
//
//            if (temp == "INT" || temp == "CHAR") {
//                type = temp;
//            }
//            else if (type == "" && parenthesis=="(") {
//                type = "function parameter";
//            }
//            else if (regex_match(temp, reg) && !isReservedWord(temp)) {
//                cout << "\t(symbol) ";
//                if (!symbolTable.findName(temp)) {
//                    string name = "$" + to_string(symbolTable.getNum());
//                    symbolTable.push(Information(temp, type, name, "WORD") );
//                }
//                type = "";
//            }
//            temp = "";
//            cout << endl;
//        }
//        else {
//            temp += line[i];
//        }
//    }

	cout << "++++++++++++++++++++++++++++++++++++++++" << endl;

	for (int i = 0; i < number; i++) {
        cout << i << "th Token: " << token[i] <<endl;
	}
	return token;
}

void Scanner::scan(ifstream& input, ofstream& output) {
	string line;
	cout << line << endl;

	vector<string> aaa;

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

vector<string> Scanner::changeStringToToken(string line) {
	vector<string> str = tokenize(line);
	return str;
	//return string(line);
}
