#include "server_Parser.h"
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include "server_Sala.h"
#include "server_Pelicula.h"
#define DELIM_CSV ','
using std::map;
using std::string;
using std::fstream;
using std::vector;
using std::multimap;

Parser::Parser() {}

map<string, Sala> Parser::parsear_archivo_salas(fstream& archivo_salas) {
    map<string, Sala> salas;
	string id_sala;
	string pantalla;
	string capacidad;
	
	while (getline(archivo_salas, id_sala, DELIM_CSV) && 
		getline(archivo_salas, pantalla, DELIM_CSV) && 
		getline(archivo_salas, capacidad)) {
		salas.emplace(id_sala, Sala(id_sala, pantalla, capacidad));
	}

    return std::move(salas);
}

vector<multimap<string, Pelicula>> Parser::parsear_archivo_peliculas(
    fstream& archivo_peliculas) {
    string titulo;
	string idioma;
	string edad;
	string genero;
	multimap<string, Pelicula> pelicula_segun_idioma;
	multimap<string, Pelicula> pelicula_segun_edad;
	multimap<string, Pelicula> pelicula_segun_genero;
	multimap<string, Pelicula> pelicula_segun_titulo;
	vector<multimap<string, Pelicula>> peliculas;
	
	while (getline(archivo_peliculas, titulo, DELIM_CSV) && 
		getline(archivo_peliculas, idioma, DELIM_CSV) &&
		getline(archivo_peliculas, edad, DELIM_CSV) && 
		getline(archivo_peliculas, genero)) {
		Pelicula pelicula(titulo, idioma, edad, genero);
			
		pelicula_segun_idioma.emplace(idioma, pelicula);
		pelicula_segun_edad.emplace(edad, pelicula);
		pelicula_segun_genero.emplace(genero, pelicula);
		pelicula_segun_titulo.emplace(titulo, pelicula);
	}
	
	peliculas.emplace_back(std::move(pelicula_segun_idioma));
	peliculas.emplace_back(std::move(pelicula_segun_edad));
	peliculas.emplace_back(std::move(pelicula_segun_genero));
	peliculas.emplace_back(std::move(pelicula_segun_titulo));

    return std::move(peliculas);
}

Parser::~Parser() {}
