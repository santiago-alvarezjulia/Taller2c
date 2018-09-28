#include "server_Sala.h"
#include <string>
#include <vector>
#define ASIENTO_VACIO '0'
using std::vector;
using std::string;

Sala::Sala(string id, string pantalla, string capacidad) : id(id), 
	pantalla(pantalla), capacidad(capacidad) {
	int cant_filas;
	int cant_columnas;
	
	if (capacidad == "grande") {
		cant_filas = 15;
		cant_columnas = 16;
	} else if (capacidad == "mediana") {
		cant_filas = 10;
		cant_columnas = 11;
	} else {
		cant_filas = 5;
		cant_columnas = 6;
	}

	this->cantidad_filas = cant_filas;
	this->cantidad_columnas = cant_columnas;
	
	vector<vector<char>> asientos_vector;
	
	for (int i = 0; i < cant_filas; i++) {
		vector<char> fila (cant_columnas, ASIENTO_VACIO);
		asientos_vector.push_back(fila);
	}
}
	
Sala::~Sala() {}
