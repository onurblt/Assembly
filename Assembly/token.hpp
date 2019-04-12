#ifndef TOKEN_HPP
#define TOKEN_HPP

//___________________________________________

#include <string>

//___________________________________________


// TokenType enum holds entries for all possible distinct tokens
// in our grammar and special control tokens
typedef enum TokType {
	TK_INVALID,        //! invalid input. Useful for error handling
	TK_IDENTIFIER,     //! identifier like: foo or bar
	TK_COMMA,          //! ,
	TK_INTEGER_LITERAL, //! 1, 23, 435, -12, 0
	TK_FLOAT_LITERAL,   //! 1.1, -45.2, -.5
	TK_STRING_LITERAL,  //! "hello world!"
	TK_I,//! I
	TK_AND,//! AND
	TK_ADD,//! ADD
	TK_LDA,
	TK_STA,
	TK_BUN,
	TK_BSA,
	TK_ISZ,
	TK_CLA,
	TK_CLE,
	TK_CMA,
	TK_CME,
	TK_CIR,
	TK_CIL,
	TK_INC,
	TK_SPA,
	TK_SNA,
	TK_SZA,
	TK_SZE,
	TK_HLT,
	TK_INP,
	TK_OUT,
	TK_SKI,
	TK_SKO,
	TK_ION,
	TK_IOF,
	TK_END_OF_INPUT      //! end of the input. Stop lexing further

}TokType;

//___________________________________________


// Token structure represents a very simple Token that is returned from
// the lexer. This here holds the token TokenType and its textual value
class Token {
public:
	Token()
	{

	}
	~Token()
	{

	}

	TokType   type;
	std::string value;
	int         line, column;
};

//___________________________________________


#endif