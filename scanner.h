#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

class Scanner {
public:
	void scan(ifstream& input, ofstream& output);
	//string changeStringToToken(string line);
	vector<string> changeStringToToken(string line);
};
