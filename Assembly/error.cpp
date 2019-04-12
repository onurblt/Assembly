#include "error.hpp"

const std::string messages[ERR_MAX] = { "No error",
"Gecersiz islem",
//"Division by zero",
"Syntax hatasi",
"Degisken zaten deklare edildi",
"Tanimlanmamis degisken" };

Error::Error(const ErrorType &type_, const std::string &file_, const int &line_)
:
//std::exception(("ERROR:" + messages[type_] +" at line " + std::to_string(line_) + " in " + file_).c_str()),
file(file_),
line(line_),
msg("HATA:" + std::to_string(line_) + " nolu satirda "+messages[type_]),// + " at line " + std::to_string(line) + " in " + file),
type(type_)
{

}

//___________________________________________

const std::string &Error::getFile() const 
{
	return file; 
}

//___________________________________________

const int &Error::getLine() const 
{ 
	return line;
}


//___________________________________________

const std::string &Error::getMessage() const 
{
	return msg;
}

//___________________________________________

const ErrorType &Error::getType() const 
{
	return type;
}

//___________________________________________
