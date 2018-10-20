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
	protocolo(socket), peliculas(peliculas), funciones(funciones) {
	this->esta_vivo = true;
}

ThreadServer::ThreadServer(ThreadServer&& other) : 
	protocolo(std::move(other.protocolo)), peliculas(std::move(other.peliculas)), 
	funciones(other.funciones) {
	this->esta_vivo = other.esta_vivo;
}

void ThreadServer::run() {
	while (this->esta_vivo) {
		// recibo cual es la funcion/comando que pide el cliente
		unsigned char function = this->protocolo.receive_unsigned_char();
		if (function == '0') {
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
	// recibo la data	
	string data = this->protocolo.receive_string();
	
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
		this->protocolo.send_unsigned_int(operacion_valida);
		
		// recorro en el multimap solo las claves que coinciden
		for (multimap<string, Pelicula>::iterator it = ret.first; 
			it != ret.second; ++it) {
			// envio el titulo
			string titulo_pelicula = it->second.getTitulo();
			this->protocolo.send_string(titulo_pelicula);
		}
		
		// fin envio titulos de peliculas
		unsigned int fin_envio_socket = FIN_ENVIO_SOCKET;
		this->protocolo.send_unsigned_int(fin_envio_socket);
	} else {
		// no hay coincidencias
		// envio un unsigned int que indica que la operacion es inválida
		unsigned int operacion_invalida = 1;
		this->protocolo.send_unsigned_int(operacion_invalida);
	}
}

void ThreadServer::send_funciones_dia() {
	// recibo la fecha	
	string fecha = this->protocolo.receive_string();
	
	// recibo un map con las funciones de la fecha correspondiente
	map<string, Funcion> funciones_del_dia = this->funciones
		.send_funciones_dia(fecha);
	
	for (map<string, Funcion>::iterator it = funciones_del_dia.begin(); 
		it != funciones_del_dia.end(); ++it) {
		// mando el id de la funcion
		string id_funcion = it->first;
		this->protocolo.send_string(id_funcion);
		
		// mando el titulo de la pelicula de la funcion
		string titulo_pelicula = it->second.getTitulo();
		this->protocolo.send_string(titulo_pelicula);
		
		// mando el id de la sala de la funcion
		string id_sala = it->second.getIdSala();
		this->protocolo.send_string(id_sala);
		
		// mando la hora de la pelicula
		string hora = it->second.getHora();
		this->protocolo.send_string(hora);
		
		bool esta_agotada_funcion = it->second.esta_agotada();
		string estado_funcion;
		if (esta_agotada_funcion) {
			estado_funcion = "AGOTADA";
		} else {
			estado_funcion = "NO AGOTADA";
		}
		this->protocolo.send_string(estado_funcion);
	}

	// fin envio funciones de la fecha
	unsigned int fin_envio_socket = FIN_ENVIO_SOCKET;
	this->protocolo.send_unsigned_int(fin_envio_socket);
}

void ThreadServer::reservar_asiento() {
	// recibo el id_funcion	
	string id_funcion = this->protocolo.receive_string();
	
	// recibo la fila
	string fila = this->protocolo.receive_string();
	
	// recibo la columna
	string columna = this->protocolo.receive_string();
	
	// delego la reserva del asiento
	bool reserva_completada = this->funciones.reservar_asiento(id_funcion, 
		fila, columna);
	
	unsigned int reserva;
    if (reserva_completada) {
		// se reservó el asiento correctamente
		reserva = 0;
	} else {
		// no se reservó el asiento correctamente
		reserva = 1;
	}

	this->protocolo.send_unsigned_int(reserva);
}

void ThreadServer::asientos() {
	// recibo el id_funcion	
	string id_funcion = this->protocolo.receive_string();

	// delego en server_FuncionesProtected
	Funcion funcion = this->funciones.asientos(id_funcion);

	unsigned int cantidad_filas = funcion.getCantidadFilas();
    unsigned int cantidad_columnas = funcion.getCantidadColumnas();
    vector<vector<char>> asientos = funcion.getAsientos();
    
    // envio los mismos datos que en ThreadServer::send_funciones_dia pero solo
    // para la funcion recibida y ademas la fecha
    // mando el titulo de la pelicula de la funcion
	string titulo_pelicula = funcion.getTitulo();
	this->protocolo.send_string(titulo_pelicula);
	
	// mando el id de la sala de la funcion
	string id_sala = funcion.getIdSala();
	this->protocolo.send_string(id_sala);
	
	// mando la fecha de la pelicula
	string fecha = funcion.getFecha();
	this->protocolo.send_string(fecha);
	
	// mando la hora de la pelicula
	string hora = funcion.getHora();
	this->protocolo.send_string(hora);
	
	bool esta_agotada_funcion = funcion.esta_agotada();
	string estado_funcion;
	if (esta_agotada_funcion) {
		estado_funcion = "AGOTADA";
	} else {
		estado_funcion = "NO AGOTADA";
	}
	this->protocolo.send_string(estado_funcion);
			
    // envio la cantidad de columnas
	this->protocolo.send_unsigned_int(cantidad_columnas);
		
    // envio la cantidad de filas
	this->protocolo.send_unsigned_int(cantidad_filas);
    
    // envio el resto de las filas que representan asientos
    string fila = "";
    int col = 0;
	string columnas_posibles = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	for (vector<vector<char>>::iterator it2 = asientos.begin(); 
		it2 != asientos.end(); ++it2) {
		fila += columnas_posibles[col];
		for (std::vector<char>::iterator it3 = (*it2).begin(); 
			it3 != (*it2).end(); ++it3) {
			fila += '\t',
			fila += *it3;
		}
		
		// envio la fila
		this->protocolo.send_string(fila);
			
		col++;
		fila = "";
	}
}

bool ThreadServer::ha_terminado() {
	return !this->esta_vivo;
}

ThreadServer::~ThreadServer() {
	//this->socket.shutdown_rw();
}
