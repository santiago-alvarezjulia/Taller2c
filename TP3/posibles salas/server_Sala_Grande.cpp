#include "server_Sala_Grande.h"
#include <string>
#include <array>
#define FILAS 15
#define COLUMNAS 16
#define ASIENTO_VACIO '0'
using std::string;
using std::array;

Sala_Grande::Sala_Grande() {}
	
string Sala_Grande::getCapacidad() {
	return Sala::getCapacidad();
}
	
void Sala_Grande::set_data_sala(string id, string pantalla, string capacidad) {
	this->id = id;
	this->pantalla = pantalla;
	this->capacidad = capacidad;
	
	array<array<char, COLUMNAS>, FILAS> asientos_array;
	
	for (int i = 0; i < FILAS; i++) {
		array<char, COLUMNAS> fila;
		fila.fill(ASIENTO_VACIO);
		asientos_array[i] = fila;
	}
	
	this->asientos = asientos_array;	
				
}

Sala_Grande::~Sala_Grande() {}
