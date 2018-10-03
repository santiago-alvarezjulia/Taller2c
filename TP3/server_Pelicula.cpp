#include "server_Pelicula.h"
#include <string>
using std::string;

Pelicula::Pelicula(string titulo, string idioma, string edad, string genero) : 
	titulo(titulo), idioma(idioma), edad(edad), genero(genero) {}
	
string Pelicula::getTitulo() {
	return this->titulo;
}

Pelicula::~Pelicula() {}
