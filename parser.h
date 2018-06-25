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
	string str;			// ó���ؾ� �� string
	stack<Tuple> sstack;
	State* state;		// ���� state ����Ŵ
public:
	Parser();
	void parse(ifstream& scanFile, ofstream& code);
	void shift();
	void reduce();
	bool isFinalState();
};
