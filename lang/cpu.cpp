#include "cpu.hpp"

#define REG(R) registers[R]


//___________________________________________

Word::Word()
{
    data=0;
}

//___________________________________________

Word::Word(const int &_data)
{
   data=_data;
}

//___________________________________________

Word::Word(const std::bitset<WORD_SIZE> &_data)
{
   data=_data;

}

//___________________________________________

Word::Word(const Word &other)
{
    data=other.data;


}
Word& Word::operator = (const Word &other)
{
    data=other.data;
    return *this;
}

//___________________________________________

Word::Word(const int &a,const int &b,const int &c,const int & d)
{

    int r=(a<<12)+(b<<8)+(c<<4)+d;
   //printf("Com=%d\n",r);
   data=r;
}
//___________________________________________

Word Word::operator+(const Word& other)
{
    Word temp(data);
    temp.carry = false;

    for (int i = 0; i < WORD_COUNT; i++)
    {
        bool sum = (temp.data[i] ^ other.data[i]) ^ temp.carry;
        temp.carry = (temp.data[i] && other.data[i]) || (temp.data[i] && temp.carry) || (other.data[i] && temp.carry);
        temp.data[i] = sum;
    }

    //std::cout <<"add="<<data.to_ulong()<<std::endl;

    return temp;
}

//___________________________________________

Word Word::operator+=(const Word& other)
{
    *this=*this+other;
    return *this;
}

//___________________________________________

Word Word::operator&(const Word& other)
{
    Word temp(data);
    temp.data &= other.data;

    return temp;
}

//___________________________________________

Word Word::operator&=(const Word& other)
{
    *this=*this&other;
    return *this;
}

//___________________________________________

const int Word::Get(const int &n)
{

    int ret=0;
    //printf("data:%s\n",data.to_string().c_str());
   // printf("n:%d\n",n);
    std::bitset<4> g(std::string("0000"));
    int pos = WORD_SIZE-(n*sizeof(int))-1;
    for(int i = sizeof(int)-1;i>=0;i--)
    {
        pos= WORD_SIZE-(n*sizeof(int))-i-1;
        g[3-i]=data[pos];
       // printf("ret%d:%d\n",i,g.to_ulong());
    }
    ret=g.to_ulong();
   // printf("ret=%d\n",g.to_ulong());
    return ret;

}

//___________________________________________

const int Word::Get()
{
    return (int)data.to_ulong();
}
//___________________________________________

Cpu::Cpu()
{
    memset(flags,0,sizeof(flags));
    for(int i = 0;i<REGISTER_MAX;i++)
    {
        registers[i]=Word(0);

    }

}

//___________________________________________

Cpu::~Cpu()
{


}

//___________________________________________

void Cpu::ParseAddress(int *data,int direct,int relative)
{
      if(data[0]==direct)
        {
              REG(AR)=Word(0x0,data[1],data[2],data[3]);
        }
        else if(data[0]==relative)
         {
             REG(AR)=Mem[Word(0x0,data[1],data[2],data[3]).Get()];
         }

}

//___________________________________________

InstructionType Cpu::Parse()
{
    int data[4];
    for(int i = 0; i< 4;i++)
    {
        data[i]=REG(IR).Get(i);
    }


    if(data[0]==0 || data[0]==8)
    {
        std::cout <<"###AND###"<<std::endl;
        ParseAddress(data,0,8);
        return AND;
    }
    else if(data[0]==1 || data[0]==9)
    {
        std::cout <<"###ADD###"<<std::endl;
        ParseAddress(data,1,9);


        return ADD;
    }
    else if(data[0]==2 || data[0]==0xA)
    {
        std::cout <<"###LDA###"<<std::endl;
        ParseAddress(data,2,0xA);
        return LDA;
    }
    else if(data[0]==3 || data[0]==0xB)
    {
       std::cout <<"###STA###"<<std::endl;
        ParseAddress(data,3,0xB);
        return STA;
    }
    else if(data[0]==4 || data[0]==0xC)
    {
        std::cout <<"###BUN###"<<std::endl;
        ParseAddress(data,4,0xC);
        return BUN;
    }
    else if(data[0]==5 || data[0]==0xD)
    {
       std::cout <<"###BSA###"<<std::endl;
        ParseAddress(data,5,0xD);
        return BSA;
    }
    else if(data[0]==6 || data[0]==0xE)
    {
        std::cout <<"###ISZ###"<<std::endl;
        ParseAddress(data,6,0xE);
        return ISZ;
    }
    else if(data[0]==7)
    {
        if(data[1]==8 && data[2]==0 && data[3]==0)
        {
            return CLA;
        }
        else if(data[1]==0 && data[2]==0 && data[3]==1)
        {
            return HLT;
        }
    }

    return HLT;
}
//___________________________________________

