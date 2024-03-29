#include <iostream>
#include <string>
#include <fstream>
#include <stack>

#include "state.h"

#define NUM_OF_COL    38
#define NUM_OF_STATES 184
using namespace std;

typedef struct tuple {
	int stateNum;
	string str;
    vector<struct tuple*> children;
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
	vector<Tuple*> sstack;
public:
	Parser(); //input tape
	void parse(vector<string>& inputTape);
	void shift(int nextDestination, string handle);
	void reduce(int nextDestination);
    bool isFinalState();
    
    void create_grammar();
    void create_LR_TABLE();
};
