#ifndef MULTI_CLIENT_ACCEPTOR_H
#define MULTI_CLIENT_ACCEPTOR_H

#include "server_thread.h"
#include "common_socket.h"
#include "server_ThreadServer.h"
#include "server_Funcion.h"
#include "server_Pelicula.h"
#include <map>
#include <string>
#include <vector>

class Multi_Client_Acceptor : public Thread {
	Socket socket_aceptador;
	std::vector<std::multimap<std::string, Pelicula>> peliculas;
	std::vector<std::multimap<std::string, Funcion>> funciones;
	std::vector<ThreadServer> threads;
	bool is_alive;
	
	public:
		Multi_Client_Acceptor(Socket& socket, std::vector
			<std::multimap<std::string, Pelicula>>& peliculas, 
			std::vector<std::multimap<std::string, Funcion>>& funciones);
		virtual void run();
		void stop();
		~Multi_Client_Acceptor();
};

#endif // MULTI_CLIENT_ACCEPTOR_H
