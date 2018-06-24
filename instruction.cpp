#include <iostream>

#include "instruction.h"

using namespace std;

string Instruction::getInstruction(Inst inst, string x) {
	switch (inst) {
	case CALL:
		return "CALL\t" + x;
	case JUMP : 
		return "JUMP\t" + x;
	default:
		return "틀린 명령입니다. (1항 아님!)";
	}
}

string Instruction::getInstruction(Inst inst, string x, string y) {
	switch (inst) {
	case LD:
		return "LD\t" + x + ", " + y;
	case ST:
		return "ST\t" + x + ", " + y;
	case JUMPF:
		return "JUMPF\t" + x + "\t" + y;
	case JUMPT:
		return "JUMPT\t" + x + "\t" + y;
	case MV:
		return "MV\t" + x + ", " + y;
	default:
		return "틀린 명령입니다. (2항 아님!)";
	}
}

string Instruction::getInstruction(Inst inst, string x, string y, string z) {
	switch (inst) {
	case ADD:
		return "ADD\t" + x + ", " + y + ", " + z;
	case MUL:
		return "MUL\t" + x + ", " + y + ", " + z;
	case LT:
		return "LT\t" + x + ", " + y + ", " + z;
	default:
		return "틀린 명령입니다. (3항 아님!)";
	}
}