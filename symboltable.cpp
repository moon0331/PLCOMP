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
	cout << "finding " << name << "..... numOfSymbols is " << numOfSymbols << endl;
	for (int i = 0; i < numOfSymbols; i++) {
		cout << table[i].getname() << " == " << name << endl;
		if (table[i].getname() == name) {
			cout << "exist!";
			return true;
		}
	}
	cout << "not exist!";
	return false;
}
Information* SymbolTable::getInfo(string name) {
	for (int i = 0; i < numOfSymbols; i++) {
		if (table[i].getname()==name) {
			return &(table[i]);
		}
	}
}
ostream& operator<<(ostream& os, const Information& info) {
	os << "( " << "[" << info.idx << "] " << "name = " << info.name << " | " << info.type << " | " << info.ID << ")";
	return os;
}


// Method of Information
Information::Information(int i, string _name, string _type, string _ID, string _nontmn)
	: idx(i), name(_name), type(_type), ID(_ID), nonterminal(_nontmn)

{
	size = (type == "INT") ? 4 : 1;
}
int Information::getIdx() const {
	return idx;
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
