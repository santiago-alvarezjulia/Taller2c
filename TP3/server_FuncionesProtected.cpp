#include "server_Funcion.h"
#include "server_FuncionesProtected.h"
#include "server_lock.h"
#include <map>
#include <string>
#include <mutex>
#include <vector>
using std::vector;
using std::map;
using std::string;
using std::mutex;

FuncionesProtected::FuncionesProtected() {
	this->funciones = map<string, Funcion>();
	this->mutex_funciones = map<string, mutex*>();
}

void FuncionesProtected::emplace_funcion(string id_funcion, 
	Funcion& funcion) {
	this->funciones.emplace(id_funcion, funcion);	
	this->mutex_funciones.emplace(id_funcion, new mutex());
}

void FuncionesProtected::send_funciones_dia(string fecha, Socket& socket) {
	// recorro las funciones, si no son del dia, las descarto
    for (map<string, Funcion>::iterator it = this->funciones.begin(); 
		it != this->funciones.end(); ++it) {
		// aca verifico el dia
		if (it->second.getFecha() != fecha) {
			continue;
		}
		
		// Lockeo el mutex segun el id
		string id_funcion = it->second.getId();
		Lock(*this->mutex_funciones.at(id_funcion));
		
		// mando el id de la funcion
		unsigned int len_id_funcion = id_funcion.size();
		socket.send_((unsigned char*)&len_id_funcion, 
			sizeof(unsigned int));
		socket.send_((unsigned char*)id_funcion.c_str(), len_id_funcion);
		
		// mando el titulo de la pelicula de la funcion
		string titulo_pelicula = it->second.getTitulo();
		unsigned int len_titulo_pelicula = titulo_pelicula.size();
		socket.send_((unsigned char*)&len_titulo_pelicula, 
			sizeof(unsigned int));
		socket.send_((unsigned char*)titulo_pelicula.c_str(), 
			len_titulo_pelicula);
		
		// mando el id de la sala de la funcion
		string id_sala = it->second.getIdSala();
		unsigned int len_id_sala = id_sala.size();
		socket.send_((unsigned char*)&len_id_sala, sizeof(unsigned int));
		socket.send_((unsigned char*)id_sala.c_str(), len_id_sala);
		
		// mando la hora de la pelicula
		string hora = it->second.getHora();
		unsigned int len_hora = hora.size();
		socket.send_((unsigned char*)&len_hora, sizeof(unsigned int));
		socket.send_((unsigned char*)hora.c_str(), len_hora);
		
		bool esta_agotada_funcion = it->second.esta_agotada();
		string estado_funcion;
		if (esta_agotada_funcion) {
			estado_funcion = "AGOTADA";
		} else {
			estado_funcion = "NO AGOTADA";
		}
		unsigned int len_estado_funcion = estado_funcion.size();
		socket.send_((unsigned char*)&len_estado_funcion, 
			sizeof(unsigned int));
		socket.send_((unsigned char*)estado_funcion.c_str(), 
			len_estado_funcion);
	}
}

bool FuncionesProtected::reservar_asiento(string id_funcion, string fila, 
	string columna) {
	// Lockeo el mutex segun el id
	Lock(*this->mutex_funciones.at(id_funcion));
	
	// iterador a la funcion
	map<string, Funcion>::iterator it = this->funciones.find(id_funcion);
    
    return it->second.reservarAsiento(fila, columna);
}

void FuncionesProtected::asientos(string id_funcion, Socket& socket) {
	// Lockeo el mutex segun el id
	Lock(*this->mutex_funciones.at(id_funcion));
	
	// iterador a la funcion
	map<string, Funcion>::iterator it = this->funciones.find(id_funcion);
    
    unsigned int cantidad_filas = it->second.getCantidadFilas();
    unsigned int cantidad_columnas = it->second.getCantidadColumnas();
    vector<vector<char>> asientos = it->second.getAsientos();
    
    // envio los mismos datos que en ThreadServer::send_funciones_dia pero solo
    // para la funcion recibida y ademas la fecha
    // mando el titulo de la pelicula de la funcion
	string titulo_pelicula = it->second.getTitulo();
	unsigned int len_titulo_pelicula = titulo_pelicula.size();
	socket.send_((unsigned char*)&len_titulo_pelicula, 
		sizeof(unsigned int));
	socket.send_((unsigned char*)titulo_pelicula.c_str(), 
		len_titulo_pelicula);
	
	// mando el id de la sala de la funcion
	string id_sala = it->second.getIdSala();
	unsigned int len_id_sala = id_sala.size();
	socket.send_((unsigned char*)&len_id_sala, sizeof(unsigned int));
	socket.send_((unsigned char*)id_sala.c_str(), len_id_sala);
	
	// mando la fecha de la pelicula
	string fecha = it->second.getFecha();
	unsigned int len_fecha = fecha.size();
	socket.send_((unsigned char*)&len_fecha, sizeof(unsigned int));
	socket.send_((unsigned char*)fecha.c_str(), len_fecha);
	
	// mando la hora de la pelicula
	string hora = it->second.getHora();
	unsigned int len_hora = hora.size();
	socket.send_((unsigned char*)&len_hora, sizeof(unsigned int));
	socket.send_((unsigned char*)hora.c_str(), len_hora);
	
	bool esta_agotada_funcion = it->second.esta_agotada();
	string estado_funcion;
	if (esta_agotada_funcion) {
		estado_funcion = "AGOTADA";
	} else {
		estado_funcion = "NO AGOTADA";
	}
	unsigned int len_estado_funcion = estado_funcion.size();
	socket.send_((unsigned char*)&len_estado_funcion, 
		sizeof(unsigned int));
	socket.send_((unsigned char*)estado_funcion.c_str(), 
		len_estado_funcion);
			
    // envio la cantidad de columnas
	socket.send_((unsigned char*)&cantidad_columnas, 
		sizeof(unsigned int));
		
    // envio la cantidad de filas
	socket.send_((unsigned char*)&cantidad_filas, sizeof(unsigned int));
    
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
		unsigned int len_fila = fila.size();
		socket.send_((unsigned char*)&len_fila, sizeof(unsigned int));
		socket.send_((unsigned char*)fila.c_str(), len_fila);
			
		col++;
		fila = "";
	}
}

FuncionesProtected::~FuncionesProtected() {
	for (map<string, mutex*>::iterator it = this->mutex_funciones.begin(); 
		it != this->mutex_funciones.end(); ++it) {
		delete it->second;
	}
}
