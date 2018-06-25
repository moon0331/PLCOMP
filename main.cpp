#include <iostream>
#include <fstream>
#include <string>
#include <regex>

#include "parser.h"
#include "scanner.h"
#include "symboltable.h"

using namespace std;

SymbolTable symbolTable; //�ɺ����̺��� �������� ���� (�ٸ� �ҽ� ���Ͽ��� ���� extern SymbolTable symbolTable; ��������� ��.

int main() {
	/*string filename;
	cin >> filename; //�����̸� �Է�
	ofstream source;
	source.open(filename); //�Է��� ���� ����
	source << filename; //����
	cout << filename;*/

	const string compilerName = "compiler2014 ";
	string filename;
	cin >> filename;
	if (compilerName+filename != "compiler2014 test.txt") { //�ϴ� test�� �����ϵ��� ó����!
        cout << "ERROR: wrong file name" << endl;
		return 0;
	}
    cout << compilerName + filename << "INPUT FILE NAME" << endl;
	ifstream inputFile_fin(filename);
	ofstream scanner_file(filename+".scanner"); //��ĳ�� ��ģ ��� .scanner
	ofstream symbol_fin(filename + ".symbol");	//���� symbol table .symbol

	Scanner scanner;	//scanner ��ü
	
	/*
	string line;
	file_fin >> line; // ����->�ܼ�
	scanner_fout << line << endl; // �ܼ�->����*/

	scanner.scan(inputFile_fin, scanner_file);	//scanner �۵�
	scanner_file.close();

	cout << "-=------------------------------" << endl;
	cout << symbolTable;
	cout << "-=------------------------------" << endl;

	ifstream scanner_file_fin(filename + ".scanner");	//scanner�� ����� ����ϱ� ���� ���� ����
	ifstream scanner_file_to_parse(filename + ".scanner"); //scanner�� ����� parsing�ϱ� ���� ����
	ofstream code_fin(filename + ".code");		//���� ��� ��� .code

	cout << "Converted Code is" << endl;
	string line;
	while (!scanner_file_fin.eof()) {
		getline(scanner_file_fin, line);
		cout << line << endl;
	}
	scanner_file_fin.close();

	Parser parser;		//parser ��ü

	parser.parse(scanner_file_to_parse, code_fin);

	inputFile_fin.close();
	scanner_file_to_parse.close();
	code_fin.close();
	symbol_fin.close();
	return 0;
}
