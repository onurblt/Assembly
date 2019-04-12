#include "cpu.hpp"


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

Word::Word(const int &op, const int &address)
{
	data = (op << 12) + address;
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
        REG[i]=Word(0);

    }
	symbolAddressTable.clear();

}

//___________________________________________

Cpu::~Cpu()
{


}

//___________________________________________

void Cpu::parseAddress(int *data)
{
	std::bitset<4> first = data[0];
	if (first[3] == 0)
        {
              REG[AR]=Word(0x0,data[1],data[2],data[3]);
        }
	else if (first[3] == 1)
         {
             REG[AR]=Mem[Word(0x0,data[1],data[2],data[3]).Get()];
         }

}

//___________________________________________

InstructionType Cpu::parse()
{
    int data[4];
    for(int i = 0; i< 4;i++)
    {
        data[i]=REG[IR].Get(i);
    }


	if (data[0] == 0 || data[0] == 8)
    {
        std::cout <<"###AND"<<std::endl;
        parseAddress(data);
        return INSTR_AND;
    }
    else if(data[0]==1 || data[0]==9)
    {
        std::cout <<"###ADD"<<std::endl;
        parseAddress(data);


		return INSTR_ADD;
    }
    else if(data[0]==2 || data[0]==0xA)
    {
		std::cout << "("<<REG[AR].Get()<<")LDA"<<std::endl;
        parseAddress(data);
		return INSTR_LDA;
    }
    else if(data[0]==3 || data[0]==0xB)
    {
		std::cout << "(" << REG[AR].Get() << ")STA" << std::endl;
        parseAddress(data);
		return INSTR_STA;
    }
    else if(data[0]==4 || data[0]==0xC)
    {
		std::cout << "(" << REG[AR].Get() << ")BUN" << std::endl;
        parseAddress(data);
		return INSTR_BUN;
    }
    else if(data[0]==5 || data[0]==0xD)
    {
		std::cout << "(" << REG[AR].Get() << ")BSA" << std::endl;
        parseAddress(data);
		return INSTR_BSA;
    }
    else if(data[0]==6 || data[0]==0xE)
    {
		std::cout << "(" << REG[AR].Get() << ")ISZ" << std::endl;
        parseAddress(data);
		return INSTR_ISZ;
    }
    else if(data[0]==7)
    {
        if(data[1]==8 && data[2]==0 && data[3]==0)
        {
			std::cout << "(" << REG[AR].Get() << ")CLA" << std::endl;
			return INSTR_CLA;
        }
		else if (data[1] == 4 && data[2] == 0 && data[3] == 0)
		{
			std::cout << "(" << REG[AR].Get() << ")CLE" << std::endl;
			return INSTR_CLE;
		}
		else if (data[1] == 2 && data[2] == 0 && data[3] == 0)
		{
			std::cout << "(" << REG[AR].Get() << ")CMA" << std::endl;
			return INSTR_CMA;
		}
		else if (data[1] == 1 && data[2] == 0 && data[3] == 0)
		{
			std::cout << "(" << REG[AR].Get() << ")CME" << std::endl;
			return INSTR_CME;
		}
		else if (data[1] == 0 && data[2] == 8 && data[3] == 0)
		{
			std::cout << "(" << REG[AR].Get() << ")CIR" << std::endl;
			return INSTR_CIR;
		}
		else if (data[1] == 0 && data[2] == 4 && data[3] == 0)
		{
			std::cout << "(" << REG[AR].Get() << ")CIL" << std::endl;
			return INSTR_CIL;
		}
		else if (data[1] == 0 && data[2] == 2 && data[3] == 0)
		{
			std::cout << "(" << REG[AR].Get() << ")INC" << std::endl;
			return INSTR_INC;
		}
		else if (data[1] == 0 && data[2] == 1 && data[3] == 0)
		{
			std::cout << "(" << REG[AR].Get() << ")BUN" << std::endl;
			return INSTR_SPA;
		}
		else if (data[1] == 0 && data[2] == 0 && data[3] == 8)
		{
			std::cout << "(" << REG[AR].Get() << ")SNA" << std::endl;
			return INSTR_SNA;
		}
		else if (data[1] == 0 && data[2] == 0 && data[3] == 4)
		{
			std::cout << "(" << REG[AR].Get() << ")SZA" << std::endl;
			return INSTR_SZA;
		}
		else if (data[1] == 0 && data[2] == 0 && data[3] == 2)
		{
			std::cout << "(" << REG[AR].Get() << ")SZE" << std::endl;
			return INSTR_SZE;
		}
        else if(data[1]==0 && data[2]==0 && data[3]==1)
        {
			std::cout << "("<<REG[AR].Get()<<")HLT" << std::endl;
			return INSTR_HLT;
        }

		
    }
	else if (data[0] == 0xF)
	{

		if (data[1] == 8 && data[2] == 0 && data[3] == 0)
		{
			return INSTR_INP;
		}
		else if (data[1] == 4 && data[2] == 0 && data[3] == 0)
		{
			return INSTR_OUT;
		}
		else if (data[1] == 2 && data[2] == 0 && data[3] == 0)
		{
			return INSTR_SKI;
		}
		else if (data[1] == 1 && data[2] == 0 && data[3] == 0)
		{
			return INSTR_SKO;
		}
		else if (data[1] == 0 && data[2] == 8 && data[3] == 0)
		{
			return INSTR_ION;
		}
		else if (data[1] == 0 && data[2] == 4 && data[3] == 0)
		{
			return INSTR_IOF;
		}
	}

	std::cout << "Error:Unknown instruction!" << std::endl;
	return INSTR_HLT;
}

