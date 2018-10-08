#ifndef SOCKET_ERROR_H
#define SOCKET_ERROR_H

#include <exception>
#include <string>

class SocketError : public std::exception {
	std::string message_error;

	public:
		explicit SocketError(std::string message_error) noexcept;
		virtual const char* what() const noexcept;
		virtual ~SocketError() noexcept;
};

#endif // SOCKET_ERROR_H
