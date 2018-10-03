#include "client_Client.h"
#include <iostream>
#include <string>
using std::string;

Client::Client(char* hostname, char* port) {
	this->socket = Socket socket_client();
	this->socket.connect_(hostname, port);
}

void idioma(std::string idioma);
void edad(std::string edad);
void genero(std::string genero);
void funciones_del_dia(std::string fecha);
void asientos_funcion(std::string id_funcion);
void reservar_asiento(std::string id_funcion, std::string fila, 
	std::string columna);


Client::~Client() {
	this->socket.shutdown_rw();
}
