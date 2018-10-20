#ifndef MULTI_CLIENT_ACCEPTOR_H
#define MULTI_CLIENT_ACCEPTOR_H

#include "server_thread.h"
#include "common_socket.h"
#include "server_ThreadServer.h"
#include "server_Funcion.h"
#include "server_Pelicula.h"
#include "server_FuncionesProtected.h"
#include <map>
#include <string>
#include <vector>

class Multi_Client_Acceptor : public Thread {
	Socket socket_aceptador;
	std::vector<std::multimap<std::string, Pelicula>>& peliculas;
	FuncionesProtected& funciones;
	std::vector<ThreadServer> threads;
	bool esta_vivo;
	
	public:
		Multi_Client_Acceptor(char* port, std::vector
			<std::multimap<std::string, Pelicula>>& peliculas, 
			FuncionesProtected& funciones);
		virtual void run();
		void frenar();
		~Multi_Client_Acceptor();
};

#endif // MULTI_CLIENT_ACCEPTOR_H
