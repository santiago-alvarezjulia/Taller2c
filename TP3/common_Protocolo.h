#ifndef COMMON_PROTOCOLO_H
#define COMMON_PROTOCOLO_H

#include "common_socket.h"
#include <string>

class Protocolo {
    Socket socket;
	
	public:
		// constructor cliente
		Protocolo(Socket socket, char* hostname, char* port);
		// constructor servidor
		explicit Protocolo(Socket& socket);
		Protocolo(Protocolo&& other);
		
		void send_string(std::string& data);
		void send_unsigned_char(uint8_t data);
		void send_unsigned_int(uint32_t data);

		std::string receive_string();
		uint8_t receive_unsigned_char();
		uint32_t receive_unsigned_int();
		
		~Protocolo();
};

#endif // COMMON_PROTOCOLO_H
