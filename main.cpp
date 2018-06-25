#include <iostream>
#include <fstream>
#include <string>
#include <regex>

#include "parser.h"
#include "scanner.h"
#include "symboltable.h"

using namespace std;

SymbolTable symbolTable; //global variable. (to use this variable in other cpp file, extern SymbolTable symbolTable; 

int main(int argc, const char *argv[]) {
	/*string filename;
	cin >> filename; //파일이름 입력
	ofstream source;
	source.open(filename); //입력할 파일 생성
	source << filename; //쓰기
	cout << filename;*/

	const string compilerName = "compiler2014 ";
	string filename = "test";
//    cout<<"FILE NAME: ";
//    cin >> filename;
//    if (compilerName+filename != "compiler2014 test") { //일단 test만 가능하도록 처리함!
//        cout << "ERROR: wrong file name" << endl;
//        return 0;
//    }
    cout << compilerName + filename << ": INPUT FILE NAME" << endl;
	ifstream inputFile_fin;
//    inputFile_fin.open(filename);
    inputFile_fin.open("test");  // XCODE needs the full path. .......................... please modify path!
    
	ofstream scanner_file; //스캐너 거친 결과 .scanner
    scanner_file.open(filename+".scanner");
    
	ofstream symbol_fin;	//최종 symbol table .symbol
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

	Scanner scanner;	//scanner 객체
	
	/*
	string line;
	file_fin >> line; // 파일->콘솔
	scanner_fout << line << endl; // 콘솔->파일*/

	scanner.scan(inputFile_fin, scanner_file);	//scanner 작동
	scanner_file.close();

	cout << "-=--symbol is-------------------" << endl;
	vector<Information>& vv = symbolTable.table;
	for (int i = 0; i < vv.size(); i++)
		cout << vv[i] << endl;
	cout << "-=------------------------------" << endl;

	ifstream scanner_file_fin(filename + ".scanner");	//ifstream for print result of scanner
	ifstream scanner_file_to_parse(filename + ".scanner"); //ifstream for parsing
	ofstream code_fin(filename + ".code");		//result .code

	cout << "Converted Code is" << endl;
	string line;
	while (!scanner_file_fin.eof()) {
		getline(scanner_file_fin, line);
		cout << line << endl;
	}
	scanner_file_fin.close();

    Parser parser;        //parser 객체
    //parser.parse(scanner_file_to_parse, code_fin);

	inputFile_fin.close();
	scanner_file_to_parse.close();
	code_fin.close();
	symbol_fin.close();
	return 0;
}