//___________________________________________

void Cpu::setRAM(const std::string &script)
{
	clear();

	generateSymbolAddressTable(script);
	Lexer lexer;
	lexer.lexString(script);


	Token token = lexer.next();

	std::vector<Token> tokens;
	while (token.type != TK_END_OF_INPUT)
	{
		tokens.push_back(token);
		token = lexer.next();

	}
	Word word(0);
	int symbol=0;
	for (int i = 0; i < tokens.size(); i++)
	{
		switch (tokens[i].type)
		{
			case TK_AND:
				if ((i +1) < tokens.size())
				{
					if (tokens[i + 1].type != TK_IDENTIFIER && tokens[i + 1].type != TK_INTEGER_LITERAL)
					{
						error=Error(ERR_SYNTAX_ERROR, "RAM file", tokens[i].line);
						return;
					}
					else if (tokens[i + 1].type == TK_IDENTIFIER)
					{
						if (symbolAddressTable.find(tokens[i + 1].value) == symbolAddressTable.end())
						{
							error = Error(ERR_UNDECLARED_VAR, "RAM file", tokens[i].line);
							return;
						}
						else
						{
							symbol = symbolAddressTable[tokens[i + 1].value];
							i++;
						}
						
					}
					else if (tokens[i + 1].type == TK_INTEGER_LITERAL)
					{
						
						symbol = std::stoi(tokens[i + 1].value);
						i++;
					}
				}
				else
				{
					error=Error(ERR_SYNTAX_ERROR, "RAM file", tokens[i].line);
					return;
				}
				if ((i + 2) < tokens.size())
				{
					if (tokens[i + 2].type == TK_I && tokens[i + 2].line==tokens[i].line)
					{
						Mem.push_back(Word(8, symbol));
						i++;
					}
					else
					{
						Mem.push_back(Word(0, symbol));
					}
				}
				else
				{
					Mem.push_back(Word(0, symbol));
				}
				break;
			case TK_ADD:
				if ((i + 1) < tokens.size())
				{
					if (tokens[i + 1].type != TK_IDENTIFIER && tokens[i + 1].type!= TK_INTEGER_LITERAL)
					{
						error = Error(ERR_SYNTAX_ERROR, "RAM file", tokens[i].line);
						return;
					}
					else if (tokens[i + 1].type == TK_IDENTIFIER)
					{
						if (symbolAddressTable.find(tokens[i + 1].value) == symbolAddressTable.end())
						{
							error = Error(ERR_UNDECLARED_VAR, "RAM file", tokens[i].line);
							return;
						}
						else
						{
							symbol = symbolAddressTable[tokens[i + 1].value];
							i++;
						}
					}
					else if (tokens[i + 1].type == TK_INTEGER_LITERAL)
					{
						symbol = std::stoi(tokens[i + 1].value);
						i++;
					}
				}
				else
				{
					error = Error(ERR_SYNTAX_ERROR, "RAM file", tokens[i].line);
					return;
				}
				if ((i + 1) < tokens.size())
				{
					if (tokens[i + 1].type == TK_I && tokens[i + 1].line == tokens[i].line)
					{
						Mem.push_back(Word(9, symbol));
						i++;
					}
					else
					{
						Mem.push_back(Word(1, symbol));
					}
				}
				else
				{
					Mem.push_back(Word(1, symbol));
				}
				break;
			case TK_LDA:
				if ((i + 1) < tokens.size())
				{
					if (tokens[i + 1].type != TK_IDENTIFIER && tokens[i + 1].type != TK_INTEGER_LITERAL)
					{
						error = Error(ERR_SYNTAX_ERROR, "RAM file", tokens[i].line);
						return;
					}
					else if (tokens[i + 1].type == TK_IDENTIFIER)
					{
						if (symbolAddressTable.find(tokens[i + 1].value) == symbolAddressTable.end())
						{
							error = Error(ERR_UNDECLARED_VAR, "RAM file", tokens[i].line);
							return;
						}
						else
						{
							symbol = symbolAddressTable[tokens[i + 1].value];
							i++;
						}
					}
					else if (tokens[i + 1].type == TK_INTEGER_LITERAL)
					{
						symbol = std::stoi(tokens[i + 1].value);
						i++;
					}
				}
				else
				{
					error = Error(ERR_SYNTAX_ERROR, "RAM file", tokens[i].line);
					return;
				}
				if ((i + 1) < tokens.size())
				{
					if (tokens[i + 1].type == TK_I && tokens[i + 1].line == tokens[i].line)
					{
						Mem.push_back(Word(0xA, symbol));
						i++;
					}
					else
					{
						Mem.push_back(Word(2, symbol));
					}
				}
				else
				{
					Mem.push_back(Word(2, symbol));
				}
				break;
			case TK_STA:
				if ((i + 1) < tokens.size())
				{
					if (tokens[i + 1].type != TK_IDENTIFIER && tokens[i + 1].type != TK_INTEGER_LITERAL)
					{
						error = Error(ERR_SYNTAX_ERROR, "RAM file", tokens[i].line);
						return;
					}
					else if (tokens[i + 1].type == TK_IDENTIFIER)
					{
						if (symbolAddressTable.find(tokens[i + 1].value) == symbolAddressTable.end())
						{
							error = Error(ERR_UNDECLARED_VAR, "RAM file", tokens[i].line);
							return;
						}
						else
						{
							symbol = symbolAddressTable[tokens[i + 1].value];
							i++;
						}
					}
					else if (tokens[i + 1].type == TK_INTEGER_LITERAL)
					{
						symbol = std::stoi(tokens[i + 1].value);
						i++;
					}
				}
				else
				{
					error = Error(ERR_SYNTAX_ERROR, "RAM file", tokens[i].line);
					return;
				}
				if ((i + 1) < tokens.size())
				{
					if (tokens[i + 1].type == TK_I && tokens[i + 1].line == tokens[i].line)
					{
						Mem.push_back(Word(0xB, symbol));
						i++;
					}
					else
					{
						Mem.push_back(Word(3, symbol));
					}
				}
				else
				{
					Mem.push_back(Word(3, symbol));
				}
				break;
			case TK_BUN:
				if ((i + 1) < tokens.size())
				{
					if (tokens[i + 1].type != TK_IDENTIFIER && tokens[i + 1].type != TK_INTEGER_LITERAL)
					{
						error = Error(ERR_SYNTAX_ERROR, "RAM file", tokens[i].line);
						return;
					}
					else if (tokens[i + 1].type == TK_IDENTIFIER)
					{
						if (symbolAddressTable.find(tokens[i + 1].value) == symbolAddressTable.end())
						{
							error = Error(ERR_UNDECLARED_VAR, "RAM file", tokens[i].line);
							return;
						}
						else
						{
							symbol = symbolAddressTable[tokens[i + 1].value];
							i++;
						}
					}
					else if (tokens[i + 1].type == TK_INTEGER_LITERAL)
					{
						symbol = std::stoi(tokens[i + 1].value);
						i++;
					}
				}
				else
				{
					error = Error(ERR_SYNTAX_ERROR, "RAM file", tokens[i].line);
					return;
				}
				if ((i + 1) < tokens.size())
				{
					if (tokens[i + 1].type == TK_I && tokens[i + 1].line == tokens[i].line)
					{
						Mem.push_back(Word(0xC, symbol));
						i++;
					}
					else
					{
						Mem.push_back(Word(4, symbol));
					}
				}
				else
				{
					Mem.push_back(Word(4, symbol));
				}
				break;
			case TK_BSA:
				if ((i + 1) < tokens.size())
				{
					if (tokens[i + 1].type != TK_IDENTIFIER && tokens[i + 1].type != TK_INTEGER_LITERAL)
					{
						error = Error(ERR_SYNTAX_ERROR, "RAM file", tokens[i].line);
						return;
					}
					else if (tokens[i + 1].type == TK_IDENTIFIER)
					{
						if (symbolAddressTable.find(tokens[i + 1].value) == symbolAddressTable.end())
						{
							error = Error(ERR_UNDECLARED_VAR, "RAM file", tokens[i].line);
							return;
						}
						else
						{
							symbol = symbolAddressTable[tokens[i + 1].value];
							i++;
						}
					}
					else if (tokens[i + 1].type == TK_INTEGER_LITERAL)
					{
						symbol = std::stoi(tokens[i + 1].value);
						i++;
					}
				}
				else
				{
					error = Error(ERR_SYNTAX_ERROR, "RAM file", tokens[i].line);
					return;
				}
				if ((i + 1) < tokens.size())
				{
					if (tokens[i + 1].type == TK_I && tokens[i + 1].line == tokens[i].line)
					{
						Mem.push_back(Word(0xD, symbol));
						i++;
					}
					else
					{
						Mem.push_back(Word(5, symbol));
					}
				}
				else
				{
					Mem.push_back(Word(5, symbol));
				}
				break;
			case TK_ISZ:
				if ((i + 1) < tokens.size())
				{
					if (tokens[i + 1].type != TK_IDENTIFIER && tokens[i + 1].type != TK_INTEGER_LITERAL)
					{
						error = Error(ERR_SYNTAX_ERROR, "RAM file", tokens[i].line);
						return;
					}
					else if (tokens[i + 1].type == TK_IDENTIFIER)
					{
						if (symbolAddressTable.find(tokens[i + 1].value) == symbolAddressTable.end())
						{
							error = Error(ERR_UNDECLARED_VAR, "RAM file", tokens[i].line);
							return;
						}
						else
						{
							symbol = symbolAddressTable[tokens[i + 1].value];
							i++;
						}
					}
					else if (tokens[i + 1].type == TK_INTEGER_LITERAL)
					{
						symbol = std::stoi(tokens[i + 1].value);
						i++;
					}
				}
				else
				{
					error = Error(ERR_SYNTAX_ERROR, "RAM file", tokens[i].line);
					return;
				}
				if ((i + 1) < tokens.size())
				{
					if (tokens[i + 1].type == TK_I && tokens[i + 1].line == tokens[i].line)
					{
						Mem.push_back(Word(0xE, symbol));
						i++;
					}
					else
					{
						Mem.push_back(Word(6, symbol));
					}
				}
				else
				{
					Mem.push_back(Word(6, symbol));
				}

				break;
			case TK_CLA:
				Mem.push_back(INSTR_CLA);
				break;
			case TK_CLE:
				Mem.push_back(INSTR_CLE);
				break;
			case TK_CMA:
				Mem.push_back(INSTR_CMA);
				break;
			case TK_CME:
				Mem.push_back(INSTR_CME);
				break;
			case TK_CIR:
				Mem.push_back(INSTR_CIR);
				break;
			case TK_CIL:
				Mem.push_back(INSTR_CIL);
				break;
			case TK_INC:
				Mem.push_back(INSTR_INC);
				break;
			case TK_SPA:
				Mem.push_back(INSTR_SPA);
				break;
			case TK_SNA:
				Mem.push_back(INSTR_SNA);
				break;
			case TK_SZA:
				Mem.push_back(INSTR_SZA);
				break;
			case TK_SZE:
				Mem.push_back(INSTR_SZE);
				break;
			case TK_HLT:
				Mem.push_back(INSTR_HLT);
				break;
			case TK_INP:
				Mem.push_back(INSTR_INP);
				break;
			case TK_OUT:
				Mem.push_back(INSTR_OUT);
				break;
			case TK_SKI:
				Mem.push_back(INSTR_SKI);
				break;
			case TK_SKO:
				Mem.push_back(INSTR_SKO);
				break;
			case TK_ION:
				Mem.push_back(INSTR_ION);
				break;
			case TK_IOF:
				Mem.push_back(INSTR_IOF);
				break;
			case TK_IDENTIFIER:
				if ((i + 1) < tokens.size())
				{
					if (tokens[i + 1].type != TK_COMMA)
					{
						if (tokens[i + 1].line == tokens[i].line)
						{
							error = Error(ERR_SYNTAX_ERROR, "RAM file", tokens[i].line);
							return;
						}
						else
						{
							error = Error(ERR_INVALID_OPERATOR, "RAM file", tokens[i].line);
							return;
						}
					}
					else
					{
						i++;
					}
				}
				else
				{
					error = Error(ERR_SYNTAX_ERROR, "RAM file", tokens[i].line);
					return;
				}
				if ((i + 1) < tokens.size())
				{
					if (tokens[i + 1].type == TK_INTEGER_LITERAL)
					{
						Mem.push_back(Word(std::stoi(tokens[i + 1].value)));
						i++;
					}
					else
					{
						
						
					}
				}
				else
				{
					error = Error(ERR_SYNTAX_ERROR, "RAM file", tokens[i].line);
					return;
				}

				break;
			case TK_INTEGER_LITERAL:
				Mem.push_back(Word(std::stoi(tokens[i].value)));
			
				break;
		};

	}


}
//___________________________________________

