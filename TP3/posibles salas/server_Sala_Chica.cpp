#include "server_Sala_Chica.h"
#include <string>
#include <array>
#define FILAS 5
#define COLUMNAS 6
#define ASIENTO_VACIO '0'
using std::string;
using std::array;

Sala_Chica::Sala_Chica() {}
	
string Sala_Chica::getCapacidad() {
	return Sala::getCapacidad();
}
	
void Sala_Chica::set_data_sala(string id, string pantalla, string capacidad) {
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

Sala_Chica::~Sala_Chica() {}
