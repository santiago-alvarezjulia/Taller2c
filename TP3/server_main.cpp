#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "server_Sala.h"
#include "server_Pelicula.h"
#include "server_Funcion.h"
#define CANT_PARAMETROS 5
#define POS_PORT 1
#define POS_SALAS 2
#define POS_PELICULAS 3
#define POS_FUNCIONES 4
#define ERROR_PARAMETROS 1
#define ERROR_ARCHIVOS 2
#define OK 0
#define DELIM_CSV ','
#define DELIM_FIN_LINEA '\n';
#define CANT_COLUMNAS_ARCHIVO_PELICULAS 4
#define CANT_COLUMNAS_ARCHIVO_FUNCIONES 4
#define CANT_COLUMNAS_ARCHIVO_SALAS 3
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
	int pos_en_linea = 0;
	string data;
	char delim;
	string id;
	string pantalla;
	string capacidad;
	
	do {
		if (pos_en_linea == CANT_COLUMNAS_ARCHIVO_SALAS - 1) {
			delim = DELIM_FIN_LINEA;
		} else {
			delim = DELIM_CSV;
		}
		
		getline(archivo_salas, data, delim);
		if (data.empty()) {
			break;
		}
		
		//cout << "pos: " << pos_en_linea << ". data: " << data << endl;
		
		switch (pos_en_linea) {
			case 0:
				id = data;
				break;
			case 1:
				pantalla = data;
				break;
			case 2:
				capacidad = data;
				break;
		}
		
		pos_en_linea++;
		if (pos_en_linea == CANT_COLUMNAS_ARCHIVO_SALAS) {
			Sala sala(id, pantalla, capacidad);
			salas.insert(std::pair<string, Sala>(id, sala));
			pos_en_linea = 0;
		}
	} while (!archivo_salas.eof());
	
	return salas;
}


vector<multimap<string, Pelicula>> clasificar_peliculas(
	fstream& archivo_peliculas) {
	int pos_en_linea = 0;
	string data;
	char delim;
	string titulo;
	string idioma;
	string edad;
	string genero;
	multimap<string, Pelicula> pelicula_segun_idioma;
	multimap<string, Pelicula> pelicula_segun_edad;
	multimap<string, Pelicula> pelicula_segun_genero;
	multimap<string, Pelicula> pelicula_segun_titulo;
	vector<multimap<string, Pelicula>> clasificacion;
	
	do {
		if (pos_en_linea == CANT_COLUMNAS_ARCHIVO_PELICULAS - 1) {
			delim = DELIM_FIN_LINEA;
		} else {
			delim = DELIM_CSV;
		}
		
		getline(archivo_peliculas, data, delim);
		if (data.empty()) {
			break;
		}
		
		//cout << "pos: " << pos_en_linea << ". data: " << data << endl;
		
		switch (pos_en_linea) {
			case 0:
				titulo = data;
				break;
			case 1:
				idioma = data;
				break;
			case 2:
				edad = data;
				break;
			case 3:
				genero = data;
				break;
		}
		
		pos_en_linea++;
		if (pos_en_linea == CANT_COLUMNAS_ARCHIVO_PELICULAS) {
			pos_en_linea = 0;
			
			Pelicula pelicula(titulo, idioma, edad, genero);
			
			pelicula_segun_idioma.insert(std::pair<string, Pelicula>(idioma, 
				pelicula));
			pelicula_segun_edad.insert(std::pair<string, Pelicula>(edad, 
				pelicula));
			pelicula_segun_genero.insert(std::pair<string, Pelicula>(genero, 
				pelicula));
			pelicula_segun_titulo.insert(std::pair<string, Pelicula>(titulo, 
				pelicula));
		}
	} while (!archivo_peliculas.eof());
	
	clasificacion.push_back(pelicula_segun_idioma);
	clasificacion.push_back(pelicula_segun_edad);
	clasificacion.push_back(pelicula_segun_genero);
	clasificacion.push_back(pelicula_segun_titulo);
	
	return clasificacion;
}


