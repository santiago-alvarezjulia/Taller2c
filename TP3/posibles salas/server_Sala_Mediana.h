#ifndef SERVER_SALA_MEDIANA_H
#define SERVER_SALA_MEDIANA_H

#include "server_Sala.h"
#include <string>
#include <array>
#define FILAS 10
#define COLUMNAS 11

class Sala_Mediana : Sala {
	std::string id;
	std::string pantalla;
	std::string capacidad;
	std::array<std::array<char, COLUMNAS>, FILAS> asientos;
	
	public:
		Sala_Mediana();
		virtual void set_data_sala(std::string id, std::string pantalla, 
			std::string capacidad);
		std::string getCapacidad();
		~Sala_Mediana();	

};

#endif // SERVER_SALA_MEDIANA_H
