#include <iostream>
#include <map>

#include "symboltable.h"
#include "state.h"

using namespace std;

extern SymbolTable symbolTable;

vector<State> stateArr;

State::State() {

}

State::State(int num) : stateNum(num) {
	cout << "state »ý¼ºÀÚ " << num << endl;
}

void State::setActionTable(string s, actNum a) {
	Action.insert(pair<string, actNum>(s, a));
}

void State::setGotoTable(string s, int v) {
	Goto.insert(pair<string, int>(s, v));
}

map<string, actNum>& State::getActionTable() {
	return Action;
}

map<string, int>& State::getGotoTable() {
	return Goto;
}

actNum State::getAction(string s) {
	return Action.find(s)->second;
}

int State::getGoto(string s) {
	return Goto.find(s)->second;
}