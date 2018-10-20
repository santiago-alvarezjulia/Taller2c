#ifndef COMMON_SOCKET_H
#define COMMON_SOCKET_H

#include <stddef.h>
#include <stdbool.h>
#include <netdb.h>

class Socket {
	int socket_fd;
	explicit Socket(int fd);
	
	public:
		Socket();
		Socket(Socket&& socket);
		Socket(const Socket&) = delete;
		Socket& operator=(const Socket&) = delete;
		
		void bind_and_listen(const char* port);
		void connect_(const char* hostname, const char* service_name);
		Socket accept_();
		
		int send_(uint8_t* chunk, int sizeof_chunk);
		int receive_(uint8_t* chunk, int sizeof_chunk);
		
		void shutdown_rd();
		void shutdown_wr();
		void shutdown_rdwr();
		
		~Socket();
};

#endif // COMMON_SOCKET_H
