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
#include "common_GeneralError.h"
#include "common_SocketError.h"
#include "server_Parser.h"
#define CANT_PARAMETROS 5
#define POS_PORT 1
#define POS_SALAS 2
#define POS_PELICULAS 3
#define POS_FUNCIONES 4
#define ERROR_PARAMETROS 1
#define ERROR_ARCHIVOS 2
#define ERROR_SOCKET 3
#define UNKNOWN_ERROR 4
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


int main(int argc, char* argv []) {
	// codigo de retorno que puede ser modificado en algun catch o antes del
	// throw
	int return_value = OK;

	try {
		// verifico cantidad de parametros
		if (argc != CANT_PARAMETROS) {
			return_value = ERROR_PARAMETROS;
			throw GeneralError(
			"Uso: ./server <puerto> <salas.csv> <peliculas.csv> <funciones.csv>");
		}

		// Abro el archivo de salas.
		fstream archivo_salas((const char*)argv[POS_SALAS], fstream::in);
		if (archivo_salas.fail()) {
			return_value = ERROR_PARAMETROS;
			throw GeneralError("El archivo %s no existe.", argv[POS_SALAS]);
		}
	
		// Abro el archivo de peliculas.
		fstream archivo_peliculas((const char*)argv[POS_PELICULAS], fstream::in);
		if (archivo_peliculas.fail()) {
			return_value = ERROR_PARAMETROS;
			throw GeneralError("El archivo %s no existe.", argv[POS_PELICULAS]);
		}

		// Abro el archivo de funciones.
		fstream archivo_funciones((const char*)argv[POS_FUNCIONES], fstream::in);
		if (archivo_funciones.fail()) {
			return_value = ERROR_PARAMETROS;
			throw GeneralError("El archivo %s no existe.", argv[POS_FUNCIONES]);
		}

		// Inicializo el parser
		Parser parser;

		// Parseo el archivo de salas.
		map<string, Sala> salas = parser.parsear_archivo_salas(archivo_salas);

		// Parseo el archivo de peliculas.
		vector<multimap<string, Pelicula>> peliculas = parser
			.parsear_archivo_peliculas(archivo_peliculas);
		
		// Parseo el archivo de funciones.
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
				return_value = ERROR_ARCHIVOS;
				throw GeneralError("La sala %s no existe en el sistema.", 
					id_sala);
			}
		
			map<string, Pelicula>::iterator it_peliculas = peliculas[3].find(titulo);
			if (it_peliculas == peliculas[3].end()) {
				// throw general excepcion
				return_value = ERROR_ARCHIVOS;
				throw GeneralError("La película %s no existe en el sistema.", 
					titulo);
			}
			
			string id_funcion_str = to_string(id_funcion);
			Funcion funcion(id_funcion_str, it_salas->second, 
				it_peliculas->second, fecha, hora);
			
			funciones.agregar_funcion(id_funcion_str, funcion);
			
			id_funcion++;
		}
		
		// creo el hilo que va a aceptar conexiones de clientesy lo lanzo
		// puede lanzar SocketError
		Multi_Client_Acceptor thread_acceptor(argv[POS_PORT], peliculas, 
			funciones);
		thread_acceptor.start();
	
		// leo por entrada estandar std::cin 
		char input = cin.get();
		while (input != END_APP) {
			input = cin.get();
		}
		
		// recibí END_APP. Freno el hilo que acepta nuevos clientes y joineo
		thread_acceptor.frenar();
		thread_acceptor.join();
	} catch (const SocketError& e) {
		cerr << e.what() << endl;
		return_value = ERROR_SOCKET;
	} catch (const GeneralError& e) {
		cerr << e.what() << endl;
	} catch (const std::exception& e) {
		cout << e.what() << endl;
		return_value = UNKNOWN_ERROR;
	} catch (...) {
		cout << "Error desconocido" << endl;
		return_value = UNKNOWN_ERROR;
	}
	
	return return_value;
}