void Cpu::generateSymbolAddressTable(const std::string &script)
{
	Lexer lexer;
	lexer.lexString(script);

	Token token = lexer.next();
	std::vector<Token> tokens;

	while (token.type != TK_END_OF_INPUT)
	{
		tokens.push_back(token);
		token = lexer.next();
	}

	for (size_t i = 0; i < tokens.size(); i++)
	{


		if (tokens[i].type == TK_IDENTIFIER)
		{
			std::string name = tokens[i].value;
			if ((i + 1) < tokens.size())
			{
			
				if (tokens[i + 1].type == TK_COMMA && tokens[i].line == tokens[i + 1].line)
				{
					
					if (symbolAddressTable.find(name) == symbolAddressTable.end())
					{
						symbolAddressTable[name] = tokens[i].line;
						i++;
					}
					else
					{
						error = Error(ERR_ALREADY_DECLARED, "RAM file", tokens[i].line);
						return;
					}

				}
			}
		}

	}



}
//___________________________________________

int Cpu::step()
{
	int res = res = REG[PC].Get();

	if (REG[AR].Get() >= 0 && REG[AR].Get()<Mem.size())
	{
		std::cout << "___________________________________________" << std::endl;
		REG[AR] = REG[PC];
		

		REG[PC] = Word(REG[PC].Get() + 1);
		if (REG[AR].Get() < 0 || REG[AR].Get() >= Mem.size())
		{
			error = Error(ERR_SYNTAX_ERROR,"RAM File",0);
			return -1;
		}

		REG[IR] = Mem[REG[AR].Get()];
		InstructionType type = parse();


		if (REG[AR].Get() < 0 || REG[AR].Get() >= Mem.size())
		{
			error = Error(ERR_SYNTAX_ERROR, "RAM File", 0);
			return -1;
		}
		

		switch (type)
		{
		case INSTR_HLT:
			hlt();
			break;
		case INSTR_AND:
			and();
			break;
		case INSTR_ADD:
			add();
			break;
		case INSTR_LDA:
			lda();
			break;
		case INSTR_STA:
			sta();
			break;
		case INSTR_BUN:
			bun();
			break;
		case INSTR_BSA:
			bsa();
			break;
		case INSTR_ISZ:
			isz();
			break;
		case INSTR_CLA:
			cla();
			break;
		case INSTR_CLE:
			cle();
			break;
		case INSTR_CMA:
			cma();
			break;
		case INSTR_CME:
			cme();
			break;
		case INSTR_CIR:
			cir();
			break;
		case INSTR_CIL:
			cil();
			break;
		case INSTR_INC:
			inc();
			break;
		case INSTR_SPA:
			spa();
			break;
		case INSTR_SNA:
			sna();
			break;
		case INSTR_SZA:
			sza();
			break;
		case INSTR_SZE:
			sze();
			break;
		case INSTR_INP:
			inp();
			break;
		case INSTR_OUT:
			out();
			break;
		case INSTR_SKI:
			ski();
			break;
		case INSTR_SKO:
			sko();
			break;
		case INSTR_ION:
			ion();
			break;
		case INSTR_IOF:
			iof();
			break;
		default:
			//std::cout << "Error:Unknown instruction!" << std::endl;
			//return false;
			break;

		}
		printRegisters();
		printFlags();
		//printRAM();

	}
	else
	{
		return -1;
	}

	return res;

}

