#include <string>
#include <vector>

using namespace std;

class Information {
private:
	int idx;
	string name;
	string type;
	string ID;
	int size;
	string nonterminal;
public:

	Information(int i, string _name, string _type, string _ID, string _nontmn);
	int getIdx() const;
	string getname() const;
	string getType();
	string getID();
	int getSize();
	string getNonterminal();
	void addType(string _type);
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
	Information* getInfo(string name);

	friend ostream& operator<< (ostream& os, const SymbolTable& s);
};
