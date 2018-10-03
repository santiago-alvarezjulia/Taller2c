#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include "common_socket.h"

class Client {
	Socket socket;
	
	public:
		Client(char* hostname, char* port);
		void idioma(std::string idioma);
		void edad(std::string edad);
		void genero(std::string genero);
		void funciones_del_dia(std::string fecha);
		void asientos_funcion(std::string id_funcion);
		void reservar_asiento(std::string id_funcion, std::string fila,
			std::string columna);
		~Client();
};

#endif // CLIENT_H
