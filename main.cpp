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
	if (compilerName+filename != "compiler2014 test") { //�ϴ� test�� �����ϵ��� ó����!
		cout << "���� �̸��� ���� ����" << endl;
		return 0;
	}
	cout << compilerName + filename << "�Է��� ���� �̸�" << endl;
	ifstream inputFile_fin(filename);
	ofstream scanner_file(filename+".scanner"); //��ĳ�� ��ģ ��� .scanner
	ofstream code_fin(filename + ".code");		//���� ��� ��� .code
	ofstream symbol_fin(filename + ".symbol");	//���� symbol table .symbol

	Scanner scanner;	//scanner ��ü
	Parser parser;		//parser ��ü
	
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

	//parser.parsing(scanner_file_fin);		//�̺κ� ������ ��

	inputFile_fin.close();
	scanner_file_fin.close();
	code_fin.close();
	symbol_fin.close();
	return 0;
}