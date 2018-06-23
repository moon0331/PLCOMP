#include <iostream>
#include <string>
#include <fstream>

#include "scanner.h"

using namespace std;


void Scanner::scan(ifstream& input, ofstream& output) {
	string line;
	cout << line << endl;
	for (int i = 1; !input.eof(); i++) {
		getline(input, line);
		//cout << "[" << i << "]" << line << " || ±ÛÀÚ¼ö : " << line.length() << endl;
		string aaa = changeStringToToken(line);
		cout << aaa << endl;
		output << aaa << endl;
		//cout << line << "asdf" << endl;
	}
}

string Scanner::changeStringToToken(string line) {
	return string(line);
}