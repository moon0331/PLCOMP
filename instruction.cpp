#include <iostream>

#include "instruction.h"

using namespace std;

//typedef enum { LD, ST, ADD, MUL, LT, CALL, JUMPF, JUMPT, JUMP, MV } Inst;

string Instruction::getInstruction(Inst inst, string x, string y, string z) {
	switch (inst) {
	case LD:
		return "LD\t" + x + ", " + y;
	case ST:
		return "ST\t" + x + ", " + y;
	case ADD:
		return "ADD\t" + x + ", " + y + ", " + z;
	case MUL:
		return "MUL\t" + x + ", " + y + ", " + z;
	case LT:
		return "LT\t" + x + ", " + y + ", " + z;
	case CALL:
		return "CALL\t" + x;
	case JUMPF:
		return "JUMPF\t" + x + "\t" + y;
	case JUMPT:
		return "JUMPT\t" + x + "\t" + y;
	case JUMP:
		return "JUMP\t" + x;
	case MV:
		return "MV\t" + x + ", " + y;
	}
}