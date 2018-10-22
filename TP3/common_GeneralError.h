#ifndef GENERAL_ERROR_H
#define GENERAL_ERROR_H

#include <exception>
#define BUFF_LEN 256

class GeneralError : public std::exception {
	char message_error[BUFF_LEN];

	public:
		explicit GeneralError(const char* fmt, ...) noexcept;
		virtual const char* what() const noexcept;
		virtual ~GeneralError() noexcept;
};

#endif // GENERAL_ERROR_H
