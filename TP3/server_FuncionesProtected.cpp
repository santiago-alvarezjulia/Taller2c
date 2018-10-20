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

void FuncionesProtected::agregar_funcion(string& id_funcion, 
	Funcion& funcion) {
	this->funciones.emplace(id_funcion, funcion);	
	this->mutex_funciones.emplace(id_funcion, new mutex());
}

map<string, Funcion> FuncionesProtected::send_funciones_dia(string& fecha) {
	map<string, Funcion> funciones_del_dia = map<string, Funcion>();
	// recorro las funciones, si no son del dia, las descarto
    for (map<string, Funcion>::iterator it = this->funciones.begin(); 
		it != this->funciones.end(); ++it) {
		// Lockeo el mutex segun el id
		Lock(*this->mutex_funciones.at(it->first));
		
		// aca verifico el dia
		if (it->second.getFecha() == fecha) {
			funciones_del_dia.emplace(it->first, it->second);
		}
	}

	return funciones_del_dia;
}

bool FuncionesProtected::reservar_asiento(string& id_funcion, string& fila, 
	string& columna) {
	// Lockeo el mutex segun el id
	Lock(*this->mutex_funciones.at(id_funcion));
	
	// iterador a la funcion
	map<string, Funcion>::iterator it = this->funciones.find(id_funcion);
    
    return it->second.reservarAsiento(fila, columna);
}

Funcion FuncionesProtected::asientos(string& id_funcion) {
	// Lockeo el mutex segun el id
	Lock(*this->mutex_funciones.at(id_funcion));
	
	// iterador a la funcion
	map<string, Funcion>::iterator it = this->funciones.find(id_funcion);

	return it->second;
}

FuncionesProtected::~FuncionesProtected() {
	for (map<string, mutex*>::iterator it = this->mutex_funciones.begin(); 
		it != this->mutex_funciones.end(); ++it) {
		delete it->second;
	}
}