#include "common_SocketError.h"
#include <exception>
#include <string>
using std::string;

SocketError::SocketError(string message_error) noexcept : 
	message_error(message_error) {}

const char* SocketError::what() const noexcept {
	return (const char*)this->message_error.c_str();	
}

SocketError::~SocketError() noexcept {}
