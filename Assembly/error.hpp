#ifndef ERROR_HPP
#define ERROR_HPP

//___________________________________________

#include <iostream>
#include <exception>
#include <string>

//___________________________________________

// define error codes 
typedef enum {
	ERR_NONE = 0, 
	ERR_INVALID_OPERATOR,
	//ERR_DIVISION_BY_ZERO,
	ERR_SYNTAX_ERROR,
	ERR_ALREADY_DECLARED,
	ERR_UNDECLARED_VAR,
	ERR_MAX
} ErrorType;


//___________________________________________



struct Error// : public std::exception
{

public:
	Error(const ErrorType &type_=ERR_NONE,const std::string &file_ = __FILE__, const int &line_ = __LINE__);

	const std::string &getFile() const;
	const int &getLine() const;
	const std::string &getMessage() const;
	const ErrorType &getType() const;

protected:
	std::string file;
	int line;
	std::string msg;
	ErrorType type;
	

	
	
};

//___________________________________________

#endif