#include "server_ThreadServer.h"
#include "common_socket.h"
#include "server_Pelicula.h"
#include "server_Funcion.h"
#include "server_Sala.h"
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <utility>
#define FUNCTION_GENERO '1'
#define FUNCTION_IDIOMA '2'
#define FUNCTION_EDAD '3'
#define FUNCTION_FUNCIONES_DIA '4'
#define FUNCTION_RESERVA '5'
#define FUNCTION_ASIENTOS '6'
#define FIN_ENVIO_SOCKET 0
using std::string;
using std::vector;
using std::multimap;
using std::cout;
using std::endl;
using std::map;

 
ThreadServer::ThreadServer(Socket& socket, 
	std::vector<std::multimap<std::string, Pelicula>>& peliculas, 
	FuncionesProtected& funciones) : 
	socket(std::move(socket)), peliculas(peliculas), funciones(funciones) {
	this->esta_vivo = true;
}

ThreadServer::ThreadServer(ThreadServer&& other) : 
	socket(std::move(other.socket)), peliculas(std::move(other.peliculas)), 
	funciones(other.funciones) {
	this->esta_vivo = other.esta_vivo;
}

void ThreadServer::run() {
	while (this->esta_vivo) {
		// recibo cual es la funcion/comando que pide el cliente
		unsigned char function;
		int i = this->socket.receive_(&function, sizeof(unsigned char));
		if (i == 0) {
			// cerro el socket el cliente -> se termina la transmision de datos
			break;
		}
	
		switch (function) {
			case FUNCTION_GENERO:
				// vector[2] -> multimap con clave genero y valor pelicula
				send_genero_idioma_edad(this->peliculas[2]);
				break;
			case FUNCTION_IDIOMA:
				// vector[0] -> multimap con clave idioma y valor pelicula
				send_genero_idioma_edad(this->peliculas[0]);
				break;
			case FUNCTION_EDAD:
				// vector[1] -> multimap con clave edad y valor pelicula
				send_genero_idioma_edad(this->peliculas[1]);
				break;
			case FUNCTION_FUNCIONES_DIA:
				send_funciones_dia();
				break;
			case FUNCTION_RESERVA:
				reservar_asiento();
				break;
			case FUNCTION_ASIENTOS:
				asientos();
				break;
		}
		
	}
	// finalizo la ejecucion del hilo
	this->esta_vivo = false;
}

void ThreadServer::send_genero_idioma_edad(
	multimap<string, Pelicula>& peliculas_segun_data) {
	// recibo la longitud de data (puede ser un idioma, una edad o un genero)
	unsigned int len_data;
	this->socket.receive_((unsigned char*)&len_data, sizeof(unsigned int));
	
	// recibo la data	
	string data(len_data, '0');
	this->socket.receive_((unsigned char*) data.c_str(), len_data);
	
	// iteradores que me indican los limites del rango en el cual la clave 
	// del multimap es data
	std::pair<multimap<string, Pelicula>::iterator, 
		multimap<string, Pelicula>::iterator> ret;
    ret = peliculas_segun_data.equal_range(data);
    
    // verifico que al menos haya una coincidencia
    if (ret.first != ret.second) {
		// hay al menos 1 coincidencia
		// envio un unsigned int que indica que la operacion es válida
		unsigned int operacion_valida = 0;
		this->socket.send_((unsigned char*)&operacion_valida, 
			sizeof(unsigned int));
		
		// recorro en el multimap solo las claves que coinciden
		for (multimap<string, Pelicula>::iterator it = ret.first; 
			it != ret.second; ++it) {
			// envio longitud del titulo y el titulo
			string titulo_pelicula = it->second.getTitulo();
			unsigned int len_titulo_pelicula = titulo_pelicula.size();
			this->socket.send_((unsigned char*)&len_titulo_pelicula, 
				sizeof(unsigned int));
			this->socket.send_((unsigned char*)titulo_pelicula.c_str(), 
				len_titulo_pelicula);
		}
		
		// fin envio titulos de peliculas
		unsigned int fin_envio_socket = FIN_ENVIO_SOCKET;
		this->socket.send_((unsigned char*)&fin_envio_socket, 
			sizeof(unsigned int));
	} else {
		// no hay coincidencias
		// envio un unsigned int que indica que la operacion es inválida
		unsigned int operacion_invalida = 1;
		this->socket.send_((unsigned char*)&operacion_invalida, 
			sizeof(unsigned int));
	}
}

void ThreadServer::send_funciones_dia() {
	// recibo la longitud de la fecha
	unsigned int len_fecha;
	this->socket.receive_((unsigned char*)&len_fecha, sizeof(unsigned int));
	
	// recibo la fecha	
	string fecha(len_fecha, '0');
	this->socket.receive_((unsigned char*)fecha.c_str(), len_fecha);
	
	// delego el envio de las funciones de la fecha
	this->funciones.send_funciones_dia(fecha, this->socket);
	
	// fin envio funciones de la fecha
	unsigned int fin_envio_socket = FIN_ENVIO_SOCKET;
	this->socket.send_((unsigned char*)&fin_envio_socket, sizeof(unsigned int));
}

void ThreadServer::reservar_asiento() {
	// recibo la longitud del id_funcion
	unsigned int len_id_funcion;
	this->socket.receive_((unsigned char*)&len_id_funcion, 
		sizeof(unsigned int));
	
	// recibo el id_funcion	
	string id_funcion(len_id_funcion, '0');
	this->socket.receive_((unsigned char*)id_funcion.c_str(), len_id_funcion);
	
	// recibo la longitud de la fila
	unsigned int len_fila;
	this->socket.receive_((unsigned char*)&len_fila, 
		sizeof(unsigned int));
	
	// recibo la fila
	string fila(len_fila, '0');
	this->socket.receive_((unsigned char*)fila.c_str(), len_fila);
	
	// recibo la longitud de la columna
	unsigned int len_columna;
	this->socket.receive_((unsigned char*)&len_columna, 
		sizeof(unsigned int));
	
	// recibo la columna
	string columna(len_columna, '0');
	this->socket.receive_((unsigned char*)columna.c_str(), len_columna);
	
	// delego la reserva del asiento
	bool reserva_completada = this->funciones.reservar_asiento(id_funcion, 
		fila, columna);
    if (reserva_completada) {
		// se reservó el asiento correctamente
		unsigned int reserva_correcta = 0;
		this->socket.send_((unsigned char*)&reserva_correcta, 
			sizeof(unsigned int));
	} else {
		// no se reservó el asiento correctamente
		unsigned int reserva_incorrecta = 1;
		this->socket.send_((unsigned char*)&reserva_incorrecta, 
			sizeof(unsigned int));
	}
}

void ThreadServer::asientos() {
	// recibo la longitud del id_funcion
	unsigned int len_id_funcion;
	this->socket.receive_((unsigned char*)&len_id_funcion, 
		sizeof(unsigned int));
	
	// recibo el id_funcion	
	string id_funcion(len_id_funcion, '0');
	this->socket.receive_((unsigned char*)id_funcion.c_str(), len_id_funcion);
	
	// delego en server_FuncionesProtected
	this->funciones.asientos(id_funcion, this->socket);
}

bool ThreadServer::ha_terminado() {
	return !this->esta_vivo;
}

ThreadServer::~ThreadServer() {
	this->socket.shutdown_rw();
}
