#include <iostream>

using namespace std;

typedef enum { LD, ST, ADD, MUL, LT, CALL, JUMPF, JUMPT, JUMP, MV } Inst;

class Instruction{
	string getInstruction(Inst inst, string x);
	string getInstruction(Inst inst, string x, string y);
	string getInstruction(Inst inst, string x, string y, string z);
};
