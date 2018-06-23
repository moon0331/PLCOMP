#include <iostream>
#include <fstream>
#include <string>
#include <regex>

#include "parser.h"
#include "scanner.h"

using namespace std;

int main() {
	/*string filename;
	cin >> filename; //파일이름 입력
	ofstream source;
	source.open(filename); //입력할 파일 생성
	source << filename; //쓰기
	cout << filename;*/

	const string compilerName = "compiler2014 ";
	string filename;
	cin >> filename;
	if (compilerName+filename != "compiler2014 test") { //일단 test만 가능하도록 처리함!
		cout << "파일 이름이 맞지 않음" << endl;
		return 0;
	}
	cout << compilerName + filename << "입력한 파일 이름" << endl;
	ifstream inputFile_fin(filename);
	ofstream scanner_file(filename+".scanner"); //스캐너 거친 결과 .scanner
	ofstream code_fin(filename + ".code");		//최종 출력 결과 .code
	ofstream symbol_fin(filename + ".symbol");	//최종 symbol table .symbol

	Scanner scanner;	//scanner 객체
	Parser parser;		//parser 객체

	/*
	string line;
	file_fin >> line; // 파일->콘솔
	scanner_fout << line << endl; // 콘솔->파일*/

	scanner.scan(inputFile_fin, scanner_file);	//scanner 작동
	scanner_file.close();

	ifstream scanner_file_fin(filename + ".scanner");	//scanner의 결과를 사용하기 위해 새로 만듬

	//parser.parsing();		//이부분 만들어야 함

	inputFile_fin.close();
	scanner_file_fin.close();
	code_fin.close();
	symbol_fin.close();
	return 0;
}