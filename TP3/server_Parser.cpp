#include "server_Parser.h"
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include "server_Sala.h"
#include "server_Pelicula.h"
#include "server_FuncionesProtected.h"
#include "server_Funcion.h"
#include "common_GeneralError.h"
#define DELIM_CSV ','
using std::map;
using std::string;
using std::fstream;
using std::vector;
using std::multimap;
using std::to_string;

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

FuncionesProtected Parser::parsear_archivo_funciones(fstream& archivo_funciones,
	map<string, Sala>& salas, multimap<string, Pelicula>& peliculas) {
	int id_funcion = 1;
	string id_sala;
	string titulo;
	string fecha;
	string hora;
	FuncionesProtected funciones;
	
	while (getline(archivo_funciones, id_sala, DELIM_CSV) && 
		getline(archivo_funciones, titulo, DELIM_CSV) &&
		getline(archivo_funciones, fecha, DELIM_CSV) && 
		getline(archivo_funciones, hora)) {
		map<string, Sala>::iterator it_salas = salas.find(id_sala);
		if (it_salas == salas.end()) {
			// throw general excepcion
			throw GeneralError("La sala %s no existe en el sistema.", 
				id_sala);
		}
	
		multimap<string, Pelicula>::iterator it_peliculas = peliculas
			.find(titulo);
		if (it_peliculas == peliculas.end()) {
			// throw general excepcion
			throw GeneralError("La película %s no existe en el sistema.", 
				titulo);
		}
			
		string id_funcion_str = to_string(id_funcion);
		Funcion funcion(id_funcion_str, it_salas->second, 
			it_peliculas->second, fecha, hora);
		
		funciones.agregar_funcion(id_funcion_str, funcion);
		
		id_funcion++;
	}

	return std::move(funciones);
}

Parser::~Parser() {}
