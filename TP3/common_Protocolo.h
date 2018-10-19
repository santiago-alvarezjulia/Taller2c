#ifndef COMMON_PROTOCOLO_H
#define COMMON_PROTOCOLO_H

#include "common_socket.h"
#include <string>

class Protocolo {
    Socket socket;
	
	public:
		Protocolo(Socket socket, char* hostname, char* port);
		
		void send_string(std::string& data);
		void send_unsigned_char(unsigned char data);

		std::string receive_string();
		unsigned int receive_unsigned_int();
		
		~Protocolo();
};

#endif // COMMON_PROTOCOLO_H