#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include "common_socket.h"

class Client {
	Socket socket;
	
	public:
		Client(Socket& socket_client, char* hostname, char* port);
		void recibo_idioma_edad_genero(unsigned char funcion, std::string data);
		void funciones_del_dia(unsigned char function, std::string fecha);
		void asientos_funcion(unsigned char function, std::string id_funcion);
		void reservar_asiento(unsigned char function, std::string id_funcion, 
			std::string fila, std::string columna);
		~Client();
};

#endif // CLIENT_H