void Cpu::Execute(const std::vector<Word> &program)
{
    Mem=program;

    for(int i = 0;i<REGISTER_MAX;i++)
    {
        registers[i]=Word(0);

    }

   // PrintRegisters();
   // PrintFlags();
    PrintRAM();

    while(true)
    {
        REG(AR)=REG(PC);


        REG(PC)=Word(REG(PC).Get()+1);

        if(REG(AR).Get()>=program.size())
        {
            return;
        }
        REG(IR)=Mem[REG(AR).Get()];
        InstructionType type=Parse();

        switch(type)
        {
        case HLT:
            return;
            break;
        case AND:
            And();
            break;
        case ADD:
            Add();
            break;
        case LDA:
            Lda();
            break;
        case STA:
            Sta();
            break;
        case BUN:
            Bun();
            break;
        case BSA:
            Bsa();
            break;
         case ISZ:
            Isz();
            break;
        default:
           std::cout <<"Error:Unknown instruction!"<<std::endl;
            return;
            break;

        }
        PrintRegisters();
        PrintFlags();
        //printRAM();

    }


}

//___________________________________________

void Cpu::And()
{

    //printf("And AR=%d\n",REG(AR).get());
    REG(DR)=Mem[REG(AR).Get()];
    REG(AC)&=REG(DR);
    REG(SC)=Word(0);
}

//___________________________________________


void Cpu::Add()
{
    //printf("Add AR=%d\n",REG(AR).Get());
    REG(DR)=Mem[REG(AR).Get()];
   // printf("Add dr=%d\n", REG(DR).Get());
    REG(AC)+=REG(DR);
    flags[E]=REG(AC).carry;
   // printf("Add ACs=%s\n", REG(AC).data.to_string().c_str());
  //   printf("Add ac=%d\n", REG(AC).Get());
    REG(SC)=Word(0);

}

//___________________________________________

void Cpu::Lda()
{
    REG(DR)=Mem[REG(AR).Get()];
    REG(AC)=REG(DR);
    REG(SC)=Word(0);

}

//___________________________________________

void Cpu::Sta()
{
    Mem[REG(AR).Get()]=REG(AC);
    REG(SC)=Word(0);

}

//___________________________________________

void Cpu::Bun()
{
    REG(PC)=REG(AR);
    REG(SC)=Word(0);

}

//___________________________________________

void Cpu::Bsa()
{
    Mem[REG(AR).Get()]=REG(PC);
    REG(PC)=Word(REG(AR).Get()+1);
    REG(SC)=Word(0);
}

//___________________________________________

void Cpu::Isz()
{
    REG(DR)=Mem[REG(AR).Get()];
    REG(DR)=Word(REG(DR).Get()+1);
    Mem[REG(AR).Get()]=REG(DR);
    if(REG(DR).Get()==0)
    {
        printf("girdi\n");
        REG(PC)=Word(REG(PC).Get()+1);
    }
    REG(SC)=Word(0);
}

//___________________________________________

void Cpu::PrintRAM()
{
    std::cout <<"-----------RAM-------------"<<std::endl;
        for(int i = 0; i < Mem.size();i++)
        {
           std::cout <<i<<"|"<<Mem[i].data.to_string()<<"("<<std::hex<<Mem[i].Get()<<")"<<std::endl;
        }

}

//___________________________________________

void Cpu::PrintRegisters()
{
    std::cout <<"-----------Registers------------"<<std::endl;
   std::cout <<std::hex<<
   "AC="<<REG(AC).Get()<<
   " | AR="<<REG(AR).Get()<<
   " | PC="<<REG(PC).Get()<<
   " | IR="<<REG(IR).Get()<<
   " | DR="<<REG(DR).Get()<<
   " | TR="<<REG(TR).Get()<<
   " | INPR="<<REG(INPR).Get()<<
   " | OUTR="<<REG(OUTR).Get()<<
   std::endl;

}

//___________________________________________

void Cpu::PrintFlags()
{
    std::cout <<"-----------Flags------------"<<std::endl;
    std::cout <<
    "I="<<flags[I]<<
    " | S="<<flags[S]<<
    " | E="<<flags[E]<<
    " | R="<<flags[R]<<
    " | IEN="<<flags[IEN]<<
    " | FGI="<<flags[FGI]<<
    " | FGO="<<flags[FGO]<<
      std::endl;


}
//___________________________________________

