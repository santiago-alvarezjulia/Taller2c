#include "server_Funcion.h"
#include "server_Sala.h"
#include "server_Pelicula.h"
#include <string>
#include <vector>
#define ASIENTO_VACIO 'O'
#define ASIENTO_OCUPADO 'X'
using std::vector;
using std::string;

Funcion::Funcion(string& id, Sala& sala, Pelicula& pelicula, string& fecha, 
	string& hora) : id(id), sala(sala), pelicula(pelicula), fecha(fecha), 
	hora(hora) {
	int cant_filas = sala.getCantidadFilas();
	int cant_columnas = sala.getCantidadColumnas();
	
	vector<vector<char>> asientos_vector;
	
	for (int i = 0; i < cant_filas; i++) {
		vector<char> fila(cant_columnas, ASIENTO_VACIO);
		asientos_vector.emplace_back(fila);
	}

	this->asientos = std::move(asientos_vector);
	this->asientos_ocupados = 0;
}

const string& Funcion::getId() const {
	return this->id;
}

const string& Funcion::getIdSala() const {
	return this->sala.getId();
}

const string& Funcion::getTitulo() const {
	return this->pelicula.getTitulo();
}

const string& Funcion::getHora() const {
	return this->hora;
}

const string& Funcion::getFecha() const {
	return this->fecha;
}

int Funcion::getCantidadFilas() const {
	return this->sala.getCantidadFilas();
}

int Funcion::getCantidadColumnas() const {
	return this->sala.getCantidadColumnas();
}

std::vector<std::vector<char>>& Funcion::getAsientos() {
	return this->asientos;
}

bool Funcion::esta_agotada() const {
	int cantidad_total_asientos = this->sala.getCantidadColumnas() * 
		this->sala.getCantidadFilas();
	return this->asientos_ocupados == cantidad_total_asientos;
}

bool Funcion::reservarAsiento(string& fila, string& columna) {
	int fila_numerada = fila_to_number(fila[0]);
	int columna_numerada = std::stoi(columna) - 1;
	
	if (this->asientos[fila_numerada][columna_numerada] == ASIENTO_OCUPADO) {
		return false;
	}
	
	this->asientos[fila_numerada][columna_numerada] = ASIENTO_OCUPADO;
	this->asientos_ocupados++;
	return true;
}

int Funcion::fila_to_number(char fila) {
	int ascii_number = static_cast<int>(fila);
	return ascii_number - 65; // A = 65 en Ascii
}

Funcion::~Funcion() {}
