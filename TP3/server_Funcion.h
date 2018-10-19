#ifndef SERVER_FUNCION_H
#define SERVER_FUNCION_H

#include "server_Sala.h"
#include "server_Pelicula.h"
#include <string>
#include <vector>

class Funcion {
	std::string id;
	Sala& sala;
	Pelicula& pelicula;
	std::string fecha;
	std::string hora;
	std::vector<std::vector<char>> asientos;
	int asientos_ocupados;
	int fila_to_number(char fila);
	
	public:
		Funcion(std::string& id, Sala& sala, Pelicula& pelicula, 
			std::string& fecha, std::string& hora);
		const std::string& getId() const;
		const std::string& getIdSala() const;
		const std::string& getTitulo() const;
		const std::string& getHora() const;
		const std::string& getFecha() const;
		int getCantidadFilas() const;
		int getCantidadColumnas() const;
		std::vector<std::vector<char>>& getAsientos();
		bool esta_agotada() const;
		bool reservarAsiento(std::string& fila, std::string& columna);
		~Funcion();	
};

#endif // SERVER_FUNCION_H