//___________________________________________

void Cpu::clear()
{
	for (int i = 0; i<REGISTER_MAX; i++)
	{
		REG[i] = Word(0);

	}
	Mem.clear();
	symbolAddressTable.clear();
	error = Error(ERR_NONE);
}

//___________________________________________

void Cpu::and()
{
   REG[DR]=Mem[REG[AR].Get()];
   REG[AC]&=REG[DR];
   REG[SC]=Word(0);
}

//___________________________________________


void Cpu::add()
{

    printf("Add AR=%d\n",REG[AR].Get());
    REG[DR]=Mem[REG[AR].Get()];
   // printf("Add dr=%d\n", REG[DR].Get());
   REG[AC]+=REG[DR];
	
    flags[FLAG_E]=REG[AC].carry;
   // printf("Add ACs=%s\n",REG[AC].data.to_string().c_str());
  //   printf("Add ac=%d\n",REG[AC].Get());
   REG[SC]=Word(0);

}

//___________________________________________

void Cpu::lda()
{
    REG[DR]=Mem[REG[AR].Get()];
   REG[AC]=REG[DR];
   REG[SC]=Word(0);

}

//___________________________________________

void Cpu::sta()
{
    Mem[REG[AR].Get()]=REG[AC];
   REG[SC]=Word(0);

}

