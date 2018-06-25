#include <iostream>
#include <fstream>
#include <string>
#include <regex>

#include "parser.h"
#include "scanner.h"
#include "symboltable.h"

using namespace std;

SymbolTable symbolTable; //�ɺ����̺��� �������� ���� (�ٸ� �ҽ� ���Ͽ��� ���� extern SymbolTable symbolTable; ��������� ��.

int main(int argc, const char *argv[]) {
	/*string filename;
	cin >> filename; //�����̸� �Է�
	ofstream source;
	source.open(filename); //�Է��� ���� ����
	source << filename; //����
	cout << filename;*/

	const string compilerName = "compiler2014 ";
	string filename = "test";
//    cout<<"FILE NAME: ";
//    cin >> filename;
//    if (compilerName+filename != "compiler2014 test") { //�ϴ� test�� �����ϵ��� ó����!
//        cout << "ERROR: wrong file name" << endl;
//        return 0;
//    }
    cout << compilerName + filename << ": INPUT FILE NAME" << endl;
	ifstream inputFile_fin;
//    inputFile_fin.open(filename);
    inputFile_fin.open("/Users/BaeHaneul/Developer/PLCOMP/PLCOMP/test");  // XCODE needs the full path.
    
	ofstream scanner_file; //��ĳ�� ��ģ ��� .scanner
    scanner_file.open(filename+".scanner");
    
	ofstream symbol_fin;	//���� symbol table .symbol
    symbol_fin.open(filename + ".symbol");
    
    if(!inputFile_fin.is_open()){
        cout<<"INPUT FILE IS NOT OPEN"<<endl;
        return 0;
    }
    if(!scanner_file.is_open()){
        cout<<"SCANNER FILE IS NOT OPEN"<<endl;
        return 0;
    }
    if(!symbol_fin.is_open()){
        cout<<"SYMBOL TABLE IS NOT OPEN"<<endl;
        return 0;
    }

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

//    Parser parser;        //parser ��ü
//    parser.parse(scanner_file_to_parse, code_fin);

	inputFile_fin.close();
	scanner_file_to_parse.close();
	code_fin.close();
	symbol_fin.close();
	return 0;
}
