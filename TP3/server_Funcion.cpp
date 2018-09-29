#include "server_Funcion.h"
#include <string>
#include <vector>
#define ASIENTO_VACIO 'O'
#define ASIENTO_OCUPADO 'X'
using std::vector;
using std::string;

Funcion::Funcion(string id, string id_sala, string titulo, string fecha, 
	string hora, string capacidad) : id(id), id_sala(id_sala), titulo(titulo), 
	fecha(fecha), hora(hora) {
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
	
	vector<vector<char>> asientos_vector;
	
	for (int i = 0; i < cant_filas; i++) {
		vector<char> fila (cant_columnas, ASIENTO_VACIO);
		asientos_vector.push_back(fila);
	}
	

	this->asientos = asientos_vector;
	this->cantidad_filas = cant_filas;
	this->cantidad_columnas = cant_columnas;
	this->asientos_ocupados = 0;
	this->cantidad_total_asientos = cant_filas * cant_columnas;
}

string Funcion::getTitulo() {
	return this->titulo;
}

int Funcion::getCantidadFilas() {
	return this->cantidad_filas;
}

int Funcion::getCantidadColumnas() {
	return this->cantidad_columnas;
}

std::vector<std::vector<char>> Funcion::getAsientos() {
	return this->asientos;
}

bool Funcion::esta_agotada() {
	return this->asientos_ocupados == this->cantidad_total_asientos;
}

bool Funcion::reservarAsiento(string fila, string columna) {
	int fila_numerada = fila_to_number(fila);
	int columna_numerada = std::stoi(columna) - 1;
	
	if (this->asientos[fila_numerada][columna_numerada] == ASIENTO_OCUPADO) {
		return false;
	}
	
	this->asientos[fila_numerada][columna_numerada] = ASIENTO_OCUPADO;
	this->asientos_ocupados++;
	return true;
}

int Funcion::fila_to_number(string fila) {
	int ascii_number = static_cast<int>(fila[0]);
	return ascii_number - 65; // A = 65 en Ascii
}

Funcion::~Funcion() {}
