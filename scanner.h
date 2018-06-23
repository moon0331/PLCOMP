#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class Scanner {
public:
	void scan(ifstream& input, ofstream& output);
	string changeString(string line);
};