vector<multimap<string, Funcion>> clasificar_funciones(
	fstream& archivo_funciones, map<string, Sala> salas, 
	multimap<string, Pelicula> peliculas) {
	int pos_en_linea = 0;
	int id_funcion = 1;
	string data;
	char delim;
	string id_sala;
	string titulo;
	string fecha;
	string hora;
	multimap<string, Funcion> funcion_segun_fecha;
	multimap<string, Funcion> funcion_segun_id;
	vector<multimap<string, Funcion>> clasificacion;
	
	do {
		if (pos_en_linea == CANT_COLUMNAS_ARCHIVO_FUNCIONES - 1) {
			delim = DELIM_FIN_LINEA;
		} else {
			delim = DELIM_CSV;
		}
		
		getline(archivo_funciones, data, delim);
		if (data.empty()) {
			break;
		}
		
		//cout << "pos: " << pos_en_linea << ". data: " << data << endl;
		
		switch (pos_en_linea) {
			case 0:
				id_sala = data;
				if (salas.find(id_sala) == salas.end()) {
					cerr << 
					"La sala " << id_sala << " no existe en el sistema." 
					<< endl;
					//return NULL;
					// TAL VEZ EXCEPCION
				}
				break;
			case 1:
				titulo = data;
				if (peliculas.find(titulo) == peliculas.end()) {
					cerr << 
					"La película " << titulo << " no existe en el sistema." 
					<< endl;
					//return NULL;
					// TAL VEZ EXCEPCION
				}
				break;
			case 2:
				fecha = data;
				break;
			case 3:
				hora = data;
				break;
		}
		
		pos_en_linea++;
		if (pos_en_linea == CANT_COLUMNAS_ARCHIVO_FUNCIONES) {
			map<string, Sala>::iterator it = salas.find(id_sala);
			
			Funcion funcion(to_string(id_funcion), id_sala, titulo, fecha, hora,
				it->second.getCapacidad());
			
			funcion_segun_fecha.insert(std::pair<string, Funcion>(fecha, 
				funcion));
			funcion_segun_id.insert(std::pair<string, Funcion>
				(to_string(id_funcion), funcion));
			
			pos_en_linea = 0;
			id_funcion++;
		}
	} while (!archivo_funciones.eof());
	
	clasificacion.push_back(funcion_segun_fecha);
	clasificacion.push_back(funcion_segun_id);
	
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
	
	// puerto de conexion
	//char* port = argv[POS_PORT];
	
	// Abro el archivo de salas.
	fstream archivo_salas((const char*)argv[POS_SALAS], fstream::in);
	if (archivo_salas.fail()) {
		cerr << "El archivo " << argv[POS_SALAS] << " no existe." << endl;
		return ERROR_PARAMETROS;
	}
	
	// Abro el archivo de salas.
	fstream archivo_peliculas((const char*)argv[POS_PELICULAS], fstream::in);
	if (archivo_peliculas.fail()) {
		cerr << "El archivo " << argv[POS_PELICULAS] << " no existe." << endl;
		archivo_salas.close();
		return ERROR_PARAMETROS;
	}
	
	// Abro el archivo de salas.
	fstream archivo_funciones((const char*)argv[POS_FUNCIONES], fstream::in);
	if (archivo_funciones.fail()) {
		cerr << "El archivo " << argv[POS_FUNCIONES] << " no existe." << endl;
		archivo_salas.close();
		archivo_peliculas.close();
		return ERROR_PARAMETROS;
	}
	
	
	map<string, Sala> clasificacion_salas = clasificar_salas(archivo_salas);
	
	vector<multimap<string, Pelicula>> clasificacion_peliculas = 
		clasificar_peliculas(archivo_peliculas);
		
	vector<multimap<string, Funcion>> clasificacion_funciones =
		clasificar_funciones(archivo_funciones, clasificacion_salas, 
		clasificacion_peliculas[3]);
	
	//if (clasificacion_funciones == NULL) {
	//	return ERROR_ARCHIVOS;
	//}
	
	/*
	// imprimo titulos con idioma SUB
	multimap<string, string> pelicula_segun_idioma = clasificacion_peliculas[0];
	string idioma = "SUB";
	std::pair<multimap<string, string>::iterator, multimap<string, string>::iterator> ret;
    ret = pelicula_segun_idioma.equal_range(idioma);
    for (multimap<string, string>::iterator it = ret.first; it != ret.second; ++it) {
      cout << it->second << endl;
	}
	
	cout << endl;
	
	// imprimo titulos del genero Drama
	multimap<string, string> pelicula_segun_genero = clasificacion_peliculas[2];
	string genero = "Drama";
	std::pair<multimap<string, string>::iterator, multimap<string, string>::iterator> ret2;
    ret2 = pelicula_segun_genero.equal_range(genero);
    for (multimap<string, string>::iterator it = ret2.first; it != ret2.second; ++it) {
      cout << it->second << endl;
	}
	 
	
	// imprimo titulos de FECHA 25/09/2018
	multimap<string, Funcion> funciones_segun_fecha = clasificacion_funciones[0];
	string fecha = "25/09/2018";
	std::pair<multimap<string, Funcion>::iterator, multimap<string, Funcion>::iterator> ret;
    ret = funciones_segun_fecha.equal_range(fecha);
    for (multimap<string, Funcion>::iterator it = ret.first; it != ret.second; ++it) {
		cout << it->second.getTitulo() << endl;
	}
	
	
	// imprimo sala de funcion 1
	int cantidad_columnas = clasificacion_funciones[1].find("1")->second.getCantidadColumnas();
	for (int i = 0; i < cantidad_columnas; i++) {
		cout << '\t' << i+1;
	}
	cout << endl;
	
	
	std::vector<std::vector<char>> asientos = 
		clasificacion_funciones[1].find("1")->second.getAsientos();
	int col = 0;
	std::string columnas_posibles = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	for (std::vector<std::vector<char>>::iterator it = asientos.begin() ; it != asientos.end(); ++it) {
		cout << columnas_posibles[col];
		for (std::vector<char>::iterator it2 = (*it).begin() ; it2 != (*it).end(); ++it2) {
			cout << '\t' << *it2;
		}
		
		col++;
		cout << endl;
	}
	cout << endl;
	cout << endl;
	cout << endl;
	// reservo asiento A-3 en funcion 1 y despues la imprimo
	bool reservar_asiento = 
		clasificacion_funciones[1].find("1")->second.reservarAsiento("A", "3");
	cout << reservar_asiento << endl;
	for (int i = 0; i < cantidad_columnas; i++) {
		cout << '\t' << i+1;
	}
	cout << endl;
	
	std::vector<std::vector<char>> asientos2 = 
		clasificacion_funciones[1].find("1")->second.getAsientos();
	col = 0;
	for (std::vector<std::vector<char>>::iterator it2 = asientos2.begin() ; it2 != asientos2.end(); ++it2) {
		cout << columnas_posibles[col];
		for (std::vector<char>::iterator it3 = (*it2).begin() ; it3 != (*it2).end(); ++it3) {
			cout << '\t' << *it3;
		}
		
		col++;
		cout << endl;
	}
	*/
	
	archivo_salas.close();
	archivo_peliculas.close();
	archivo_funciones.close();
	
	return OK;
}
