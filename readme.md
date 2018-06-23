2018-1 프로그래밍언어+컴파일러 프로젝트

# 소스 파일 설명
1. main.cpp는 메인 실행 과정
2. parser.cpp는 class Parser의 메서드와 필요한 함수 정의
3. scanner.cpp는 class Scanner의 메서드와 필요한 함수 정의
4. symboltable.cpp는 class SymbolTable의 메서드와 필요한 함수 정의
5. instruction.cpp는 class Instruction의 메서드와 필요한 함수 정의

# 헤더 파일 설명
1. parser.h는 class Parser의 선언
2. scanner.h는 class Scanner의 선언
3. symboltable.h는 class SymbolTable과 이에 들어가는 class Information의 선언
4. instruction.h는 class Instruction의 선언

# 현재 상황
현재 입력하는 파일의 이름은 test로 해야만 작동되도록 설정됨.
test 파일에 코드를 입력하면, scanner가 이를 인식하여 test.scanner 파일을 생성함.
scanner가 함수명 및 변수명을 $(숫자) 꼴로 바꾸었으므로, parser는 test.scanner를 읽어와서 작동해야 함.
