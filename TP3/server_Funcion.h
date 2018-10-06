#ifndef SERVER_FUNCION_H
#define SERVER_FUNCION_H

#include "server_Sala.h"
#include "server_Pelicula.h"
#include <string>
#include <vector>

class Funcion {
	std::string id;
	Sala sala;
	Pelicula pelicula;
	std::string fecha;
	std::string hora;
	std::vector<std::vector<char>> asientos;
	int asientos_ocupados;
	int fila_to_number(std::string fila);
	
	public:
		Funcion(std::string id, Sala sala, Pelicula pelicula, std::string fecha, 
			std::string hora);
		std::string getId();
		std::string getIdSala();
		std::string getTitulo();
		std::string getHora();
		std::string getFecha();
		int getCantidadFilas();
		int getCantidadColumnas();
		std::vector<std::vector<char>> getAsientos();
		bool esta_agotada();
		bool reservarAsiento(std::string fila, std::string columna);
		~Funcion();	
};

#endif // SERVER_FUNCION_H
