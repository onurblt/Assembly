#include "cpu.hpp"

#define REG(R) registers[R]


//___________________________________________

Word::Word()
{
    data=0;
	carry = false;
}

//___________________________________________

Word::Word(const int &_data)
{
   data=_data;
   carry = false;
}

//___________________________________________

Word::Word(const std::bitset<WORD_SIZE> &_data)
{
   data=_data;
   carry = false;
}

//___________________________________________

Word::Word(const Word &other)
{
    data=other.data;
	carry = other.carry;

}
Word& Word::operator = (const Word &other)
{
    data=other.data;
	carry = other.carry;
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
        bool sum = (data[i] ^ other.data[i]) ^ temp.carry;
        temp.carry = (data[i] && other.data[i]) || (data[i] && temp.carry) || (other.data[i] && temp.carry);
        temp.data[i] = sum;
    }
	printf("c=%d\n", temp.carry);
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

Word Word::operator~()
{
	Word temp(this->data);
	for (int i = 0; i < WORD_SIZE; i++)
	{
		temp.data.flip(i);
	}

	return temp;
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

Word Word::Shl()
{
	Word temp(data);
	temp.carry = data[0];
	for (int i = 1; i < WORD_SIZE; i++)
	{
		temp.data[i - 1] = data[i];
	}


	return temp;
}

//___________________________________________

Word Word::Shr()
{
	Word temp(data);
	temp.carry = data[15];
	for (int i = 0; i < WORD_SIZE-1; i++)
	{
		temp.data[i + 1] = data[i];
	}


	return temp;
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
        std::cout <<"###AND"<<std::endl;
        ParseAddress(data,0,8);
        return AND;
    }
    else if(data[0]==1 || data[0]==9)
    {
        std::cout <<"###ADD"<<std::endl;
        ParseAddress(data,1,9);


        return ADD;
    }
    else if(data[0]==2 || data[0]==0xA)
    {
		std::cout << "("<<REG(AR).Get()<<")LDA"<<std::endl;
        ParseAddress(data,2,0xA);
        return LDA;
    }
    else if(data[0]==3 || data[0]==0xB)
    {
		std::cout << "(" << REG(AR).Get() << ")STA" << std::endl;
        ParseAddress(data,3,0xB);
        return STA;
    }
    else if(data[0]==4 || data[0]==0xC)
    {
		std::cout << "(" << REG(AR).Get() << ")BUN" << std::endl;
        ParseAddress(data,4,0xC);
        return BUN;
    }
    else if(data[0]==5 || data[0]==0xD)
    {
		std::cout << "(" << REG(AR).Get() << ")BSA" << std::endl;
        ParseAddress(data,5,0xD);
        return BSA;
    }
    else if(data[0]==6 || data[0]==0xE)
    {
		std::cout << "(" << REG(AR).Get() << ")ISZ" << std::endl;
        ParseAddress(data,6,0xE);
        return ISZ;
    }
    else if(data[0]==7)
    {
        if(data[1]==8 && data[2]==0 && data[3]==0)
        {
			std::cout << "(" << REG(AR).Get() << ")CLA" << std::endl;
            return CLA;
        }
		else if (data[1] == 4 && data[2] == 0 && data[3] == 0)
		{
			std::cout << "(" << REG(AR).Get() << ")CLE" << std::endl;
			return CLE;
		}
		else if (data[1] == 2 && data[2] == 0 && data[3] == 0)
		{
			std::cout << "(" << REG(AR).Get() << ")CMA" << std::endl;
			return CMA;
		}
		else if (data[1] == 1 && data[2] == 0 && data[3] == 0)
		{
			std::cout << "(" << REG(AR).Get() << ")CME" << std::endl;
			return CME;
		}
		else if (data[1] == 0 && data[2] == 8 && data[3] == 0)
		{
			std::cout << "(" << REG(AR).Get() << ")CIR" << std::endl;
			return CIR;
		}
		else if (data[1] == 0 && data[2] == 4 && data[3] == 0)
		{
			std::cout << "(" << REG(AR).Get() << ")CIL" << std::endl;
			return CIL;
		}
		else if (data[1] == 0 && data[2] == 2 && data[3] == 0)
		{
			std::cout << "(" << REG(AR).Get() << ")INC" << std::endl;
			return INC;
		}
		else if (data[1] == 0 && data[2] == 1 && data[3] == 0)
		{
			std::cout << "(" << REG(AR).Get() << ")BUN" << std::endl;
			return SPA;
		}
		else if (data[1] == 0 && data[2] == 0 && data[3] == 8)
		{
			std::cout << "(" << REG(AR).Get() << ")SNA" << std::endl;
			return SNA;
		}
		else if (data[1] == 0 && data[2] == 0 && data[3] == 4)
		{
			std::cout << "(" << REG(AR).Get() << ")SZA" << std::endl;
			return SZA;
		}
		else if (data[1] == 0 && data[2] == 0 && data[3] == 2)
		{
			std::cout << "(" << REG(AR).Get() << ")SZE" << std::endl;
			return SZE;
		}
        else if(data[1]==0 && data[2]==0 && data[3]==1)
        {
			std::cout << "("<<REG(AR).Get()<<")HLT" << std::endl;
            return HLT;
        }

		
    }
	else if (data[0] == 0xF)
	{

		if (data[1] == 8 && data[2] == 0 && data[3] == 0)
		{
			return INP;
		}
		else if (data[1] == 4 && data[2] == 0 && data[3] == 0)
		{
			return OUT;
		}
		else if (data[1] == 2 && data[2] == 0 && data[3] == 0)
		{
			return SKI;
		}
		else if (data[1] == 1 && data[2] == 0 && data[3] == 0)
		{
			return SKO;
		}
		else if (data[1] == 0 && data[2] == 8 && data[3] == 0)
		{
			return ION;
		}
		else if (data[1] == 0 && data[2] == 4 && data[3] == 0)
		{
			return IOF;
		}
	}

	std::cout << "Error:Unknown instruction!" << std::endl;
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

	while (REG(AR).Get() >= 0 && REG(AR).Get()<program.size())
    {
		std::cout << "___________________________________________" << std::endl;
        REG(AR)=REG(PC);


        REG(PC)=Word(REG(PC).Get()+1);

        REG(IR)=Mem[REG(AR).Get()];
        InstructionType type=Parse();

        switch(type)
        {
        case HLT:
            Hlt();
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
		 case CLA:
			 Cla();
			 break;
		 case CLE:
			 Cle();
			 break;
		 case CMA:
			 Cma();
			 break;
		 case CME:
			 Cme();
			 break;
		 case CIR:
			 Cir();
			 break;
		 case CIL:
			 Cil();
			 break;
		 case INC:
			 Inc();
			 break;
		 case SPA:
			 Spa();
			 break;
		 case SNA:
			 Sna();
			 break;
		 case SZA:
			 Sza();
			 break;
		 case SZE:
			 Sze();
			 break;
		 case INP:
			 Inp();
			 break;
		 case OUT:
			 Out();
			 break;
		 case SKI:
			 Ski();
			 break;
		 case SKO:
			 Sko();
			 break;
		 case ION:
			 Ion();
			 break;
		 case IOF:
			 Iof();
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
        //printf("girdi\n");
        REG(PC)=Word(REG(PC).Get()+1);
    }
    REG(SC)=Word(0);
}

//___________________________________________

void Cpu::Cla()
{
	REG(AC) = Word(0);
}

//___________________________________________

void Cpu::Cle()
{
	flags[E] = false;
}

//___________________________________________

void Cpu::Cma()
{
	REG(AC) = ~REG(AC);
}

//___________________________________________

void Cpu::Cme()
{
	flags[E] = !flags[E];
}

//___________________________________________

void Cpu::Cir()
{
	
	REG(AC)=REG(AC).Shr();
	REG(AC).data[0] = flags[E];
	flags[E] = REG(AC).carry;


}

//___________________________________________

void Cpu::Cil()
{
	REG(AC) = REG(AC).Shl();
	REG(AC).data[15] = flags[E];
	flags[E] = REG(AC).carry;
}

//___________________________________________

void Cpu::Inc()
{
	REG(AC) = Word(REG(AC).Get() + 1);
}

//___________________________________________

void Cpu::Spa()
{
	if (REG(AC).data[15] == 0)
	{
		REG(PC) = Word(REG(PC).Get() + 1);
	}
}

//___________________________________________

void Cpu::Sna()
{
	if (REG(AC).data[15] == 1)
	{
		REG(PC) = Word(REG(PC).Get() + 1);
	}
}

//___________________________________________

void Cpu::Sza()
{
	if (REG(AC).Get() == 0)
	{
		REG(PC) = Word(REG(PC).Get() + 1);
	}
}

//___________________________________________

void Cpu::Sze()
{
	if (flags[E] == 0)
	{
		REG(PC) = Word(REG(PC).Get() + 1);
	}
}

//___________________________________________

void Cpu::Hlt()
{
	REG(AR) = Word(-1);
}

//___________________________________________

void Cpu::Inp()
{
	int val;
	std::cout << "IN=";
	std::cin >> val;
	REG(INPR) = Word(val);


	REG(AC) = REG(INPR);
	flags[FGI] = 0;
	
}

//___________________________________________

void Cpu::Out()
{

	REG(OUTR) = REG(AC);
	std::cout <<"OUT="<< std::hex << REG(OUTR).Get()<<std::endl;
	flags[FGO] = 0;
}

//___________________________________________

void Cpu::Ski()
{
	if (flags[FGI] == 1)
	{
		REG(PC) = Word(REG(PC).Get() + 1);
	}
}

//___________________________________________

void Cpu::Sko()
{
	if (flags[FGO] == 1)
	{
		REG(PC) = Word(REG(PC).Get() + 1);
	}
}

//___________________________________________

void Cpu::Ion()
{
	flags[IEN] = true;
}

//___________________________________________

void Cpu::Iof()
{
	flags[IEN] = false;
}

//___________________________________________

void Cpu::PrintRAM()
{
	//std::cout << "___________________________________________" << std::endl;
	//std::cout << "___________________RAM____________________" << std::endl;
        for(int i = 0; i < Mem.size();i++)
        {
           std::cout <<i<<"|"<<Mem[i].data.to_string()<<"("<<std::hex<<Mem[i].Get()<<")"<<std::endl;
        }

}

//___________________________________________

void Cpu::PrintRegisters()
{
    //std::cout <<"___________________________________________"<<std::endl;
	//std::cout <<"________________Registers__________________" << std::endl;
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
	//std::cout << "___________________________________________" << std::endl;
	//std::cout << "____________________Flags__________________" << std::endl;
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

