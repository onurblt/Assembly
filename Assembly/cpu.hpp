#ifndef LANG_HPP_INCLUDED
#define LANG_HPP_INCLUDED

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <bitset>
#include <math.h>
#include <initializer_list>
#include <map>
#include "lexer.hpp"

//___________________________________________

enum RegisterType
{
        AC=0,//! Accumulator
        PC,//! Program Counter
        IR,//! Instruction Register
        DR,//! Data Register
        TR,//! Temporary Register
        AR,//! Address Register
        INPR,//! Input Register
        OUTR,//! Output Register
        SC,//! Sequence Counter
        REGISTER_MAX

};

//___________________________________________

enum InstructionType
{

	INSTR_AND,//! 0XXX OR 8XXX
	INSTR_ADD,//! 1XXX OR 9XXX
	INSTR_LDA,//! 2XXX OR AXXX
	INSTR_STA,//! 3XXX OR BXXX
	INSTR_BUN,//! 4XXX OR CXXX
	INSTR_BSA,//! 5XXX OR DXXX
	INSTR_ISZ,//! 6XXX OR EXXX
	INSTR_CLA = 0X7800,
	INSTR_CLE = 0X7400,
	INSTR_CMA = 0X7200,
	INSTR_CME = 0X7100,
	INSTR_CIR = 0X7080,
	INSTR_CIL = 0X7040,
	INSTR_INC = 0X7020,
	INSTR_SPA = 0X7010,
	INSTR_SNA = 0X7008,
	INSTR_SZA = 0X7004,
	INSTR_SZE = 0X7002,
	INSTR_HLT = 0x7001,
	INSTR_INP = 0XF800,
	INSTR_OUT = 0XF400,
	INSTR_SKI = 0XF200,
	INSTR_SKO = 0XF100,
	INSTR_ION = 0XF080,
	INSTR_IOF = 0XF040

};

//___________________________________________

enum FlagType
{
    FLAG_I=0,
    FLAG_S,
    FLAG_E,
    FLAG_R,
    FLAG_IEN,
    FLAG_FGI,
    FLAG_FGO,
    FLAG_MAX


};


//___________________________________________


#define WORD_SIZE 16



class Word
{
public:
    Word();
    Word(const int &data);
    Word(const std::bitset<WORD_SIZE> &data);

   Word(const int &a,const int &b,const int &c,const int & d);
   Word(const int &op, const int &address);
    Word(const Word &other) ;
    Word& operator = (const Word &other) ;

    Word operator+(const Word& other);
    Word operator+=(const Word& other);
    Word operator&(const Word& other);
    Word operator&=(const Word& other);
	Word operator~();
    const int Get(const int &n);
    const int Get();
    std::bitset<WORD_SIZE> data;
	Word Shl();
	Word Shr();
    bool carry;
};


#define WORD_COUNT 16

typedef Word RAM[WORD_COUNT];

typedef bool Flag;

//___________________________________________

class Cpu
{
public:
    Cpu();
    virtual ~Cpu();


    void printRAM();
    void printRegisters();
    void printFlags();
	void setRAM(const std::string &script);
	int step();
	void clear();
	

protected:
    InstructionType parse();
    void parseAddress(int *data);
     void add();
     void and();
     void lda();
    void sta();
    void bun();
    void bsa();
    void isz();
	void cla();
	void cle();
	void cma();
	void cme();
	void cir();
	void cil();
	void inc();
	void spa();
	void sna();
	void sza();
	void sze();
	void hlt();
	void inp();
	void out();
	void ski();
	void sko();
	void ion();
	void iof();

public:
    Flag flags[FLAG_MAX];

    Word REG[REGISTER_MAX];

    std::vector<Word> Mem;
	std::map<std::string,int> symbolAddressTable;
	void generateSymbolAddressTable(const std::string &script);

	Error error;
};

//___________________________________________

#endif // LANG_HPP_INCLUDED
