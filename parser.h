#include <iostream>
#include <string>
#include <fstream>
#include <stack>

#include "state.h"

using namespace std;

const string transition[] = {
	"", 
};

typedef struct tuple {
	int stateNum;
	string str;
}Tuple;

class Parser {
private:
	string str;			// 처리해야 할 string
	stack<Tuple> sstack;
	State* state;		// 현재 state 가리킴
public:
	Parser();
	void parse(ifstream& scanFile, ofstream& code);
	void shift();
	void reduce();
	bool isFinalState();
};
