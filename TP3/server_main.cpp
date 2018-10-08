#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <mutex>
#include "server_Sala.h"
#include "server_Pelicula.h"
#include "server_Funcion.h"
#include "common_socket.h"
#include "server_ThreadServer.h"
#include "server_Multi_Client_Acceptor.h"
#include "server_FuncionesProtected.h"
#include "server_ArchivoEntradaError.h"
#include "common_SocketError.h"
#define CANT_PARAMETROS 5
#define POS_PORT 1
#define POS_SALAS 2
#define POS_PELICULAS 3
#define POS_FUNCIONES 4
#define ERROR_PARAMETROS 1
#define ERROR_ARCHIVOS 2
#define ERROR_SOCKET 3
#define OK 0
#define DELIM_CSV ','
#define CANT_COLUMNAS_ARCHIVO_PELICULAS 4
#define CANT_COLUMNAS_ARCHIVO_FUNCIONES 4
#define CANT_COLUMNAS_ARCHIVO_SALAS 3
#define FIN_ENVIO_SOCKET 0
#define END_APP 'q'
using std::cin;
using std::fstream;
using std::cerr;
using std::cout;
using std::endl;
using std::multimap;
using std::string;
using std::vector;
using std::map;
using std::to_string;
using std::mutex;


map<string, Sala> clasificar_salas(fstream& archivo_salas) {
	map<string, Sala> salas;
	string id;
	string pantalla;
	string capacidad;
	
	while (getline(archivo_salas, id, DELIM_CSV) && 
		getline(archivo_salas, pantalla, DELIM_CSV) && 
		getline(archivo_salas, capacidad)) {
		salas.emplace(id, Sala(id, pantalla, capacidad));
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


FuncionesProtected clasificar_funciones(
	fstream& archivo_funciones, map<string, Sala>& salas, 
	multimap<string, Pelicula>& peliculas) {
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
			string message_error = "La sala " + id_sala + 
				" no existe en el sistema.";
			// throw excepcion
			throw ArchivoEntradaError(message_error);
		}
		
		map<string, Pelicula>::iterator it_peliculas = peliculas.find(titulo);
		if (it_peliculas == peliculas.end()) {
			string message_error = "La película " + titulo + 
				" no existe en el sistema.";
			// throw excepcion
			throw ArchivoEntradaError(message_error);
		}
			
		Funcion funcion(to_string(id_funcion), it_salas->second, 
			it_peliculas->second, fecha, hora);
			
		funciones.emplace_funcion(to_string(id_funcion), funcion);
			
		id_funcion++;
	}
	
	return funciones;
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
		return ERROR_PARAMETROS;
	}
	
	// Abro el archivo de funciones.
	fstream archivo_funciones((const char*)argv[POS_FUNCIONES], fstream::in);
	if (archivo_funciones.fail()) {
		cerr << "El archivo " << argv[POS_FUNCIONES] << " no existe." << endl;
		return ERROR_PARAMETROS;
	}
	
	// Parseo el archivo de salas.
	map<string, Sala> clasificacion_salas = clasificar_salas(archivo_salas);
	
	// Parseo el archivo de peliculas.
	vector<multimap<string, Pelicula>> clasificacion_peliculas = 
		clasificar_peliculas(archivo_peliculas);
		
		
	try {
		// Parseo el archivo de funciones.
		FuncionesProtected clasificacion_funciones = clasificar_funciones(
			archivo_funciones, clasificacion_salas, clasificacion_peliculas[3]);
			
		// creo el socket que escucha clientes nuevos. Ambas lineas pueden 
		// lanzar SocketError
		Socket main_socket;
		main_socket.bind_and_listen(argv[POS_PORT]);
		
		// creo el hilo que va a aceptar conexiones de clientesy lo lanzo
		Multi_Client_Acceptor thread_acceptor(main_socket, 
			clasificacion_peliculas, clasificacion_funciones);
		thread_acceptor.start();
	
		// leo por entrada estandar std::cin 
		char input;
		while (true) {
			input = cin.get();
			if (input == END_APP) {
				// recibí END_APP. Freno el hilo que acepta nuevos clientes y joineo
				thread_acceptor.frenar();
				thread_acceptor.join();
				break;
			}
		}
	} catch (const ArchivoEntradaError& e) {
		cerr << e.what() << endl;
		return ERROR_ARCHIVOS;
	} catch (const SocketError& e) {
		// no printeo e.what para que no fallen las pruebas en sercom
		return ERROR_SOCKET;
	} 
	
	return OK;
}
