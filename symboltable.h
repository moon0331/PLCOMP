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

	Information(string _name, string _type, string _ID);
	string getname() const;
	string getType();
	string getID();
	int getSize();
	friend ostream& operator<<(ostream& os, const Information& info);
};

class SymbolTable {
private:
	int numOfSymbols;
public:
	vector<Information> table;

	SymbolTable();
	int getNum();
	void push(Information info);
	bool findName(string name);

	friend ostream& operator<< (ostream& os, const SymbolTable& s);
};