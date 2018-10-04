#include "server_Funcion.h"
#include "server_Sala.h"
#include "server_Pelicula.h"
#include <string>
#include <vector>
#define ASIENTO_VACIO 'O'
#define ASIENTO_OCUPADO 'X'
using std::vector;
using std::string;

Funcion::Funcion(string id, Sala sala, Pelicula pelicula, string fecha, 
	string hora) : id(id), sala(sala), pelicula(pelicula), fecha(fecha), 
	hora(hora) {
	int cant_filas = sala.getCantidadFilas();
	int cant_columnas = sala.getCantidadColumnas();
	
	vector<vector<char>> asientos_vector;
	
	for (int i = 0; i < cant_filas; i++) {
		vector<char> fila (cant_columnas, ASIENTO_VACIO);
		asientos_vector.push_back(fila);
	}

	this->asientos = asientos_vector;
	this->asientos_ocupados = 0;
}

string Funcion::getId() {
	return this->id;
}

string Funcion::getIdSala() {
	return this->sala.getId();
}

string Funcion::getTitulo() {
	return this->pelicula.getTitulo();
}

string Funcion::getHora() {
	return this->hora;
}

int Funcion::getCantidadFilas() {
	return this->sala.getCantidadFilas();
}

int Funcion::getCantidadColumnas() {
	return this->sala.getCantidadColumnas();
}

std::vector<std::vector<char>> Funcion::getAsientos() {
	return this->asientos;
}

bool Funcion::esta_agotada() {
	int cantidad_total_asientos = this->sala.getCantidadColumnas() * 
		this->sala.getCantidadFilas();
	return this->asientos_ocupados == cantidad_total_asientos;
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
