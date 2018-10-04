#ifndef SERVER_H
#define SERVER_H

#include "common_socket.h"
#include "server_Pelicula.h"
#include "server_Funcion.h"
#include <vector>
#include <map>
#include <string>

class Server {
	Socket socket;
	std::vector<std::multimap<std::string, Pelicula>> peliculas;
	std::vector<std::multimap<std::string, Funcion>> funciones;
	void send_genero_idioma_edad(
		std::multimap<std::string, Pelicula>& peliculas_segun_data);
	void send_funciones_dia();
	void reservar_asiento();
	void asientos();
	
	public:
		Server(Socket& socket, 
			std::vector<std::multimap<std::string, Pelicula>>& peliculas, 
			std::vector<std::multimap<std::string, Funcion>>& funciones);
		void run();
		~Server();
};

#endif // SERVER_H
