#ifndef SERVER_PARSER_H
#define SERVER_PARSER_H

#include <string>
#include <fstream>
#include <map>
#include <vector>
#include "server_Sala.h"
#include "server_Pelicula.h"
#include "server_FuncionesProtected.h"
#include "server_Funcion.h"

class Parser {
	public:
		Parser();
        std::map<std::string, Sala> parsear_archivo_salas(
            std::fstream& archivo_salas);
        std::vector<std::multimap<std::string, Pelicula>> 
            parsear_archivo_peliculas(std::fstream& archivo_peliculas);
        FuncionesProtected parsear_archivo_funciones(std::fstream& 
            archivo_funciones, std::map<std::string, Sala>& salas, 
            std::multimap<std::string, Pelicula>& peliculas);
		~Parser();	
};

#endif // SERVER_PARSER_H
