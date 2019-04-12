#include "lexer.hpp"

//___________________________________________

Lexer::Lexer()
{
	source = "";
	pos = 0;
	start = 0;
	line = 0;
}

//___________________________________________

Lexer::~Lexer()
{


}
//___________________________________________

void Lexer::lexString(const std::string &_source)
{
	source = _source;
	pos = 0;
	start = 0;
	line = 0;
}
//___________________________________________

const Token Lexer::next()
{
	// length of the source code.
	int len = source.length();

	// return the end token if this is an empty string
	if (len == 0) {
		Token token;
		token.line = line;
		token.type = TK_END_OF_INPUT;
		return token;

	}

	// loop while current position index is smaller than the
	// length of the input source.
	while (pos < len) {

		// get current character
		char ch = source[pos];
		char next = pos + 1 < source.length() ? source[pos + 1] : '\0';
		// Set the current token start position and advance the position
		// to point to the next character
		start = pos++;

		if (ch == ',')
		{
			Token token;
			token.type = TK_COMMA;
			token.line = line;
			return token;
		}
			


		// skip if current character is a white space or a new line
		if (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t')
		{
			if (ch == '\n')
			{
				line++;
			}
			continue;

		}
		// deal with comments. Comments start with //
		// so check first that next character exists and is not the end of
		// source, secondly test that next character is /
		// (m_pos points to the next character already beacues we incremented it)
		if (ch == ';' && pos < len /* && next == '/' */) {
			// skip everything until we encounter either end of string
			// or a new line
			while (++pos < len && source[pos] != '\n');
			line++;
			continue;
		}

		// if this is a alphabetic character
		if (isalpha(ch)) return lexIdentifier();

		// is the character a number ?
		if (isdigit(ch)||ch=='-') return lexNumber();


		if (ch == '\"' || ch == '\'')
		{
			return lexString();
		}


		// match the possible operator
		// if none match then in default set it to invalid
		TokType type;
		switch (ch) {
		case ',':
			type = TK_COMMA;
		default:
			type = TK_INVALID;
			break;
		}
		Token token;
		token.type = type;
		token.line = line;
		token.value = std::string(source, start, pos - start);
		return token;
	}

	// done iterating through the string. Return EndOfInput Token
	Token token;
	token.type = TK_END_OF_INPUT;
	token.line = line;
	return token;

}

const Token Lexer::lexIdentifier()
{
	// read while position is within the string and
	// next character is a alpha numeric
	while (pos < source.length() && isalnum(source[pos]))
	{
		pos++;
	}

	std::string lexeme(source, start, pos - start);

	TokType type;
	if (lexeme == "I")
	{
		type = TokType::TK_I;
	}
	else if (lexeme == "AND")
	{
		type = TokType::TK_AND;
	}
	else if (lexeme == "ADD")
	{
		type = TokType::TK_ADD;
	}
	else if (lexeme == "LDA")
	{
		type = TokType::TK_LDA;
	}
	else if (lexeme == "STA")
	{
		type = TokType::TK_STA;
	}
	else if (lexeme == "BUN")
	{
		type = TokType::TK_BUN;
	}
	else if (lexeme == "BSA")
	{
		type = TokType::TK_BSA;
	}
	else if (lexeme == "ISZ")
	{
		type = TokType::TK_ISZ;
	}
	else if (lexeme == "CLA")
	{
		type = TokType::TK_CLA;
	}
	else if (lexeme == "CLE")
	{
		type = TokType::TK_CLE;
	}
	else if (lexeme == "CMA")
	{
		type = TokType::TK_CMA;
	}
	else if (lexeme == "CME")
	{
		type = TokType::TK_CME;
	}
	else if (lexeme == "CIR")
	{
		type = TokType::TK_CIR;
	}
	else if (lexeme == "CIL")
	{
		type = TokType::TK_CIL;
	}
	else if (lexeme == "INC")
	{
		type = TokType::TK_INC;
	}
	else if (lexeme == "SPA")
	{
		type = TokType::TK_SPA;
	}
	else if (lexeme == "SNA")
	{
		type = TokType::TK_SNA;
	}
	else if (lexeme == "SZA")
	{
		type = TokType::TK_SZA;
	}
	else if (lexeme == "SZE")
	{
		type = TokType::TK_SZE;
	}
	else if (lexeme == "HLT")
	{
		type = TokType::TK_HLT;
	}
	else if (lexeme == "INP")
	{
		type = TokType::TK_INP;
	}
	else if (lexeme == "OUT")
	{
		type = TokType::TK_OUT;
	}
	else if (lexeme == "SKI")
	{
		type = TokType::TK_SKI;
	}
	else if (lexeme == "SKO")
	{
		type = TokType::TK_SKO;
	}
	else if (lexeme == "ION")
	{
		type = TokType::TK_ION;
	}
	else if (lexeme == "IOF")
	{
		type = TokType::TK_IOF;
	}
	else
	{
		type = TokType::TK_IDENTIFIER;
	}

	Token token;
	token.type = type;
	token.line = line;
	token.value = lexeme;

	return token;
}
const Token Lexer::lexNumber()
{
	// read while position is within the string and
	// next character is a number
	bool isfloat = false;
	bool isnegative = false;
	while (pos < source.length() && (isdigit(source[pos]) || source[pos] == '.' || source[pos]=='-'))
	{
		if (source[pos] == '-')
		{
			if (isnegative)
			{

			}
			isnegative = true;
		}
		
		if (source[pos] == '.')
		{
			if (isfloat)
			{
				//ERR_SYNTAX_ERROR
			}

			isfloat = true;
		}
		pos++;
	}
	// Done. Create new token
	Token token;

	token.type = isfloat ? TK_FLOAT_LITERAL : TK_INTEGER_LITERAL;
	token.value = std::string(source, start, pos - start);
	token.line = line;

	return token;
}

const Token Lexer::lexString()
{
	// read while position is within the string and
	// next character is a number
	bool started = false;
	while (pos < source.length())
	{
		if (source[pos] == '\'' || source[pos] == '\"')
		{
			pos++;
			break;
		}
		pos++;
	}
	// Done. Create new token
	Token token;

	token.type = TK_STRING_LITERAL;
	token.value = std::string(source, start + 1, pos - start - 2);
	token.line = line;

	return token;
}
