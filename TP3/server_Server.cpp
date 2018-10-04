#include "server_Server.h"
#include "common_socket.h"
#include "server_Pelicula.h"
#include "server_Funcion.h"
#include "server_Sala.h"
#include <iostream>
#include <vector>
#include <map>
#include <string>
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

 
Server::Server(Socket& socket, 
	std::vector<std::multimap<std::string, Pelicula>>& peliculas, 
	std::vector<std::multimap<std::string, Funcion>>& funciones) : 
	socket(std::move(socket)), peliculas(peliculas), funciones(funciones) {}


void Server::run() {
	while (true) {
		cout << "esperando function" << endl;
		
		unsigned char function;
		int i = this->socket.receive_(&function, sizeof(unsigned char));
		if (i == 0) {
			cout << "fin esperando function" << endl;
			return;
		}
	
		cout << "Function: " << function << endl;
	
		if (function == FUNCTION_GENERO) {
			send_genero_idioma_edad(this->peliculas[2]);
		} else if (function == FUNCTION_IDIOMA) {
			send_genero_idioma_edad(this->peliculas[0]);
		} else if (function == FUNCTION_EDAD) {
			send_genero_idioma_edad(this->peliculas[1]);
		} else if (function == FUNCTION_FUNCIONES_DIA) {
			send_funciones_dia();
		} else if (function == FUNCTION_RESERVA) {
			reservar_asiento();
		} else if (function == FUNCTION_ASIENTOS) {
			asientos();
		}
	}
}

void Server::send_genero_idioma_edad(
	multimap<string, Pelicula>& peliculas_segun_data) {
	// recibo la longitud de data (puede ser un idioma, una edad o un genero)
	unsigned int len_data;
	this->socket.receive_((unsigned char*)&len_data, sizeof(unsigned int));
	
	// recibo la data	
	string data(len_data, '0');
	this->socket.receive_((unsigned char*) data.c_str(), len_data);
	
	std::pair<multimap<string, Pelicula>::iterator, 
		multimap<string, Pelicula>::iterator> ret;
    ret = peliculas_segun_data.equal_range(data);
    for (multimap<string, Pelicula>::iterator it = ret.first; it != ret.second; 
		++it) {
		string titulo_pelicula = it->second.getTitulo();
		unsigned int len_titulo_pelicula = titulo_pelicula.size();
		this->socket.send_((unsigned char*)&len_titulo_pelicula, 
			sizeof(unsigned int));
		this->socket.send_((unsigned char*)titulo_pelicula.c_str(), 
			len_titulo_pelicula);
	}
	// fin envio titulos de peliculas
	unsigned int fin_envio_socket = FIN_ENVIO_SOCKET;
	this->socket.send_((unsigned char*)&fin_envio_socket, sizeof(unsigned int));
}

void Server::send_funciones_dia() {
	// recibo la longitud de la fecha
	unsigned int len_fecha;
	this->socket.receive_((unsigned char*)&len_fecha, sizeof(unsigned int));
	
	// recibo la fecha	
	string fecha(len_fecha, '0');
	this->socket.receive_((unsigned char*)fecha.c_str(), len_fecha);
	
	std::pair<multimap<string, Funcion>::iterator, 
		multimap<string, Funcion>::iterator> ret;
    ret = this->funciones[0].equal_range(fecha);
    for (multimap<string, Funcion>::iterator it = ret.first; it != ret.second; 
		++it) {
		// mando el id de la funcion
		string id_funcion = it->second.getId();
		unsigned int len_id_funcion = id_funcion.size();
		this->socket.send_((unsigned char*)&len_id_funcion, 
			sizeof(unsigned int));
		this->socket.send_((unsigned char*)id_funcion.c_str(), len_id_funcion);
		
		// mando el titulo de la pelicula de la funcion
		string titulo_pelicula = it->second.getTitulo();
		unsigned int len_titulo_pelicula = titulo_pelicula.size();
		this->socket.send_((unsigned char*)&len_titulo_pelicula, 
			sizeof(unsigned int));
		this->socket.send_((unsigned char*)titulo_pelicula.c_str(), 
			len_titulo_pelicula);
		
		// mando el id de la sala de la funcion
		string id_sala = it->second.getIdSala();
		unsigned int len_id_sala = id_sala.size();
		this->socket.send_((unsigned char*)&len_id_sala, sizeof(unsigned int));
		this->socket.send_((unsigned char*)id_sala.c_str(), len_id_sala);
		
		// mando la hora de la pelicula
		string hora = it->second.getHora();
		unsigned int len_hora = hora.size();
		this->socket.send_((unsigned char*)&len_hora, sizeof(unsigned int));
		this->socket.send_((unsigned char*)hora.c_str(), len_hora);
		
		bool esta_agotada_funcion = it->second.esta_agotada();
		string estado_funcion;
		if (esta_agotada_funcion) {
			estado_funcion = "[AGOTADA]";
		} else {
			estado_funcion = "[NO AGOTADA]";
		}
		unsigned int len_estado_funcion = estado_funcion.size();
		this->socket.send_((unsigned char*)&len_estado_funcion, 
			sizeof(unsigned int));
		this->socket.send_((unsigned char*)estado_funcion.c_str(), 
			len_estado_funcion);
	}
	// fin envio funciones de la fecha
	unsigned int fin_envio_socket = FIN_ENVIO_SOCKET;
	this->socket.send_((unsigned char*)&fin_envio_socket, sizeof(unsigned int));
}

void Server::reservar_asiento() {
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
	
	std::pair<multimap<string, Funcion>::iterator, 
		multimap<string, Funcion>::iterator> ret;
    ret = this->funciones[1].equal_range(id_funcion);
    
    bool reserva_completada = ret.first->second.reservarAsiento(fila, columna);
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

void Server::asientos() {
	// recibo la longitud del id_funcion
	unsigned int len_id_funcion;
	this->socket.receive_((unsigned char*)&len_id_funcion, 
		sizeof(unsigned int));
	
	// recibo el id_funcion	
	string id_funcion(len_id_funcion, '0');
	this->socket.receive_((unsigned char*)id_funcion.c_str(), len_id_funcion);
	
	std::pair<multimap<string, Funcion>::iterator, 
		multimap<string, Funcion>::iterator> ret;
    ret = this->funciones[1].equal_range(id_funcion);
    
    unsigned int cantidad_filas = ret.first->second.getCantidadFilas();
    unsigned int cantidad_columnas = ret.first->second.getCantidadColumnas();
    vector<vector<char>> asientos = ret.first->second.getAsientos();
    
    // envio la cantidad de columnas
	this->socket.send_((unsigned char*)&cantidad_columnas, 
		sizeof(unsigned int));
		
    // envio la cantidad de filas
	this->socket.send_((unsigned char*)&cantidad_filas, sizeof(unsigned int));
    
    // envio el resto de las filas que representan asientos
    string fila = "";
    int col = 0;
	string columnas_posibles = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	for (vector<vector<char>>::iterator it = asientos.begin(); 
		it != asientos.end(); ++it) {
		fila += columnas_posibles[col];
		for (std::vector<char>::iterator it2 = (*it).begin(); 
			it2 != (*it).end(); ++it2) {
			fila += '\t',
			fila += *it2;
		}
		
		// envio la fila
		unsigned int len_fila = fila.size();
		this->socket.send_((unsigned char*)&len_fila, sizeof(unsigned int));
		this->socket.send_((unsigned char*)fila.c_str(), len_fila);
			
		col++;
		fila = "";
	}
    
}


Server::~Server() {
	this->socket.shutdown_rw();
}
