#include <iostream>
#include "symboltable.h"

using namespace std;


ostream& operator<<(ostream& os, const Information& info) {
	os << "( name = " << info.name << ", "<< info.type << ", " << info.ID << ")";
	return os;
}


ostream& operator<< (ostream& os, const SymbolTable& s) {
	for (int i = 0; i < s.numOfSymbols; i++) {
		os << "[" << i << "]" << s.table[i] << endl;
	}
	return os;
}