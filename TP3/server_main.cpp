#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "server_Sala.h"
#include "server_Pelicula.h"
#include "server_Funcion.h"
#include "common_socket.h"
#include "server_End_App.h"
#include "server_Server.h"
#define CANT_PARAMETROS 5
#define POS_PORT 1
#define POS_SALAS 2
#define POS_PELICULAS 3
#define POS_FUNCIONES 4
#define ERROR_PARAMETROS 1
#define ERROR_ARCHIVOS 2
#define OK 0
#define DELIM_CSV ','
#define CANT_COLUMNAS_ARCHIVO_PELICULAS 4
#define CANT_COLUMNAS_ARCHIVO_FUNCIONES 4
#define CANT_COLUMNAS_ARCHIVO_SALAS 3
#define FIN_ENVIO_SOCKET 0
using std::fstream;
using std::cerr;
using std::cout;
using std::endl;
using std::multimap;
using std::string;
using std::vector;
using std::map;
using std::to_string;


map<string, Sala> clasificar_salas(fstream& archivo_salas) {
	map<string, Sala> salas;
	string id;
	string pantalla;
	string capacidad;
	
	while (getline(archivo_salas, id, DELIM_CSV) && 
		getline(archivo_salas, pantalla, DELIM_CSV) && 
		getline(archivo_salas, capacidad)) {
		salas.emplace(id, Sala (id, pantalla, capacidad));
	}
	
	return salas;
}


vector<multimap<string, Pelicula>> clasificar_peliculas(
	fstream& archivo_peliculas) {
	string titulo;
	string idioma;
	string edad;
	string genero;
	multimap<string, Pelicula> pelicula_segun_idioma;
	multimap<string, Pelicula> pelicula_segun_edad;
	multimap<string, Pelicula> pelicula_segun_genero;
	multimap<string, Pelicula> pelicula_segun_titulo;
	vector<multimap<string, Pelicula>> clasificacion;
	
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
	
	clasificacion.emplace_back(pelicula_segun_idioma);
	clasificacion.emplace_back(pelicula_segun_edad);
	clasificacion.emplace_back(pelicula_segun_genero);
	clasificacion.emplace_back(pelicula_segun_titulo);
	
	return clasificacion;
}


vector<multimap<string, Funcion>> clasificar_funciones(
	fstream& archivo_funciones, map<string, Sala>& salas, 
	multimap<string, Pelicula>& peliculas) {
	int id_funcion = 1;
	string id_sala;
	string titulo;
	string fecha;
	string hora;
	multimap<string, Funcion> funcion_segun_fecha;
	multimap<string, Funcion> funcion_segun_id;
	vector<multimap<string, Funcion>> clasificacion;
	
	while (getline(archivo_funciones, id_sala, DELIM_CSV) && 
		getline(archivo_funciones, titulo, DELIM_CSV) &&
		getline(archivo_funciones, fecha, DELIM_CSV) && 
		getline(archivo_funciones, hora)) {
		map<string, Sala>::iterator it_salas = salas.find(id_sala);
		if (it_salas == salas.end()) {
			cerr << 
			"La sala " << id_sala << " no existe en el sistema." 
			<< endl;
			// TAL VEZ EXCEPCION
		}
		
		map<string, Pelicula>::iterator it_peliculas = peliculas.find(titulo);
		if (it_peliculas == peliculas.end()) {
			cerr << 
			"La película " << titulo << " no existe en el sistema." 
			<< endl;
			// TAL VEZ EXCEPCION
		}
			
		Funcion funcion(to_string(id_funcion), it_salas->second, 
		it_peliculas->second, fecha, hora);
			
		funcion_segun_fecha.emplace(fecha, funcion);
		funcion_segun_id.emplace(to_string(id_funcion), funcion);
			
		id_funcion++;
	}
	
	clasificacion.emplace_back(funcion_segun_fecha);
	clasificacion.emplace_back(funcion_segun_id);
	
	return clasificacion;
}


int main(int argc, char* argv []) {
	// verifico cantidad de parametros
	if (argc != CANT_PARAMETROS) {
		cerr << 
		"Uso: ./server <puerto> <salas.csv> <peliculas.csv> <funciones.csv>" 
		<< endl;
		return ERROR_PARAMETROS;
	}
	
	// Abro el archivo de salas.
	fstream archivo_salas((const char*)argv[POS_SALAS], fstream::in);
	if (archivo_salas.fail()) {
		cerr << "El archivo " << argv[POS_SALAS] << " no existe." << endl;
		return ERROR_PARAMETROS;
	}
	
	// Abro el archivo de peliculas.
	fstream archivo_peliculas((const char*)argv[POS_PELICULAS], fstream::in);
	if (archivo_peliculas.fail()) {
		cerr << "El archivo " << argv[POS_PELICULAS] << " no existe." << endl;
		archivo_salas.close();
		return ERROR_PARAMETROS;
	}
	
	// Abro el archivo de funciones.
	fstream archivo_funciones((const char*)argv[POS_FUNCIONES], fstream::in);
	if (archivo_funciones.fail()) {
		cerr << "El archivo " << argv[POS_FUNCIONES] << " no existe." << endl;
		archivo_salas.close();
		archivo_peliculas.close();
		return ERROR_PARAMETROS;
	}
	
	// Parseo el archivo de salas.
	map<string, Sala> clasificacion_salas = clasificar_salas(archivo_salas);
	
	// Parseo el archivo de peliculas.
	vector<multimap<string, Pelicula>> clasificacion_peliculas = 
		clasificar_peliculas(archivo_peliculas);
		
	// Parseo el archivo de funciones.	
	vector<multimap<string, Funcion>> clasificacion_funciones =
		clasificar_funciones(archivo_funciones, clasificacion_salas, 
		clasificacion_peliculas[3]);
	
	Socket main_socket;
	main_socket.bind_and_listen(argv[POS_PORT]);
	Socket asoc = main_socket.accept_();
	
	Server server(asoc, clasificacion_peliculas, clasificacion_funciones);
	server.run();
	
	// Lanzo un thread (End_App) que lee std::cin. Si lee una q, frena todo 
	// el programa.
	End_App thread_end_app;
	thread_end_app.start();
	while (true) {
		//Socket asoc = main_socket.accept_();
		if (!thread_end_app.isAlive()) {
			thread_end_app.join();
			break;
		}
	}
	
	return OK;
}
