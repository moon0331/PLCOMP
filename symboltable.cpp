#include <iostream>
#include "symboltable.h"

using namespace std;

// Methods of SymbolTable
SymbolTable::SymbolTable() {
	numOfSymbols = 0;
}
int SymbolTable::getNum() {
	return numOfSymbols;
}
void SymbolTable::push(Information info) {
	table.push_back(info);
	numOfSymbols++;
}
bool SymbolTable::findName(string name) {
	cout << "finding " << name << ".....";
	for (int i = 0; i < numOfSymbols; i++) {
		if (table[i].getname() == name) {
			cout << "exist!";
			return true;
		}
	}
	cout << "not exist!";
	return false;
}
ostream& operator<<(ostream& os, const Information& info) {
	os << "( name = " << info.name << " | " << info.type << " | " << info.ID << ")";
	return os;
}

// Method of Information
Information::Information(string _name, string _type, string _ID, string _nontmn)
	: name(_name), type(_type), ID(_ID), nonterminal(_nontmn)
{
	size = (type == "INT") ? 4 : 1;
}

string Information::getname() const {
	return name;
}
string Information::getType() {
	return type;
}
string Information::getID() {
	return ID;
}
int Information::getSize() {
	return size;
}
string Information::getNonterminal() {
	return nonterminal;
}
void Information::addType(string _type) {
	type += _type;
}

ostream& operator<< (ostream& os, const SymbolTable& s) {
	for (int i = 0; i < s.numOfSymbols; i++) {
		os << "[" << i << "]" << s.table[i] << endl;
	}
	return os;
}