//___________________________________________

void Cpu::bun()
{
    REG[PC]=REG[AR];
   REG[SC]=Word(0);

}

//___________________________________________

void Cpu::bsa()
{
    Mem[REG[AR].Get()]=REG[PC];
    REG[PC]=Word(REG[AR].Get()+1);
   REG[SC]=Word(0);
}

//___________________________________________

void Cpu::isz()
{
    REG[DR]=Mem[REG[AR].Get()];
    REG[DR]=Word(REG[DR].Get()+1);
    Mem[REG[AR].Get()]=REG[DR];
    if(REG[DR].Get()==0)
    {
        //printf("girdi\n");
        REG[PC]=Word(REG[PC].Get()+1);
    }
   REG[SC]=Word(0);
}

//___________________________________________

void Cpu::cla()
{
	REG[AC] = Word(0);
}

//___________________________________________

void Cpu::cle()
{
	flags[FLAG_E] = false;
}

//___________________________________________

void Cpu::cma()
{
	REG[AC] = ~REG[AC];
}

//___________________________________________

void Cpu::cme()
{
	flags[FLAG_E] = !flags[FLAG_E];
}

//___________________________________________

void Cpu::cir()
{
	
	REG[AC]=REG[AC].Shr();
	REG[AC].data[0] = flags[FLAG_E];
	flags[FLAG_E] =REG[AC].carry;


}

