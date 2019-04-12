#ifndef LEXER_H
#define LEXER_H

//___________________________________________

#include "token.hpp"
#include "error.hpp"

//___________________________________________

class Lexer
{
public:
	Lexer();

	void lexString(const std::string &source);
	virtual ~Lexer();
    
	const Token next();

protected:
	const Token lexIdentifier();
	
	const Token lexNumber();

	const Token lexString();
   

	int line;

	// hold the source code we are lexing
	std::string source;

	// current read position
	int pos;

	// current token start position
	int start;


};

//___________________________________________

#endif