#include <iostream>
#include "symboltable.h"

using namespace std;

/* SymbolTable에 대한 메서드 */

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
	os << "( name = " << info.name << ", " << info.type << ", " << info.ID << ")";
	return os;
}

/* Information에 대한 메서드 */


Information::Information(string _name, string _type, string _ID)
	: name(_name), type(_type), ID(_ID)
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

ostream& operator<< (ostream& os, const SymbolTable& s) {
	for (int i = 0; i < s.numOfSymbols; i++) {
		os << "[" << i << "]" << s.table[i] << endl;
	}
	return os;
}