//___________________________________________

void Cpu::cil()
{
	REG[AC] =REG[AC].Shl();
	REG[AC].data[15] = flags[FLAG_E];
	flags[FLAG_E] =REG[AC].carry;
}

//___________________________________________

void Cpu::inc()
{
	REG[AC] = Word(REG[AC].Get() + 1);
}

//___________________________________________

void Cpu::spa()
{
	if (REG[AC].data[15] == 0)
	{
		REG[PC] = Word(REG[PC].Get() + 1);
	}
}

//___________________________________________

void Cpu::sna()
{
	if (REG[AC].data[15] == 1)
	{
		REG[PC] = Word(REG[PC].Get() + 1);
	}
}

//___________________________________________

void Cpu::sza()
{
	if (REG[AC].Get() == 0)
	{
		REG[PC] = Word(REG[PC].Get() + 1);
	}
}

//___________________________________________

void Cpu::sze()
{
	if (flags[FLAG_E] == 0)
	{
		REG[PC] = Word(REG[PC].Get() + 1);
	}
}

//___________________________________________

void Cpu::hlt()
{
	REG[AR] = Word(-1);
}

//___________________________________________

void Cpu::inp()
{
	int val;
	std::cout << "IN=";
	std::cin >> val;
	REG[INPR] = Word(val);


	REG[AC] = REG[INPR];
	flags[FLAG_FGI] = 0;
	
}

