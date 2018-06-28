#include <vector>
#include <string>
#include <map>

using namespace std;


typedef string actNum;

class State {
private:
	int stateNum;
	map<string, actNum> Action;
	map<string, int> Goto;

public:
	State();
	State(int num);
	void setActionTable(string s, actNum a);
	void setGotoTable(string s, int v);
	map<string, actNum>& getActionTable();
	map<string, int>& getGotoTable();
	actNum getAction(string s);
	int getGoto(string s);
};
