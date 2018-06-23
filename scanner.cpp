#include <iostream>
#include <string>
#include <fstream>

#include "scanner.h"

using namespace std;

string* tokenize(string line) {
	cout << "++++++++++++++++++++++++++++++++++++++++" << endl;
	int size = line.size();
	int seperate = 0;
	for (int i = 0; i < size; i++) {
		if (line[i] == ' ')
			seperate++;
	}
	cout << "���� ������ " << seperate << endl;
	string* token = new string[seperate+1];
	string temp;
	int num = 0;
	for (int i = 0; i < size; i++) {
		if (line[i] == ' ') {
			token[num++] = temp;
			cout << "token " << temp << " �߰�" << endl;
			temp = "";
		}
		else {
			temp += line[i];
		}
	}

	cout << "++++++++++++++++++++++++++++++++++++++++" << endl;

	for (int i = 0; i < num; i++) {
		cout << i << "��° token�� " << token[i] <<endl;
	}
	return token;
}

void Scanner::scan(ifstream& input, ofstream& output) {
	string line;
	cout << line << endl;

	string* aaa;

	for (int i = 1; !input.eof(); i++) {
		char c;
		getline(input, line);
		//cout << "[" << i << "]" << line << " || ���ڼ� : " << line.length() << endl;
		aaa = changeStringToToken(line);

		cout << line << endl;
		output << line << endl;
		//cout << line << "asdf" << endl;
	}

	delete aaa;
}

string* Scanner::changeStringToToken(string line) {
	string* str = tokenize(line);
	return str;
	//return string(line);
}