//___________________________________________

void Cpu::out()
{

	REG[OUTR] =REG[AC];
	std::cout <<"OUT="<< std::hex << REG[OUTR].Get()<<std::endl;
	flags[FLAG_FGO] = 0;
}

//___________________________________________

void Cpu::ski()
{
	if (flags[FLAG_FGI] == 1)
	{
		REG[PC] = Word(REG[PC].Get() + 1);
	}
}

//___________________________________________

void Cpu::sko()
{
	if (flags[FLAG_FGO] == 1)
	{
		REG[PC] = Word(REG[PC].Get() + 1);
	}
}

//___________________________________________

void Cpu::ion()
{
	flags[FLAG_IEN] = true;
}

//___________________________________________

void Cpu::iof()
{
	flags[FLAG_IEN] = false;
}

//___________________________________________

void Cpu::printRAM()
{
	//std::cout << "___________________________________________" << std::endl;
	//std::cout << "___________________RAM____________________" << std::endl;
        for(int i = 0; i < Mem.size();i++)
        {
           std::cout <<i<<"|"<<Mem[i].data.to_string()<<"("<<std::hex<<Mem[i].Get()<<")"<<std::endl;
        }

}

//___________________________________________

void Cpu::printRegisters()
{
    //std::cout <<"___________________________________________"<<std::endl;
	//std::cout <<"________________Registers__________________" << std::endl;
   std::cout <<std::hex<<
   "AC="<<REG[AC].Get()<<
   " | AR="<<REG[AR].Get()<<
   " | PC="<<REG[PC].Get()<<
   " | IR="<<REG[IR].Get()<<
   " | DR="<<REG[DR].Get()<<
   " | TR="<<REG[TR].Get()<<
   " | INPR="<<REG[INPR].Get()<<
   " | OUTR="<<REG[OUTR].Get()<<
   std::endl;

}

//___________________________________________

void Cpu::printFlags()
{
	//std::cout << "___________________________________________" << std::endl;
	//std::cout << "____________________Flags__________________" << std::endl;
    std::cout <<
		"I=" << flags[FLAG_I] <<
		" | S=" << flags[FLAG_S] <<
    " | E="<<flags[FLAG_E]<<
	" | R=" << flags[FLAG_R] <<
	" | IEN=" << flags[FLAG_IEN] <<
	" | FGI=" << flags[FLAG_FGI] <<
	" | FGO=" << flags[FLAG_FGO] <<
      std::endl;


}
//___________________________________________

