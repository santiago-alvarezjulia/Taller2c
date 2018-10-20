#include "server_Pelicula.h"
#include <string>
using std::string;

Pelicula::Pelicula(string& titulo, string& idioma, string& edad, 
	string& genero) : titulo(titulo), idioma(idioma), edad(edad), 
	genero(genero) {}
	
const string& Pelicula::getTitulo() const{
	return this->titulo;
}

Pelicula::~Pelicula() {}
