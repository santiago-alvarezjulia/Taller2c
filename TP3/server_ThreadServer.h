#ifndef SERVER_H
#define SERVER_H

#include "common_socket.h"
#include "server_Pelicula.h"
#include "server_Funcion.h"
#include "server_thread.h"
#include "server_FuncionesProtected.h"
#include <vector>
#include <map>
#include <string>

class ThreadServer : public Thread {
	Socket socket;
	bool esta_vivo;
	std::vector<std::multimap<std::string, Pelicula>> peliculas;
	FuncionesProtected& funciones;
	void send_genero_idioma_edad(
		std::multimap<std::string, Pelicula>& peliculas_segun_data);
	void send_funciones_dia();
	void reservar_asiento();
	void asientos();
	
	public:
		ThreadServer(Socket& socket, 
			std::vector<std::multimap<std::string, Pelicula>>& peliculas, 
			FuncionesProtected& funciones);
		ThreadServer(ThreadServer&& other); 
		virtual void run();
		bool ha_terminado();
		~ThreadServer();
};

#endif // SERVER_H
