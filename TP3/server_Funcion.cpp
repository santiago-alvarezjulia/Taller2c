#include "server_Funcion.h"
#include <string>
using std::string;

Funcion::Funcion(string id, string id_sala, string titulo, string fecha, 
	string hora) : id(id), id_sala(id_sala), titulo(titulo), fecha(fecha),
	hora(hora) {}

string Funcion::getTitulo() {
	return this->titulo;
}
Funcion::~Funcion() {}
