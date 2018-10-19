#include "server_Sala.h"
#include <string>
#define CAPACIDAD_GRANDE "grande"
#define CAPACIDAD_MEDIANA "mediana"
using std::string;

Sala::Sala(string& id, string& pantalla, string& capacidad) : id(id), 
	pantalla(pantalla) {
	if (capacidad == CAPACIDAD_GRANDE) {
		this->cantidad_filas = 15;
		this->cantidad_columnas = 16;
	} else if (capacidad == CAPACIDAD_MEDIANA) {
		this->cantidad_filas = 10;
		this->cantidad_columnas = 11;
	} else {
		this->cantidad_filas = 5;
		this->cantidad_columnas = 6;
	}	
}

const string& Sala::getId() const {
	return this->id;
}

int Sala::getCantidadFilas() const {
	return this->cantidad_filas;
}

int Sala::getCantidadColumnas() const {
	return this->cantidad_columnas;
}

Sala::~Sala() {}
