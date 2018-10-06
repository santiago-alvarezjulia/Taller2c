#ifndef ARCHIVO_ENTRADA_ERROR_H
#define ARCHIVO_ENTRADA_ERROR_H

#include <exception>
#include <string>

class ArchivoEntradaError : public std::exception {
	std::string message_error;

	public:
		ArchivoEntradaError(std::string message_error) noexcept;
		virtual const char* what() const noexcept;
		virtual ~ArchivoEntradaError() noexcept;
};

#endif // ARCHIVO_ENTRADA_ERROR_H
