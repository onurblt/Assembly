#ifndef LANG_HPP_INCLUDED
#define LANG_HPP_INCLUDED

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <bitset>
#include <math.h>
#include <initializer_list>

//___________________________________________

enum RegisterType
{
        AC,//! Accumulator
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

    AND,//! 0XXX OR 8XXX
    ADD,//! 1XXX OR 9XXX
    LDA,//! 2XXX OR AXXX
    STA,//! 3XXX OR BXXX
    BUN,//! 4XXX OR CXXX
    BSA,//! 5XXX OR DXXX
    ISZ,//! 6XXX OR EXXX
    CLA=0X7800,
    CLE=0X7400,
    CMA=0X7200,
    CME=0X7100,
    CIR=0X7080,
    CIL=0X7040,
    INC=0X7020,
    SPA=0X7010,
    SNA=0X7008,
    SZA=0X7004,
    SZE=0X7002,
    HLT=0x7001,
    INP=0XF800,
    OUT=0XF400,
    SKI=0XF200,
    SKO=0XF100,
    ION=0XF080,
    IOF=0XF040

};

//___________________________________________

enum FlagType
{
    I=0,
    S,
    E,
    R,
    IEN,
    FGI,
    FGO,
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


    void PrintRAM();
    void PrintRegisters();
    void PrintFlags();
    void Execute(const std::vector<Word> &program);

protected:
    InstructionType Parse();
    void ParseAddress(int *data,int direct,int relative);
     void Add();
     void And();
     void Lda();
    void Sta();
    void Bun();
    void Bsa();
    void Isz();
	void Cla();
	void Cle();
	void Cma();
	void Cme();
	void Cir();
	void Cil();
	void Inc();
	void Spa();
	void Sna();
	void Sza();
	void Sze();
	void Hlt();
	void Inp();
	void Out();
	void Ski();
	void Sko();
	void Ion();
	void Iof();

    Flag flags[FLAG_MAX];

    Word registers[REGISTER_MAX];

    std::vector<Word> Mem;

};

//___________________________________________

#endif // LANG_HPP_INCLUDED
