#include "server_Sala.h"
#include <string>
using std::string;

Sala::Sala(string id, string pantalla, string capacidad) : id(id), 
	pantalla(pantalla), capacidad(capacidad) {}
	
string Sala::getCapacidad() {
	return this->capacidad;
}

Sala::~Sala() {}
