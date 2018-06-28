//#include <iostream>
#include <fstream>
#include <string>
#include <regex>

#include "parser.h"
#include "scanner.h"
#include "symboltable.h"

using namespace std;

SymbolTable symbolTable; //global variable. (to use this variable in other cpp file, extern SymbolTable symbolTable; 

int main(int argc, const char *argv[]) {
	const string compilerName = "compiler2018 ";
    string filename;
    cout<<"FILE NAME: ";
    cin>>filename;
    
	ifstream inputFile_fin;
	inputFile_fin.open(filename);

	ofstream scanner_file; // After scanner
	scanner_file.open(filename + ".scanner");

	ofstream symbol_fin;	// Final symbol table
	symbol_fin.open(filename + ".symbol");

	if (!inputFile_fin.is_open()) {
		cout << "INPUT FILE IS NOT OPEN" << endl;
		return 0;
	}
	if (!scanner_file.is_open()) {
		cout << "SCANNER FILE IS NOT OPEN" << endl;
		return 0;
	}
	if (!symbol_fin.is_open()) {
		cout << "SYMBOL TABLE IS NOT OPEN" << endl;
		return 0;
	}

	Scanner scanner;	//scanner 객체
						/*
						string line;
						file_fin >> line; // file->console
						scanner_fout << line << endl; // console->file*/
    
    //scanner starts to scan
	vector<string> inputTape=scanner.scan(inputFile_fin, scanner_file);
	scanner_file.close();

	cout << "\n<SYMBOL>" << endl;
	vector<Information>& vv = symbolTable.table;

	for (int i = 0; i < (int)vv.size(); i++)
		cout << vv[i] << endl;
    //ifstream for print result of scanner
	ifstream scanner_file_fin(filename + ".scanner");
	ifstream scanner_file_to_parse(filename + ".scanner"); //ifstream for parsing
	ofstream code_fin(filename + ".code");		//result .code

    cout << "\n<Converted Code>" << endl;
    string line;
    while (!scanner_file_fin.eof()) {
        getline(scanner_file_fin, line);
        cout << line << endl;
    }
	
	
	ofstream sym(filename + ".symbol");  //symbol table file

	for (int i = 0; i < (int)symbolTable.table.size(); i++) {
		sym << symbolTable.table[i] << endl;
	}

	sym.close(); //write finished

	scanner_file_fin.close();

    Parser parser;        //parser object
    parser.parse(inputTape);
    
    
    

	inputFile_fin.close();
	scanner_file_to_parse.close();
	code_fin.close();
	symbol_fin.close();

	return 0;

}
