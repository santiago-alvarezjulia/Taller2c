#include "server_ArchivoEntradaError.h"
#include <exception>
#include <string>
using std::string;

ArchivoEntradaError::ArchivoEntradaError(string message_error) noexcept : 
	message_error(message_error) {}

const char* ArchivoEntradaError::what() const noexcept {
	return (const char*)this->message_error.c_str();	
}

ArchivoEntradaError::~ArchivoEntradaError() noexcept {}
