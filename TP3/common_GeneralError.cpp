#include "common_GeneralError.h"
#include <exception>
#include <errno.h>
#include <cstdio>
#include <cstdarg>
#include <cstring>

GeneralError::GeneralError(const char* fmt, ...) noexcept {
	int _errno = errno;
	va_list args;
	va_start(args, fmt);
	int s = vsnprintf(this->message_error, BUFF_LEN, fmt, args);
	va_end(args);
	strncpy(this->message_error + s, strerror(_errno), BUFF_LEN - s);
	this->message_error[BUFF_LEN - 1] = 0;
}

const char* GeneralError::what() const noexcept {
	return this->message_error;	
}

GeneralError::~GeneralError() noexcept {}
