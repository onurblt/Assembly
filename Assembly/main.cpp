#include <vld.h>
#include "cpu.hpp"

#include "lexer.hpp"


int main()
{
    Cpu cpu;
	/*
//add example
    std::cout<<std::endl<<std::endl<<">>>>>>>ADD Example<<<<<<<<<<"<<std::endl<<std::endl;
    std::vector<Word> program={
        Word(0x9003),//0 ADD 3
        Word(0x1005),//1 ADD 4
        Word(HLT),//2
        Word(0x0004),//3 4
        Word(0x9000),//4 7
        Word(0xEE00),//5 8
        };
    cpu.Execute(program);
	
    //cpu.PrintRegisters();
	
//and example
    std::cout<<std::endl<<std::endl<<">>>>>>>AND Example<<<<<<<<<<"<<std::endl<<std::endl;
    program={
        Word(0x0003),//0 AND 3
        Word(0x8004),//1 ADD 4
        Word(HLT),//2
        Word(0x0001),//3 1
        Word(0x0003),//4 3
        };
    cpu.Execute(program);
    //lda exapmle
    std::cout<<std::endl<<std::endl<<">>>>>>>LDA Example<<<<<<<<<<"<<std::endl<<std::endl;
    program={
        Word(0x2003),//0 LDA 3
        Word(0x8004),//1 ADD 4
        Word(HLT),//2
        Word(0x0001),//3 1
        Word(0x0003),//4 3
        };
    cpu.Execute(program);
     //sta exapmle
    std::cout<<std::endl<<std::endl<<">>>>>>>STA Example<<<<<<<<<<"<<std::endl<<std::endl;
    program={
        Word(0x2003),//0 LDA 3
        Word(0x3004),//1 STA 4
        Word(HLT),//2
        Word(0x0021),//3 0x21(33 dec)
        Word(0x0009),//4 9
        };
    cpu.Execute(program);
    cpu.PrintRAM();

      //bun example
    std::cout<<std::endl<<std::endl<<">>>>>>>BUN Example<<<<<<<<<<"<<std::endl<<std::endl;
    program={
        Word(0x3005),//0 STA 3
        Word(0x4003),//1 BUN 5
        Word(0x1006),//2 ADD 6
        Word(0x1007),//3 ADD 7
        Word(HLT),//4
        Word(0x0021),//5 0x21(33 dec)
        Word(0x0009),//6 9
        Word(0x0003),//7 3
        };
    cpu.Execute(program);


      //bsa exapmle
    std::cout<<std::endl<<std::endl<<">>>>>>>BSA Example<<<<<<<<<<"<<std::endl<<std::endl;
    program={
        Word(0x2006),//0 LDA 6
        Word(0x5002),//1 BSA 3
        Word(0x1006),//2 ADD 6
        Word(0x1007),//3 ADD 7
        Word(HLT),//4
        Word(0x0021),//5 0x21(33 dec)
        Word(0x0009),//6 9
        Word(0x0003),//7 3
        };
    cpu.Execute(program);
    cpu.PrintRAM();

      //isz exapmle
   std::cout<<std::endl<<std::endl<<">>>>>>>ISZ Example<<<<<<<<<<"<<std::endl<<std::endl;
    program={
        Word(0x2007),//0 LDA 7
        Word(0x6005),//1 ISZ 5
        Word(0x1006),//2 ADD 6
        Word(0x1007),//3 ADD 7
        Word(HLT),//4
        Word(-1),//5 0x21(33 dec)
        Word(0x0009),//6 9
        Word(0x0003),//7 3
        };
    cpu.Execute(program);
    cpu.PrintRAM();
	*/
	/*
	//cla & cma exapmle
	std::cout << std::endl << std::endl << ">>>>>>>CLA ve CMA Example<<<<<<<<<<" << std::endl << std::endl;
	std::vector<Word> program = {
		Word(0x2006),//0 LDA 7	
		Word(CMA),//1 CMA
		Word(0x3005),//2 STA 5
		Word(CLA),//3 CLA
		Word(HLT),//4
		Word(0x0003),//5 3
		Word(0x0009),//6 9
	};
	cpu.Execute(program);
	cpu.PrintRAM();
	*/
/*
	//cir & cil exapmle
	std::cout << std::endl << std::endl << ">>>>>>> CIR & CIL Example<<<<<<<<<<" << std::endl << std::endl;
	std::vector<Word> program = {
		Word(0x2006),//0 LDA 6
		Word(CIR),//1 CIR
		Word(CIL),//2 CIL
		Word(CIL),//3 CIL
		Word(CIL),//4 CIL
		Word(HLT),//5 HLT
		Word(0x0003),//6 3
		Word(0x0009),//7 9
	};
	cpu.Execute(program);

	//inpr & outr exapmle
	std::cout << std::endl << std::endl << ">>>>>>> INP & OUT Example<<<<<<<<<<" << std::endl << std::endl;
	program = {
		Word(INP),//0 INP
		Word(0x3004),//1 STA 4
		Word(OUT),//2 CIL
		Word(HLT),//3 HLT
		Word(0x0000),//4 0
	};
	cpu.Execute(program);
	cpu.PrintRAM();
	std::cout << "OK!" << std::endl;
	std::getchar();*/

Lexer lex;
lex.lexString("ADD 1\n STA 3 I");

Token token = lex.next();

while (token.type != TK_END_OF_INPUT)
{
	std::cout << token.value <<"|"<< token.type << std::endl;
	token = lex.next();
}

	std::getchar();
    return 0;
}
