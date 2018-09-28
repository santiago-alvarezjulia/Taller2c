#include "server_Sala_Mediana.h"
#include <string>
#include <array>
#define FILAS 10
#define COLUMNAS 11
#define ASIENTO_VACIO '0'
using std::string;
using std::array;

Sala_Mediana::Sala_Mediana() {}
	
string Sala_Mediana::getCapacidad() {
	return Sala::getCapacidad();
}
	
void Sala_Mediana::set_data_sala(string id, string pantalla, string capacidad) {
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

Sala_Mediana::~Sala_Chica() {}
