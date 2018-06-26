#include <iostream>
#include <string>
#include <fstream>
#include <stack>

#include "state.h"

#define NUM_OF_COL    38
#define NUM_OF_STATES 177

using namespace std;

const string transition[] = {
	"", 
};

typedef struct tuple {
	int stateNum;
	string str;
}Tuple;

typedef struct _grammar{
    string left_side;
    vector<string> right_side;
}Grammar;

class Parser {
private:
    map<string, int> MAP_LR_TABLE;
    int LR_TABLE[NUM_OF_STATES][NUM_OF_COL];
    vector<Grammar> grammars;
    
	string str;			// string
	stack<Tuple> sstack;
	State* state;		// present state
public:
	Parser(); //input tape
	void parse(ifstream& scanFile, ofstream& code, vector<string>& inputTape);
	void shift();
	void reduce();
    void create_grammar();
    void create_LR_TABLE();
	bool isFinalState();
};
