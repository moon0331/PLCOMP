#include <string>
#include <vector>

using namespace std;

class Information {
private:
	string name;
	string type;
	//여러 함수 적용시 함수 이름도 들어가야
	string ID;
	int size;
public:

	Information(string _name, string _type, string _ID)
		: name(_name), type(_type), ID(_ID) 
	{ 
		size = (type == "INT") ? 4 : 1;
	}

	string getname() const {
		return name;
	}
	string getType() {
		return type;
	}
	string getID() {
		return ID;
	}
	int getSize() {
		return size;
	}
	friend ostream& operator<<(ostream& os, const Information& info);
};

class SymbolTable {
private:
	int numOfSymbols;
public:
	vector<Information> table;
	SymbolTable() {
		numOfSymbols = 0;
	}
	int getNum() {
		return numOfSymbols;
	}
	void push(Information info) {
		table.push_back(info);
		numOfSymbols++;
	}
	bool findName(string name) {
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

	friend ostream& operator<< (ostream& os, const SymbolTable& s);
};